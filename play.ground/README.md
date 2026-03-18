# Xon Playground

Web playground for Xon with two execution modes:

- Browser mode: WASM parse + eval (no backend, fully standalone)
- Sandbox mode: optional backend eval API with process isolation and guardrails

## Quick Start

```bash
cd play.ground
npm install
npm run build
```

Run browser-only mode:

```bash
npm start
```

Run sandbox mode (optional, for isolated server-side eval):

```bash
npm run start:sandbox
```

Then open `http://localhost:8000`.

## Scripts

| Command | Description |
|---|---|
| `npm start` | Static playground via `http-server` |
| `npm run start:sandbox` | Playground + optional `/api/eval` sandbox backend |
| `npm run build` | Build `xon.js`/`xon.wasm` from C sources |
| `npm run dev` | Build + static start |
| `npm run dev:sandbox` | Build + sandbox start |
| `npm run verify` | Validate local toolchain/setup |

## Sandbox API

Endpoints:

- `GET /api/healthz`
- `POST /api/eval`

Request body:

```json
{
  "code": "{ value: (x, i) => x + i }",
  "options": {
    "maxCallDepth": 256,
    "timeoutMs": 600,
    "duplicateKeys": "error",
    "typeMode": "dynamic"
  }
}
```

Response shape:

- success: `{ ok: true, result, renderedXon, meta }`
- failure: `{ ok: false, error, meta }`

Current sandbox policy:

- Input body and source-size limits
- Per-request worker process
- Hard timeout kill
- Runtime option clamping (`maxCallDepth`, `maxEvalSteps`, etc.)
- `include()` and `import()` blocked in sandbox mode

## Files

- `index.html`: playground UI
- `xon-wrapper.js`: browser parser helpers
- `xon.js`, `xon.wasm`: generated WASM parser artifacts
- `service/server.js`: static + API server
- `service/eval_worker.js`: isolated eval worker

## Deployment Notes

- Static-only deploy (Netlify/GitHub Pages/Vercel): browser mode works independently.
- Sandbox mode is optional and needs a Node host with the native Xon module available.
- Core parser/runtime supports function aliases directly: `function`, `func`, `fun`, and `fx`.

## Requirements

- Node.js 18+
- Emscripten (to rebuild WASM artifacts)
- Xon native module is only required for `start:sandbox`
