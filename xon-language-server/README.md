# Xon Language Server

Language Server Protocol (LSP) server for Xon.

## Features

- Live validation diagnostics via `xon validate`
- Document formatting via `xon format`

## Requirements

- Node.js 18+
- `xon` CLI available in PATH

## Install

```bash
cd xon-language-server
npm install
npm start
```

Or install globally:

```bash
npm install -g @xerxisfy/xon-language-server
```

## Editor Integration

### Neovim (`nvim-lspconfig`)

```lua
require('lspconfig').xon_language_server.setup {
  cmd = { "xon-language-server" }
}
```

### Emacs (`lsp-mode`)

```elisp
(add-to-list 'lsp-language-id-configuration '(xon-mode . "xon"))
(lsp-register-client
 (make-lsp-client :new-connection (lsp-stdio-connection '("xon-language-server"))
                  :major-modes '(xon-mode)
                  :server-id 'xon-ls))
```
