# Xon Playground Deploy Instructions

## 1) Build Assets

```bash
cd ~/xon/play.ground
npm install
npm run build
```

## 2) Choose Runtime Mode

### A. Static-only (browser WASM parse + eval)

Deploy only `play.ground/` files to Netlify/Vercel/GitHub Pages.

UI mode to use: `Settings -> Parser -> Execution mode -> Browser (WASM parse+eval)`.

### B. Optional sandbox runtime (API)

Run the sandbox server:

```bash
cd ~/xon/play.ground
npm run start:sandbox
```

This serves:

- UI at `http://localhost:8000`
- API at `http://localhost:8000/api/eval`

UI mode to use: `Settings -> Parser -> Execution mode -> Sandbox API (eval)`.

## 3) Health Check

```bash
curl -s http://localhost:8000/api/healthz
```

Expect `{ "ok": true, ... }`.

## 4) Eval API Smoke Test

```bash
curl -s http://localhost:8000/api/eval \
  -H 'content-type: application/json' \
  --data '{"code":"{ total: max(1, 2, 3) }"}'
```

Expect `{ "ok": true, "result": ... }`.
