# Contributing to Xon

Thank you for your interest in contributing to Xon!

## How to Contribute

### Reporting Bugs
- Use GitHub Issues
- Include steps to reproduce
- Provide sample `.xon` files
- Mention your OS and compiler version

### Suggesting Features
- Open a GitHub Discussion or Issue
- Describe the use case
- Provide syntax examples

### Pull Requests
1. Fork the repository
2. Create a feature branch
3. Make your changes in `src/`
4. Regenerate parser: `./tools/lemon src/xon.lemon`
5. Test: `make test`
6. Commit with clear messages
7. Push and open a PR

## Code Style
- 4 spaces for indentation
- Follow existing conventions
- Add comments for complex logic
- Update docs for new features

## Development Setup

```bash
git clone https://github.com/yourusername/xon.git
cd xon
make
make test
```

## Questions?
Open a GitHub Discussion or contact the maintainers.
