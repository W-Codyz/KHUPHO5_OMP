'use strict';

const http = require('http');
const fs   = require('fs');
const path = require('path');
const url  = require('url');

// ── Config ─────────────────────────────────────────────────────────────────
const cfgPath = path.join(__dirname, 'config.json');
const cfg = Object.assign(
    { port: 7779, musicDir: './music' },
    fs.existsSync(cfgPath) ? JSON.parse(fs.readFileSync(cfgPath, 'utf8')) : {}
);

const MUSIC_DIR  = path.resolve(__dirname, cfg.musicDir);
const PORT       = cfg.port;
const AUDIO_EXT  = /\.(mp3|ogg|wav)$/i;
const MAX_RESULT = 30;

if (!fs.existsSync(MUSIC_DIR)) {
    fs.mkdirSync(MUSIC_DIR, { recursive: true });
    console.log(`[Music] Da tao thu muc nhac: ${MUSIC_DIR}`);
}

// ── Helpers ─────────────────────────────────────────────────────────────────

// Chuan hoa tieng Viet co dau → khong dau, chu thuong
// Vi du: "Chạy Ngay Đi" → "chay ngay di"
function normalize(str) {
    return str
        .normalize('NFD')
        .replace(/[̀-ͯ]/g, '')
        .replace(/đ/g, 'd').replace(/Đ/g, 'D')
        .toLowerCase()
        .trim();
}

// Tinh diem fuzzy giua ten bai hat va chuoi tim kiem (diem cao = phu hop hon)
function fuzzyScore(displayName, query) {
    const t = normalize(displayName);
    const q = normalize(query);
    if (!q) return 1;
    if (t === q) return 100;
    if (t.startsWith(q)) return 80;
    if (t.includes(q)) return 60;
    // tat ca tu trong query xuat hien trong ten
    const words = q.split(/\s+/).filter(Boolean);
    if (words.length > 1 && words.every(w => t.includes(w))) return 40;
    // so tu phu hop rieng le
    const matchedWords = words.filter(w => t.includes(w));
    if (matchedWords.length > 0) return 20 + matchedWords.length * 5;
    // khop theo thu tu ky tu (subsequence)
    let ti = 0, qi = 0;
    while (ti < t.length && qi < q.length) {
        if (t[ti] === q[qi]) qi++;
        ti++;
    }
    if (qi === q.length) return Math.max(1, 15 - (t.length - q.length) * 0.2);
    return 0;
}

// Lay danh sach nhac, loc/sap xep theo fuzzy search neu co tu khoa
function getMusicList(search) {
    let files;
    try {
        files = fs.readdirSync(MUSIC_DIR).filter(f => AUDIO_EXT.test(f));
    } catch (e) {
        return '';
    }

    let results;
    if (search && search.trim()) {
        results = files
            .map(f => {
                const display = f.replace(AUDIO_EXT, '').replace(/_/g, ' ');
                return { f, score: fuzzyScore(display, search) };
            })
            .filter(x => x.score > 0)
            .sort((a, b) => b.score - a.score || a.f.localeCompare(b.f))
            .map(x => x.f);
    } else {
        results = files.sort((a, b) => a.localeCompare(b));
    }

    return results
        .slice(0, MAX_RESULT)
        .map(f => {
            // Ten hien thi: khong dau, thay _ bang dau cach (SA-MP khong hien thi tieng Viet co dau)
            const display = f.replace(AUDIO_EXT, '').replace(/_/g, ' ');
            const name = normalize(display);
            return `${name}|${f}`;
        })
        .join('\n');
}

// Serve file am thanh, ho tro Range request
function serveAudio(req, res, rawName) {
    const safe = path.basename(decodeURIComponent(rawName));
    const abs  = path.join(MUSIC_DIR, safe);

    if (!abs.startsWith(MUSIC_DIR + path.sep) && abs !== MUSIC_DIR) {
        res.writeHead(403); return res.end('Forbidden');
    }

    fs.stat(abs, (err, stat) => {
        if (err) { res.writeHead(404); return res.end('Not Found'); }

        const ext  = path.extname(abs).slice(1).toLowerCase();
        const mime = { mp3: 'audio/mpeg', ogg: 'audio/ogg', wav: 'audio/wav' }[ext]
                  || 'application/octet-stream';

        const rangeHeader = req.headers.range;
        if (rangeHeader) {
            const [s, e] = rangeHeader.replace(/bytes=/, '').split('-');
            const start  = parseInt(s, 10);
            const end    = e ? parseInt(e, 10) : stat.size - 1;
            if (start >= stat.size || end >= stat.size || start > end) {
                res.writeHead(416, { 'Content-Range': `bytes */${stat.size}` });
                return res.end();
            }
            res.writeHead(206, {
                'Content-Type':   mime,
                'Content-Range':  `bytes ${start}-${end}/${stat.size}`,
                'Content-Length': end - start + 1,
                'Accept-Ranges':  'bytes',
            });
            fs.createReadStream(abs, { start, end }).pipe(res);
        } else {
            res.writeHead(200, {
                'Content-Type':   mime,
                'Content-Length': stat.size,
                'Accept-Ranges':  'bytes',
                'Cache-Control':  'public, max-age=3600',
            });
            fs.createReadStream(abs).pipe(res);
        }
    });
}

// ── HTTP Server ─────────────────────────────────────────────────────────────
http.createServer((req, res) => {
    const parsed   = url.parse(req.url, true);
    const pathname = parsed.pathname;
    const query    = parsed.query;

    res.setHeader('Access-Control-Allow-Origin', '*');

    if (pathname === '/api/music/list') {
        const list = getMusicList(null);
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end(list || 'Chua co bai hat|.');

    } else if (pathname === '/api/music/search') {
        const q = (query.q || '').trim();
        if (!q) { res.writeHead(400); return res.end('Thieu tham so q'); }
        const list = getMusicList(q);
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end(list || 'Khong tim thay|.');

    } else if (pathname.startsWith('/music/')) {
        serveAudio(req, res, pathname.slice(7));

    } else {
        res.writeHead(404); res.end('Not Found');
    }

}).listen(PORT, '0.0.0.0', () => {
    console.log(`[Music] Dang chay tren port ${PORT}`);
    console.log(`[Music] Thu muc nhac : ${MUSIC_DIR}`);
    console.log(`[Music] API list     : http://127.0.0.1:${PORT}/api/music/list`);
    console.log(`[Music] API search   : http://127.0.0.1:${PORT}/api/music/search?q=tenbaihot`);
    console.log(`[Music] Stream URL   : http://YOUR_VPS_IP:${PORT}/music/tenfile.mp3`);
    console.log('[Music] -- Them file .mp3 vao thu muc music/ la co ngay --');
});
