# Xon Examples

## Basic Configuration

```javascript
{
    app_name: "MyApp",
    version: "1.0.0",
    debug: true,
}
```

## Nested Objects

```javascript
{
    database: {
        host: "localhost",
        port: 5432,
        credentials: {
            user: "admin",
            password: null,
        }
    }
}
```

## Lists

```javascript
{
    colors: ["red", "green", "blue"],
    numbers: [1, 2, 3, 0xFF],
}
```

## Comments

```javascript
{
    // This is a comment
    name: "value",  // Inline comment
}
```

## Hexadecimal Numbers

```javascript
{
    max_connections: 0x64,    // 100
    buffer_size: 0x1000,      // 4096
    color: 0xFF5733,          // RGB color
}
```

## All Data Types

```javascript
{
    string: "Hello, World!",
    number: 42,
    float: 3.14,
    hex: 0xDEADBEEF,
    bool_true: true,
    bool_false: false,
    null_value: null,
    list: [1, 2, 3],
    object: { nested: "value" },
}
```
