# Xerxisfy Xon (Python)

Python bindings for Xon (Xerxis Object Notation) using `ctypes`.

## Install

```bash
pip install xerxisfy-xon
```

## Usage

```python
import xon

config = xon.parse_file("config.xon")
data = xon.parse_string('{ app: "demo", enabled: true }')
text = xon.dumps({"app": "demo", "enabled": True}, indent=2)
```

## Native Library

The package requires the native `libxon` library.

- Wheels include the platform-native library artifact produced during build.
- You can override library resolution with `XON_LIB_PATH`.
