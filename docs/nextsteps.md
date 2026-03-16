# Next Steps (Deployment and Rollout)

This is the practical sequence to ship Xon after the current implementation pass.

## 1. Stabilize and Freeze

- Run full test pass: `./scripts/run_tests.sh`
- Run release readiness checks: `./scripts/release_check.sh`
- Verify CLI smoke tests:
  - `./xon validate tests/test.xon`
  - `./xon format tests/test.xon`
  - `./xon convert tests/test.xon /tmp/test.json`
- Confirm logging output is created under `logs/`.

## 2. CI/CD Setup

- Add GitHub Actions workflow for:
  - parser generation check (`./tools/lemon src/xon.lemon`)
  - C build + tests
  - Python binding test
  - Node binding build/test (when Node toolchain is available)
- Publish build artifacts for each tagged release.

## 3. Package and Release

- npm:
  - verify package metadata and `bin/xon`
  - run `npm pack --dry-run` locally
  - publish `@xerxisfy/xon`
- npm (LSP):
  - publish `@xerxisfy/xon-language-server`
- Python:
  - build wheel: `python -m build --wheel bindings/python --outdir dist-python`
  - verify: `python -m twine check dist-python/*`
  - publish `xerxisfy-xon` to PyPI
- Tag release in git (`v1.x.y`) and attach changelog.

## 4. Playground Deployment

- Build WASM: `cd play.ground && ./build_wasm.sh`
- Deploy static playground to one target:
  - Netlify
  - GitHub Pages
  - Vercel
- Validate parser + converter features in production URL.

## 5. Editor Tooling Rollout

- Finish and package `vscode-xon/` extension.
- Publish to VS Code Marketplace.
- Add docs for install + command usage (`xon.validate`, `xon.format`).

## 5.1 CI Secrets Needed

Configure these repository secrets for automated publishing:

- `NPM_TOKEN` for npm package publishes (`@xerxisfy/xon` and `@xerxisfy/xon-language-server`)
- `PYPI_API_TOKEN` for PyPI publish
- `VSCE_PAT` for VS Code Marketplace publish

## 6. Observability and Operations

- Define log retention policy for `logs/`.
- Add log rotation or size cap mechanism (future enhancement).
- Standardize bug report template to include:
  - CLI command
  - input file
  - relevant log lines

## 7. Documentation Completion

- Keep `README.md` in sync with:
  - CLI commands
  - logging behavior
  - bindings status
- Keep `docs/tools.md` updated when dependencies change.
- Keep roadmap updates in `docs/roadMap.md`.











<!-- NEXT STEPS TO MAKING THE LANGUAGE LIVE ANd USABLE -->
Done. I implemented the remaining release work across npm, PyPI, VS Code, and other IDE support.

Key deliverables are in:

npm hardening: package.json, .gitignore, scripts/release_check.sh
Python publish packaging: bindings/python/pyproject.toml, bindings/python/setup.py, bindings/python/README.md, bindings/python/MANIFEST.in, bindings/python/xon.py
VS Code marketplace readiness: vscode-xon/package.json, vscode-xon/CHANGELOG.md, vscode-xon/.vscodeignore, vscode-xon/README.md
Other IDEs (LSP): xon-language-server/package.json, xon-language-server/src/server.js, xon-language-server/README.md
CI/release automation: .github/workflows/ci.yml, .github/workflows/release.yml
Docs sync: README.md, docs/tools.md, docs/nextsteps.md, docs/scripts.run.md
Validation done locally:

./build.sh passes
./scripts/run_tests.sh passes
./scripts/release_check.sh passes (with environment-based skips if npm/build module are unavailable)
What you need to do now before actual publish:

Set repo secrets: NPM_TOKEN, PYPI_API_TOKEN, VSCE_PAT.
Run local preflight: ./scripts/release_check.sh.
Publish manually or tag vX.Y.Z to trigger .github/workflows/release.yml.
If you want, I can now prepare the exact git commit sequence and release tag commands for v1.0.0.

