'use strict';

const http   = require('http');
const fs     = require('fs');
const path   = require('path');
const url    = require('url');
const Busboy = require('busboy');

// ── Config ─────────────────────────────────────────────────────────────────
const cfgPath = path.join(__dirname, 'config.json');
const cfg = Object.assign(
    { port: 7779, musicDir: './music' },
    fs.existsSync(cfgPath) ? JSON.parse(fs.readFileSync(cfgPath, 'utf8')) : {}
);

const MUSIC_DIR   = path.resolve(__dirname, cfg.musicDir);
const PORT        = cfg.port;
const AUDIO_EXT   = /\.(mp3|ogg|wav)$/i;
const MAX_RESULT  = 30;
const MAX_DISPLAY = 60;
const MAX_UPLOAD  = 100 * 1024 * 1024; // 100MB

if (!fs.existsSync(MUSIC_DIR)) {
    fs.mkdirSync(MUSIC_DIR, { recursive: true });
    console.log(`[Music] Da tao thu muc nhac: ${MUSIC_DIR}`);
}

// ── File cache ──────────────────────────────────────────────────────────────
let cachedFiles = null;

function reloadFileCache() {
    try {
        cachedFiles = fs.readdirSync(MUSIC_DIR)
            .filter(f => AUDIO_EXT.test(f))
            .sort((a, b) => a.localeCompare(b));
    } catch(e) {
        cachedFiles = [];
    }
    console.log(`[Music] Cache reload: ${cachedFiles.length} file(s)`);
}

reloadFileCache();

try {
    fs.watch(MUSIC_DIR, { persistent: false }, () => {
        reloadFileCache();
    });
} catch(e) {
    console.log('[Music] fs.watch khong kha dung, cache chi reload khi khoi dong');
}

function getSortedFiles() {
    return cachedFiles || [];
}

// ── Helpers ─────────────────────────────────────────────────────────────────

function stripDiacritics(str) {
    return str
        .normalize('NFD')
        .replace(/[̀-ͯ]/g, '')
        .replace(/đ/g, 'd').replace(/Đ/g, 'D');
}

function normalize(str) {
    return stripDiacritics(str).toLowerCase().trim();
}

function fuzzyScore(displayName, query) {
    const t = normalize(displayName);
    const q = normalize(query);
    if (!q) return 1;

    if (t === q) return 100;
    if (q.length >= 3) {
        if (t.startsWith(q)) return 85;
        if (t.includes(q))   return 65;
    }

    const qWords = q.split(/[^a-z0-9]+/).filter(Boolean);
    const tWords = t.split(/[^a-z0-9]+/).filter(Boolean);

    if (qWords.length === 0) return 0;

    let matched = 0;
    for (const qw of qWords) {
        if (qw.length <= 2) {
            if (tWords.includes(qw)) matched++;
        } else {
            if (tWords.some(tw => tw === qw || tw.startsWith(qw))) matched++;
        }
    }

    if (matched === 0) return 0;
    if (matched === qWords.length) {
        return qWords.length > 1 ? 55 : 35;
    }
    return Math.floor(20 + (matched / qWords.length) * 30);
}

function getMusicList(search) {
    const files = getSortedFiles();
    if (files.length === 0) return '';

    let results;
    if (search && search.trim()) {
        results = files
            .map((f, idx) => {
                const display = f.replace(AUDIO_EXT, '').replace(/_/g, ' ');
                return { idx, score: fuzzyScore(display, search) };
            })
            .filter(x => x.score > 0)
            .sort((a, b) => b.score - a.score || a.idx - b.idx);
    } else {
        results = files.map((_, idx) => ({ idx }));
    }

    return results
        .slice(0, MAX_RESULT)
        .map(({ idx }) => {
            const f = files[idx];
            const display = f.replace(AUDIO_EXT, '').replace(/_/g, ' ');
            let name = stripDiacritics(display);
            name = name.replace(/[|\n\r]/g, ' ');
            if (name.length > MAX_DISPLAY) name = name.slice(0, MAX_DISPLAY - 1) + '~';
            return `${name}|${idx}`;
        })
        .join('\n');
}

function serveByIndex(req, res, n) {
    const files = getSortedFiles();
    if (isNaN(n) || n < 0 || n >= files.length) {
        res.writeHead(404); return res.end('Not Found');
    }
    serveFile(req, res, files[n]);
}

function serveAudio(req, res, rawName) {
    let filename;
    try { filename = decodeURIComponent(rawName); } catch(e) { filename = rawName; }
    serveFile(req, res, path.basename(filename));
}

