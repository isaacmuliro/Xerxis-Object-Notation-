// XonParser: A JavaScript wrapper for the XON parser compiled to WebAssembly.
// This code assumes that the XON parser has been compiled to WebAssembly
// using Emscripten and exposes necessary functions for parsing.
// 

class XonParser {
    constructor(module) {
        this.module = module;
        this.FS = module.FS;
    }

    parse(code) {
        try {
            // Write code to virtual filesystem
            const filename = '/temp.xon';
            this.FS.writeFile(filename, code);

            // Read file and tokenize (simulating main.c logic)
            const file = this.FS.open(filename, 'r');
            const tokens = [];
            let line = 1;

            // This is a simplified version - ideally we'd expose xon_get_token via WASM
            // For now, we'll use a JavaScript lexer that matches your C lexer

            this.FS.close(file);
            this.FS.unlink(filename);

            return {
                success: true,
                ast: this.tokenize(code),
                error: null
            };
        } catch (e) {
            return {
                success: false,
                ast: null,
                error: e.message
            };
        }
    }

    // JavaScript implementation matching your C lexer
    tokenize(code) {
        const tokens = [];
        let line = 1;
        let i = 0;

        while (i < code.length) {
            let c = code[i];

            // Skip whitespace
            if (/\s/.test(c)) {
                if (c === '\n') line++;
                i++;
                continue;
            }

            // Skip comments
            if (c === '/' && code[i + 1] === '/') {
                while (i < code.length && code[i] !== '\n') i++;
                continue;
            }

            // Structural tokens
            if ('{}[]:,'.includes(c)) {
                tokens.push({ type: c, value: c, line });
                i++;
                continue;
            }

            // Strings
            if (c === '"') {
                let str = '';
                i++;
                while (i < code.length && code[i] !== '"') {
                    if (code[i] === '\\') {
                        i++;
                        const escapes = { n: '\n', t: '\t', r: '\r', '"': '"', '\\': '\\' };
                        str += escapes[code[i]] || code[i];
                    } else {
                        str += code[i];
                    }
                    i++;
                }
                i++; // skip closing "
                tokens.push({ type: 'STRING', value: str, line });
                continue;
            }

            // Numbers (including hex)
            if (/[0-9-]/.test(c)) {
                let num = '';
                if (c === '0' && (code[i + 1] === 'x' || code[i + 1] === 'X')) {
                    // Hex number
                    i += 2;
                    while (i < code.length && /[0-9A-Fa-f]/.test(code[i])) {
                        num += code[i++];
                    }
                    tokens.push({ type: 'NUMBER', value: parseInt(num, 16), line });
                } else {
                    // Decimal number
                    while (i < code.length && /[0-9.]/.test(code[i])) {
                        num += code[i++];
                    }
                    tokens.push({ type: 'NUMBER', value: parseFloat(num), line });
                }
                continue;
            }

            // Keywords and identifiers
            if (/[a-zA-Z_]/.test(c)) {
                let word = '';
                while (i < code.length && /[a-zA-Z0-9_]/.test(code[i])) {
                    word += code[i++];
                }
                const keywords = { true: 'BOOL', false: 'BOOL', null: 'NULL' };
                const type = keywords[word] || 'IDENTIFIER';
                const value = word === 'true' ? true : word === 'false' ? false : word === 'null' ? null : word;
                tokens.push({ type, value, line });
                continue;
            }

            throw new Error(`Unexpected character '${c}' at line ${line}`);
        }

        return this.buildAST(tokens);
    }

    buildAST(tokens) {
        // Simple recursive descent parser matching your grammar
        let pos = 0;

        const peek = () => tokens[pos];
        const consume = () => tokens[pos++];
        const expect = (type) => {
            const token = consume();
            if (token?.type !== type) throw new Error(`Expected ${type}, got ${token?.type}`);
            return token;
        };

        const parseValue = () => {
            const token = peek();
            if (!token) throw new Error('Unexpected end of input');

            if (token.type === '{') return parseObject();
            if (token.type === '[') return parseList();
            if (token.type === 'STRING') return { type: 'STRING', value: consume().value };
            if (token.type === 'NUMBER') return { type: 'NUMBER', value: consume().value };
            if (token.type === 'BOOL') return { type: 'BOOL', value: consume().value };
            if (token.type === 'NULL') { consume(); return { type: 'NULL', value: null }; }
            if (token.type === 'IDENTIFIER') return { type: 'STRING', value: consume().value }; // Unquoted key
            throw new Error(`Unexpected token: ${token.type}`);
        };

        const parseObject = () => {
            expect('{');
            const pairs = [];
            
            while (peek()?.type !== '}') {
                const key = parseValue(); // STRING or IDENTIFIER
                expect(':');
                const value = parseValue();
                pairs.push({ key: key.value, value });
                
                if (peek()?.type === ',') consume();
            }
            
            expect('}');
            return { type: 'OBJECT', pairs };
        };

        const parseList = () => {
            expect('[');
            const items = [];
            
            while (peek()?.type !== ']') {
                items.push(parseValue());
                if (peek()?.type === ',') consume();
            }
            
            expect(']');
            return { type: 'LIST', items };
        };

        return parseValue();
    }

    formatAST(ast, indent = 0) {
        const pad = '  '.repeat(indent);
        
        if (!ast) return 'null';
        
        switch (ast.type) {
            case 'OBJECT':
                let obj = '{\n';
                ast.pairs.forEach(({ key, value }, i) => {
                    obj += `${pad}  "${key}": ${this.formatAST(value, indent + 1)}`;
                    if (i < ast.pairs.length - 1) obj += ',';
                    obj += '\n';
                });
                obj += `${pad}}`;
                return obj;
                
            case 'LIST':
                let list = '[\n';
                ast.items.forEach((item, i) => {
                    list += `${pad}  ${this.formatAST(item, indent + 1)}`;
                    if (i < ast.items.length - 1) list += ',';
                    list += '\n';
                });
                list += `${pad}]`;
                return list;
                
            case 'STRING':
                return `"${ast.value}"`;
                
            case 'NUMBER':
                return String(ast.value);
                
            case 'BOOL':
                return String(ast.value);
                
            case 'NULL':
                return 'null';
                
            default:
                return '?';
        }
    }
}