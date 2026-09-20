import http from 'node:http';

const PORT = 3000;
const HOST = '0.0.0.0';

const server = http.createServer((req, res) => {
  res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8' });
  res.end('Hyper Editor Agent: Pure C++ Headless Video & Audio Editor Engine.\nNo UI or Web Components.\nCLI executable: ./build/hyper_editor\n');
});

server.listen(PORT, HOST, () => {
  console.log(`Hyper Editor Headless Service active on http://${HOST}:${PORT}`);
});
