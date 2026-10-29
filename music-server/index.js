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
    const watcher = fs.watch(MUSIC_DIR, { persistent: false }, () => {
        reloadFileCache();
    });
    watcher.on('error', (err) => {
        console.log('[Music] fs.watch error (cache se khong tu dong reload):', err.message);
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
  body { font-family: Arial, sans-serif; background: #0f0f1a; color: #ddd; display: flex; justify-content: center; padding: 32px 16px; min-height: 100vh; }
  .card { background: #1a1a2e; border: 1px solid #2a2a4e; border-radius: 8px; padding: 32px; width: 100%; max-width: 500px; height: fit-content; }
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
    transition: border-color .2s, background .2s;
  }
  .file-area:hover, .file-area.drag { border-color: #c084fc; }
  .file-area.has-file { border-color: #7c3aed; border-style: solid; background: rgba(124,58,237,0.08); }
  .file-area input { display: none; }
  .file-name { font-size: 13px; color: #555; margin-top: 6px; }
  .file-area.has-file .file-name { color: #c084fc; font-weight: 600; word-break: break-all; }
  .btn-upload {
    width: 100%; padding: 10px; background: #7c3aed; color: #fff;
    border: none; border-radius: 4px; font-size: 15px; cursor: pointer;
    transition: background .2s;
  }
  .btn-upload:hover { background: #6d28d9; }
  .btn-upload:disabled { background: #444; cursor: not-allowed; }
  #status { margin-top: 14px; padding: 10px 14px; border-radius: 4px; font-size: 14px; display: none; }
  .ok  { background: #14532d; color: #86efac; border: 1px solid #166534; }
  .err { background: #450a0a; color: #fca5a5; border: 1px solid #7f1d1d; }
  .progress { background: #1e3a5f; color: #93c5fd; border: 1px solid #1d4ed8; }
  .sep { border: none; border-top: 1px solid #2a2a4e; margin: 20px 0; }
  .list-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; }
  .list-header span { font-size: 13px; color: #aaa; }
  .list-count { font-size: 12px; color: #666; }
  .song-list { max-height: 340px; overflow-y: auto; border: 1px solid #2a2a4e; border-radius: 6px; }
  .song-list::-webkit-scrollbar { width: 6px; }
  .song-list::-webkit-scrollbar-track { background: #0f0f1a; }
  .song-list::-webkit-scrollbar-thumb { background: #3a3a5e; border-radius: 3px; }
  .song-item { display: flex; align-items: center; padding: 7px 12px; border-bottom: 1px solid #1e1e38; gap: 8px; }
  .song-item:last-child { border-bottom: none; }
  .song-item:hover { background: #1e1e38; }
  .song-num { font-size: 11px; color: #555; min-width: 22px; text-align: right; }
  .song-name { flex: 1; font-size: 13px; color: #ccc; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
  .btn-del { padding: 3px 10px; background: #7f1d1d; color: #fca5a5; border: 1px solid #991b1b; border-radius: 3px; font-size: 12px; cursor: pointer; transition: background .15s; flex-shrink: 0; }
  .btn-del:hover { background: #991b1b; }
  .btn-del:disabled { opacity: 0.4; cursor: not-allowed; }
  .list-empty { padding: 20px; text-align: center; color: #555; font-size: 13px; }
  .progress-bar-wrap { background: rgba(0,0,0,0.25); border-radius: 3px; height: 8px; margin-top: 9px; overflow: hidden; }
  .progress-bar-fill { height: 100%; background: #60a5fa; border-radius: 3px; transition: width 0.25s ease; width: 0; }
  .progress-pct { float: right; font-size: 12px; opacity: 0.85; }
  @keyframes song-flash { 0%,25%{ background:#14532d; } 100%{ background:transparent; } }
  .song-item.song-new { animation: song-flash 3s ease-out forwards; }
  .song-item.song-new .song-name { color: #86efac; }
  .file-list { list-style: none; margin: 5px 0 0; padding: 0; text-align: left; }
  .file-list li { font-size: 11px; color: #c084fc; padding: 1px 0; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
</style>
</head>
<body>
<div class="card">
  <h1>KHU PHO 5 Music Admin</h1>
  <label>Username</label>
  <input type="text" id="user" placeholder="admin" autocomplete="username" />
  <label>Password</label>
  <input type="password" id="pass" placeholder="&#x2022;&#x2022;&#x2022;&#x2022;&#x2022;&#x2022;&#x2022;&#x2022;" autocomplete="current-password" />
  <hr class="sep">
  <label>File nhac (.mp3 / .ogg / .wav, toi da 100MB)</label>
  <div class="file-area" id="dropZone">
    <div>Keo tha file vao day hoac <u>bam de chon</u></div>
    <div class="file-name" id="fileName">Chua chon file</div>
    <input type="file" id="file" accept=".mp3,.ogg,.wav" multiple />
  </div>
  <button class="btn-upload" id="btn">Upload</button>
  <div id="status"></div>
  <hr class="sep">
  <div class="list-header">
    <span>Danh sach bai hat</span>
    <span class="list-count" id="listCount"></span>
  </div>
  <div class="song-list" id="songList">
    <div class="list-empty">Dang tai...</div>
  </div>
</div>
<script>
  var dropZone  = document.getElementById('dropZone');
  var fileInput = document.getElementById('file');
  var fileNameEl= document.getElementById('fileName');
  var btn       = document.getElementById('btn');
  var statusEl  = document.getElementById('status');
  var songList  = document.getElementById('songList');
  var listCount = document.getElementById('listCount');
  var selectedFiles = [];

  dropZone.addEventListener('click', function() { fileInput.click(); });
  dropZone.addEventListener('dragover', function(e) { e.preventDefault(); dropZone.classList.add('drag'); });
  dropZone.addEventListener('dragleave', function() { dropZone.classList.remove('drag'); });
  dropZone.addEventListener('drop', function(e) {
    e.preventDefault();
    dropZone.classList.remove('drag');
    if (e.dataTransfer.files.length) setFiles(e.dataTransfer.files);
  });
  fileInput.addEventListener('change', function() { if (fileInput.files.length) setFiles(fileInput.files); });

  function escHtml(s) {
    return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;').replace(/'/g,'&#39;');
  }

  function setFiles(fileList) {
    selectedFiles = Array.from(fileList).filter(function(f) { return /\.(mp3|ogg|wav)$/i.test(f.name); });
    if (selectedFiles.length === 0) {
      fileNameEl.innerHTML = 'Chua chon file';
      dropZone.classList.remove('has-file');
      return;
    }
    dropZone.classList.add('has-file');
    if (selectedFiles.length === 1) {
      fileNameEl.innerHTML = escHtml(selectedFiles[0].name) +
        ' <span style="opacity:0.7">(' + (selectedFiles[0].size / 1024 / 1024).toFixed(1) + ' MB)</span>';
    } else {
      var totalMB = selectedFiles.reduce(function(s, f) { return s + f.size; }, 0) / 1024 / 1024;
      var listHtml = '<ul class="file-list">';
      var show = Math.min(selectedFiles.length, 5);
      for (var i = 0; i < show; i++) {
        listHtml += '<li>' + escHtml(selectedFiles[i].name) + '</li>';
      }
      if (selectedFiles.length > 5) {
        listHtml += '<li style="opacity:0.6">...va ' + (selectedFiles.length - 5) + ' file nua</li>';
      }
      listHtml += '</ul>';
      fileNameEl.innerHTML = '<b>' + selectedFiles.length + ' files</b> (' + totalMB.toFixed(1) + ' MB tong)' + listHtml;
    }
  }

  function clearFiles() {
    selectedFiles = [];
    try { fileInput.value = ''; } catch(e) {}
    fileNameEl.innerHTML = 'Chua chon file';
    dropZone.classList.remove('has-file');
  }

  function showStatus(msg, type) {
    statusEl.style.display = 'block';
    statusEl.className = type;
    statusEl.textContent = msg;
  }

  function showProgress(pct, cur, total, name) {
    statusEl.style.display = 'block';
    statusEl.className = 'progress';
    var label = total > 1 ? '[' + cur + '/' + total + '] Dang upload...' : 'Dang upload...';
    var sub = total > 1 ? '<div style="font-size:11px;opacity:0.75;margin-top:3px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap">' + escHtml(name) + '</div>' : '';
    statusEl.innerHTML =
      label + ' <span class="progress-pct">' + pct + '%</span>' + sub +
      '<div class="progress-bar-wrap"><div class="progress-bar-fill" id="pbar" style="width:' + pct + '%"></div></div>';
  }

  function updateProgress(pct) {
    var pbar = document.getElementById('pbar');
    var pcts = statusEl.querySelector('.progress-pct');
    if (pcts) pcts.textContent = pct + '%';
    if (pbar) pbar.style.width = pct + '%';
  }

  function loadSongList(highlightFile) {
    fetch('/api/music/files').then(function(r) { return r.text(); }).then(function(text) {
      text = text.trim();
      if (!text) {
        songList.innerHTML = '<div class="list-empty">Chua co bai hat nao</div>';
        listCount.textContent = '(0 bai)';
        return;
      }
      var lines = text.split('\\n').filter(Boolean);
      listCount.textContent = '(' + lines.length + ' bai)';
      var html = '';
      for (var i = 0; i < lines.length; i++) {
        var sep = lines[i].indexOf('|');
        var display  = sep >= 0 ? lines[i].slice(0, sep) : lines[i];
        var filename = sep >= 0 ? lines[i].slice(sep + 1) : lines[i];
        var safe = encodeURIComponent(filename);
        html += '<div class="song-item">' +
          '<span class="song-num">' + (i + 1) + '</span>' +
          '<span class="song-name" title="' + escHtml(filename) + '">' + escHtml(display) + '</span>' +
          '<button class="btn-del" onclick="deleteSong(this,\\'' + safe + '\\')">Xoa</button>' +
          '</div>';
      }
      songList.innerHTML = html;
      if (highlightFile) {
        var hlSet = {};
        var hlArr = Array.isArray(highlightFile) ? highlightFile : [highlightFile];
        for (var k = 0; k < hlArr.length; k++) hlSet[hlArr[k]] = true;
        var items = songList.querySelectorAll('.song-item');
        var firstNew = null;
        for (var j = 0; j < items.length; j++) {
          var nameEl = items[j].querySelector('.song-name');
          if (nameEl && hlSet[nameEl.getAttribute('title')]) {
            items[j].classList.add('song-new');
            if (!firstNew) firstNew = items[j];
          }
        }
        if (firstNew) firstNew.scrollIntoView({ block: 'nearest' });
      }
    }).catch(function() {
      songList.innerHTML = '<div class="list-empty">Loi tai danh sach</div>';
    });
  }

  function deleteSong(btnEl, encodedFile) {
    var user = document.getElementById('user').value.trim();
    var pass = document.getElementById('pass').value;
    if (!user || !pass) { showStatus('Nhap username va password de xoa!', 'err'); return; }
    var filename = decodeURIComponent(encodedFile);
    if (!confirm('Xoa bai: ' + filename + '?')) return;
    btnEl.disabled = true;
    fetch('/admin/delete?name=' + encodedFile, {
      method: 'POST',
      headers: { 'Authorization': 'Basic ' + btoa(user + ':' + pass) }
    }).then(function(r) {
      return r.text().then(function(text) { return { ok: r.ok, text: text }; });
    }).then(function(r) {
      showStatus(r.text, r.ok ? 'ok' : 'err');
      if (r.ok) loadSongList();
      else btnEl.disabled = false;
    }).catch(function(e) {
      showStatus('Loi ket noi: ' + e.message, 'err');
      btnEl.disabled = false;
    });
  }

  function doUpload(files, idx, user, pass, results) {
    if (idx >= files.length) {
      var ok = results.filter(function(r) { return r.ok; }).length;
      var total = results.length;
      if (ok === total) {
        showStatus('Upload xong ' + ok + '/' + total + ' file thanh cong!', 'ok');
      } else {
        showStatus('Xong: ' + ok + '/' + total + ' thanh cong, ' + (total - ok) + ' that bai.', 'err');
      }
      var uploaded = results.filter(function(r) { return r.ok; }).map(function(r) { return r.name; });
      loadSongList(uploaded.length ? uploaded : null);
      clearFiles();
      btn.disabled = false;
      return;
    }

    var file = files[idx];
    showProgress(0, idx + 1, files.length, file.name);

    var xhr = new XMLHttpRequest();

    xhr.upload.addEventListener('progress', function(e) {
      if (e.lengthComputable) {
        updateProgress(Math.round(e.loaded / e.total * 100));
      }
    });

    xhr.addEventListener('load', function() {
      results.push({ ok: xhr.status >= 200 && xhr.status < 300, name: file.name });
      doUpload(files, idx + 1, user, pass, results);
    });

    xhr.addEventListener('error', function() {
      results.push({ ok: false, name: file.name });
      doUpload(files, idx + 1, user, pass, results);
    });

    xhr.addEventListener('abort', function() {
      results.push({ ok: false, name: file.name });
      doUpload(files, idx + 1, user, pass, results);
    });

    var fd = new FormData();
    fd.append('music', file);
    xhr.open('POST', '/admin/upload');
    xhr.setRequestHeader('Authorization', 'Basic ' + btoa(user + ':' + pass));
    xhr.send(fd);
  }

  btn.addEventListener('click', function() {
    var user = document.getElementById('user').value.trim();
    var pass = document.getElementById('pass').value;

    if (!user || !pass) { showStatus('Nhap username va password!', 'err'); return; }
    if (selectedFiles.length === 0) { showStatus('Chon file nhac truoc!', 'err'); return; }
    var oversized = selectedFiles.filter(function(f) { return f.size > 100 * 1024 * 1024; });
    if (oversized.length) { showStatus('"' + oversized[0].name + '" qua lon (toi da 100MB)!', 'err'); return; }

    btn.disabled = true;
    doUpload(selectedFiles.slice(), 0, user, pass, []);
  });

  loadSongList();
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
    let wsFinished   = false;
    let bbFinished   = false;

    function tryReplyOk() {
        if (wsFinished && bbFinished) reply(200, 'Upload thanh cong!');
    }

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
            fileStream.resume();
            reply(413, 'File qua lon (toi da 100MB)');
        });

        ws.on('error', err => {
            reply(500, 'Loi luu file: ' + err.message);
        });

        ws.on('finish', () => {
            wsFinished = true;
            tryReplyOk();
        });
    });

    bb.on('finish', () => {
        if (!fileReceived) return reply(400, 'Khong co file nao duoc gui len');
        bbFinished = true;
        tryReplyOk();
    });

    bb.on('error', err => {
        reply(500, 'Loi xu ly upload: ' + err.message);
    });

    req.pipe(bb);
}

// ── Delete handler ───────────────────────────────────────────────────────────
function handleDelete(req, res, query) {
    if (!checkAuth(req)) {
        res.writeHead(401, { 'Content-Type': 'text/plain; charset=utf-8' });
        return res.end('Sai username hoac password');
    }

    let rawName = query.name || '';
    try { rawName = decodeURIComponent(rawName); } catch(e) {}
    const filename = path.basename(rawName);

    if (!filename || !AUDIO_EXT.test(filename)) {
        res.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
        return res.end('Ten file khong hop le');
    }

    const abs = path.join(MUSIC_DIR, filename);
    if (!abs.startsWith(MUSIC_DIR + path.sep)) {
        res.writeHead(403, { 'Content-Type': 'text/plain; charset=utf-8' });
        return res.end('Forbidden');
    }

    fs.unlink(abs, (err) => {
        if (err) {
            const code = err.code === 'ENOENT' ? 404 : 500;
            const msg  = err.code === 'ENOENT' ? 'Khong tim thay file' : 'Loi xoa file: ' + err.message;
            res.writeHead(code, { 'Content-Type': 'text/plain; charset=utf-8' });
            return res.end(msg);
        }
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end('Da xoa: ' + filename);
    });
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

    // POST /admin/delete?name=<filename> - xoa nhac (can auth)
    } else if (pathname === '/admin/delete' && req.method === 'POST') {
        return handleDelete(req, res, query);

    // GET /api/music/files - danh sach file thuc (display|filename)
    } else if (pathname === '/api/music/files') {
        const files = getSortedFiles();
        const list = files.map(f => {
            const display = f.replace(AUDIO_EXT, '').replace(/_/g, ' ');
            return display + '|' + f;
        }).join('\n');
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
        return res.end(list);

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
