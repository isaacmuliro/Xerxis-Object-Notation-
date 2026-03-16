# Script Run Quick Commands

From repo root (`/Users/xerxiscmuliro/xon`):

```bash
./build.sh
./scripts/run_tests.sh
./scripts/release_check.sh
```

CLI smoke test:

```bash
./xon validate tests/test.xon
./xon format tests/test.xon
./xon convert tests/test.xon /tmp/test_out.json
./xon convert /tmp/test_out.json /tmp/test_roundtrip.xon
./xon parse examples/config.xon
```

Playground:

```bash
cd play.ground
./verify.sh
./build_wasm.sh
python3 -m http.server 8000
```

Then open: `http://localhost:8000`
