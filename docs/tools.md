# Tools and Packages Used by Xon

This file lists the tools, runtimes, and packages used to build, test, and run this project.

## Core Build Toolchain

- `clang` or `gcc` (C99 compiler)
- `make` (optional, for Makefile workflow)
- `bash` (for `build.sh` and test scripts)
- `git` (source control)

## Parser Generator

- Lemon parser generator (bundled in repo)
  - `tools/lemon` (binary)
  - `tools/lemon.c` + `tools/lempar.c` (source/template)
  - Used to generate:
    - `src/xon.c`
    - `src/xon.h`
    - `src/xon.out`

## Language Runtimes

- Node.js (used for Node bindings and npm workflows)
- npm (package management/scripts)
- Python 3 (Python binding tests)

## Node / npm Packages

From root `package.json`:

- `node-addon-api` (dependency)
- `node-gyp` (dev dependency)

From `play.ground/package.json`:

- `http-server` (dev dependency)

From `vscode-xon/package.json`:

- `@vscode/vsce` (dev dependency, extension packaging/publish)

From `xon-language-server/package.json`:

- `vscode-languageserver`
- `vscode-languageserver-textdocument`

## WebAssembly / Playground Tooling

- Emscripten (`emcc`)
  - Required by `play.ground/build_wasm.sh`

## Build and Test Scripts

- `build.sh` - builds parser, shared library, CLI, and C example
- `scripts/run_tests.sh` - runs full C + Python tests
- `scripts/release_check.sh` - preflight checks before publishing
- `play.ground/verify.sh` - verifies playground prerequisites
- `play.ground/build_wasm.sh` - builds WASM artifacts
- `play.ground/setup_emscripten.sh` - installs/configures Emscripten

## CI / Release Automation

- GitHub Actions workflows:
  - `.github/workflows/ci.yml`
  - `.github/workflows/release.yml`
- Release secrets expected by workflows:
  - `NPM_TOKEN`
  - `PYPI_API_TOKEN`
  - `VSCE_PAT`

## Optional Developer Tools

- `nvm` (Node version manager; recommended)
- VS Code (for `vscode-xon/` extension development)
- `twine` and `build` (Python distribution build/publish tooling)