function serveFile(req, res, filename) {
    const abs = path.join(MUSIC_DIR, filename);

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
            const parts = rangeHeader.replace(/bytes=/, '').split('-');
            const start = parseInt(parts[0], 10);
            const end   = parts[1] ? parseInt(parts[1], 10) : stat.size - 1;
            if (isNaN(start) || isNaN(end) || start < 0 || start >= stat.size || end >= stat.size || start > end) {
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

// ── Auth ─────────────────────────────────────────────────────────────────────
function checkAuth(req) {
    const h = req.headers['authorization'];
    if (!h || !h.startsWith('Basic ')) return false;
    const decoded = Buffer.from(h.slice(6), 'base64').toString('utf8');
    const colon   = decoded.indexOf(':');
    if (colon < 0) return false;
    const user = decoded.slice(0, colon);
    const pass = decoded.slice(colon + 1);
    return user === cfg.admin?.username && pass === cfg.admin?.password;
}

// ── Admin HTML ────────────────────────────────────────────────────────────────
const ADMIN_HTML = `<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>KHU PHO 5 Music Admin</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body { font-family: Arial, sans-serif; background: #0f0f1a; color: #ddd; display: flex; justify-content: center; align-items: center; min-height: 100vh; }
  .card { background: #1a1a2e; border: 1px solid #2a2a4e; border-radius: 8px; padding: 32px; width: 100%; max-width: 440px; }
  h1 { text-align: center; color: #c084fc; margin-bottom: 24px; font-size: 20px; letter-spacing: 1px; }
  label { display: block; font-size: 13px; color: #aaa; margin-bottom: 4px; }
  input[type=text], input[type=password] {
    display: block; width: 100%; padding: 9px 12px;
    background: #0f0f1a; color: #eee; border: 1px solid #333;
    border-radius: 4px; font-size: 14px; margin-bottom: 14px;
  }
  input[type=text]:focus, input[type=password]:focus { outline: none; border-color: #c084fc; }
  .file-area {
    border: 2px dashed #333; border-radius: 6px; padding: 20px;
    text-align: center; cursor: pointer; margin-bottom: 16px;
    transition: border-color .2s;
  }
  .file-area:hover, .file-area.drag { border-color: #c084fc; }
  .file-area input { display: none; }
  .file-name { font-size: 13px; color: #aaa; margin-top: 6px; }
  button {
    width: 100%; padding: 10px; background: #7c3aed; color: #fff;
    border: none; border-radius: 4px; font-size: 15px; cursor: pointer;
    transition: background .2s;
  }
  button:hover { background: #6d28d9; }
  button:disabled { background: #444; cursor: not-allowed; }
  #status { margin-top: 14px; padding: 10px 14px; border-radius: 4px; font-size: 14px; display: none; }
  .ok  { background: #14532d; color: #86efac; border: 1px solid #166534; }
  .err { background: #450a0a; color: #fca5a5; border: 1px solid #7f1d1d; }
  .progress { background: #1e3a5f; color: #93c5fd; border: 1px solid #1d4ed8; }
  .sep { border: none; border-top: 1px solid #2a2a4e; margin: 20px 0; }
</style>
</head>
<body>
<div class="card">
  <h1>KHU PHO 5 Music Admin</h1>
  <label>Username</label>
  <input type="text" id="user" placeholder="admin" autocomplete="username" />
  <label>Password</label>
  <input type="password" id="pass" placeholder="••••••••" autocomplete="current-password" />
  <hr class="sep">
  <label>File nhac (.mp3 / .ogg / .wav, toi da 100MB)</label>
  <div class="file-area" id="dropZone">
    <div>Keo tha file vao day hoac <u>bam de chon</u></div>
    <div class="file-name" id="fileName">Chua chon file</div>
    <input type="file" id="file" accept=".mp3,.ogg,.wav" />
  </div>
  <button id="btn">Upload</button>
  <div id="status"></div>
</div>
<script>
  const dropZone = document.getElementById('dropZone');
  const fileInput = document.getElementById('file');
  const fileNameEl = document.getElementById('fileName');
  const btn = document.getElementById('btn');
  const status = document.getElementById('status');

  dropZone.addEventListener('click', () => fileInput.click());
  dropZone.addEventListener('dragover', e => { e.preventDefault(); dropZone.classList.add('drag'); });
  dropZone.addEventListener('dragleave', () => dropZone.classList.remove('drag'));
  dropZone.addEventListener('drop', e => {
    e.preventDefault();
    dropZone.classList.remove('drag');
    if (e.dataTransfer.files[0]) setFile(e.dataTransfer.files[0]);
  });
  fileInput.addEventListener('change', () => { if (fileInput.files[0]) setFile(fileInput.files[0]); });

  function setFile(f) {
    fileInput._selected = f;
    fileNameEl.textContent = f.name + ' (' + (f.size / 1024 / 1024).toFixed(1) + ' MB)';
  }

  function showStatus(msg, type) {
    status.style.display = 'block';
    status.className = type;
    status.textContent = msg;
  }

  btn.addEventListener('click', async () => {
    const user = document.getElementById('user').value.trim();
    const pass = document.getElementById('pass').value;
    const file = fileInput._selected || fileInput.files[0];

    if (!user || !pass) { showStatus('Nhap username va password!', 'err'); return; }
    if (!file) { showStatus('Chon file nhac truoc!', 'err'); return; }
    if (file.size > 100 * 1024 * 1024) { showStatus('File qua lon (toi da 100MB)!', 'err'); return; }

    const formData = new FormData();
    formData.append('music', file);

    btn.disabled = true;
    showStatus('Dang upload, vui long cho...', 'progress');

    try {
      const res = await fetch('/admin/upload', {
        method: 'POST',
        headers: { 'Authorization': 'Basic ' + btoa(user + ':' + pass) },
        body: formData
      });
      const text = await res.text();
      showStatus(text, res.ok ? 'ok' : 'err');
    } catch(err) {
      showStatus('Loi ket noi: ' + err.message, 'err');
    } finally {
      btn.disabled = false;
    }
  });
</script>
</body>
</html>`;

// ── Upload handler ────────────────────────────────────────────────────────────
function handleUpload(req, res) {
    if (!checkAuth(req)) {
        res.writeHead(401, {
            'WWW-Authenticate': 'Basic realm="KHU PHO 5 Music Admin"',
            'Content-Type': 'text/plain; charset=utf-8'
        });
        return res.end('Sai username hoac password');
    }

    const contentType = req.headers['content-type'] || '';
    if (!contentType.includes('multipart/form-data')) {
        res.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
        return res.end('Content-Type phai la multipart/form-data');
    }

    let responded = false;
    function reply(code, msg) {
        if (responded) return;
        responded = true;
        res.writeHead(code, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end(msg);
    }

    let fileReceived = false;

    const bb = Busboy({ headers: req.headers, limits: { fileSize: MAX_UPLOAD, files: 1 } });

    bb.on('file', (_field, fileStream, info) => {
        let rawName = info.filename || '';
        // Busboy sometimes decodes UTF-8 filenames as Latin-1 — reinterpret bytes
        if (/[\x80-\xFF]/.test(rawName)) {
            try { rawName = Buffer.from(rawName, 'latin1').toString('utf8'); } catch(e) {}
        }
        const filename = path.basename(rawName);

        if (!AUDIO_EXT.test(filename)) {
            fileStream.resume();
            return reply(400, 'Chi chap nhan file .mp3, .ogg hoac .wav');
        }

        const dest = path.join(MUSIC_DIR, filename);
        if (!dest.startsWith(MUSIC_DIR + path.sep)) {
            fileStream.resume();
            return reply(403, 'Forbidden');
        }

        fileReceived = true;
        const ws = fs.createWriteStream(dest);
        fileStream.pipe(ws);

        fileStream.on('limit', () => {
            ws.destroy();
            fs.unlink(dest, () => {});
            reply(413, 'File qua lon (toi da 100MB)');
        });

        ws.on('error', err => {
            reply(500, 'Loi luu file: ' + err.message);
        });
    });

    bb.on('finish', () => {
        if (!fileReceived) return reply(400, 'Khong co file nao duoc gui len');
        reply(200, 'Upload thanh cong!');
    });

    bb.on('error', err => {
        reply(500, 'Loi xu ly upload: ' + err.message);
    });

    req.pipe(bb);
}

// ── HTTP Server ─────────────────────────────────────────────────────────────
http.createServer((req, res) => {
    const parsed   = url.parse(req.url, true);
    const pathname = parsed.pathname;
    const query    = parsed.query;

    res.setHeader('Access-Control-Allow-Origin', '*');

    // GET /admin - trang upload
    if (pathname === '/admin' && req.method === 'GET') {
        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        return res.end(ADMIN_HTML);

    // POST /admin/upload - upload nhac (can auth)
    } else if (pathname === '/admin/upload' && req.method === 'POST') {
        return handleUpload(req, res);

    // GET /api/music/list
    } else if (pathname === '/api/music/list') {
        const list = getMusicList(null);
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end(list || 'Chua co bai hat|.');

    // GET /api/music/search?q=...
    } else if (pathname === '/api/music/search') {
        const q = (query.q || '').trim();
        if (!q) { res.writeHead(400); return res.end('Thieu tham so q'); }
        const list = getMusicList(q);
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end(list || 'Khong tim thay|.');

    // GET /music/idx/:n
    } else if (pathname.startsWith('/music/idx/')) {
        const n = parseInt(pathname.slice(11), 10);
        serveByIndex(req, res, n);

    // GET /music/<filename>
    } else if (pathname.startsWith('/music/')) {
        serveAudio(req, res, pathname.slice(7));

    } else {
        res.writeHead(404); res.end('Not Found');
    }

}).listen(PORT, '0.0.0.0', () => {
    console.log(`[Music] Dang chay tren port ${PORT}`);
    console.log(`[Music] Thu muc nhac : ${MUSIC_DIR}`);
    console.log(`[Music] Admin UI     : http://51.79.165.177:${PORT}/admin`);
    console.log(`[Music] API list     : http://51.79.165.177:${PORT}/api/music/list`);
    console.log(`[Music] API search   : http://51.79.165.177:${PORT}/api/music/search?q=tenbaihot`);
    console.log(`[Music] Stream URL   : http://51.79.165.177:${PORT}/music/idx/0`);
    console.log('[Music] -- Them file .mp3 vao thu muc music/ la co ngay --');
});
