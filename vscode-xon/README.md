# Xon Language Support (VS Code)

Adds syntax highlighting, validation, and formatting support for `.xon` files.

## Features

- Language registration for `.xon`
- Syntax highlighting for comments, keys, numbers, booleans, and strings
- `Xon: Validate Current File` command
- `Xon: Format Current File` command

## Requirements

- `xon` CLI available in your shell path

## Packaging

```bash
cd vscode-xon
npm install
npm run package
```

To publish:

```bash
npm run publish:vsce
```

## Development

1. Open this folder in VS Code.
2. Run `F5` to launch the Extension Development Host.
3. Open any `.xon` file and run the Xon commands from the command palette.
