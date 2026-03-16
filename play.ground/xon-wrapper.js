class XonParser {
    constructor(module) {
        this.module = module;
        this.FS = module.FS;
    }

    parse(code) {
        try {
            const tokens = this.tokenize(code);
            const ast = this.buildAST(tokens);
            return { success: true, ast, error: null };
        } catch (e) {
            return { success: false, ast: null, error: e.message };
        }
    }

    tokenize(code) {
        const tokens = [];
        let i = 0;
        let line = 1;

        const push = (type, value = null) => tokens.push({ type, value, line });

        while (i < code.length) {
            const c = code[i];

            if (c === "\n") {
                line++;
                i++;
                continue;
            }
            if (/\s/.test(c)) {
                i++;
                continue;
            }

            if (c === "/" && code[i + 1] === "/") {
                i += 2;
                while (i < code.length && code[i] !== "\n") i++;
                continue;
            }
            if (c === "/" && code[i + 1] === "*") {
                i += 2;
                let closed = false;
                while (i < code.length) {
                    if (code[i] === "\n") line++;
                    if (code[i] === "*" && code[i + 1] === "/") {
                        i += 2;
                        closed = true;
                        break;
                    }
                    i++;
                }
                if (!closed) throw new Error(`Unterminated block comment at line ${line}`);
                continue;
            }

            if ("{}[]:,".includes(c)) {
                push(c, c);
                i++;
                continue;
            }

            if (c === '"') {
                i++;
                let value = "";
                while (i < code.length) {
                    const ch = code[i];
                    if (ch === '"') {
                        i++;
                        break;
                    }
                    if (ch === "\n") line++;
                    if (ch === "\\") {
                        i++;
                        if (i >= code.length) throw new Error(`Unterminated escape sequence at line ${line}`);
                        const esc = code[i];
                        const map = { n: "\n", t: "\t", r: "\r", '"': '"', "\\": "\\" };
                        value += map[esc] ?? esc;
                        i++;
                        continue;
                    }
                    value += ch;
                    i++;
                }
                push("STRING", value);
                continue;
            }

            if (c === "-" || /[0-9]/.test(c)) {
                let start = i;
                i++;
                if (code[start] === "-" && i >= code.length) {
                    throw new Error(`Invalid number at line ${line}`);
                }

                if (code[start] === "0" || (code[start] === "-" && code[start + 1] === "0")) {
                    const xIndex = code[start] === "-" ? start + 2 : start + 1;
                    if (code[xIndex] === "x" || code[xIndex] === "X") {
                        i = xIndex + 1;
                        const hexStart = i;
                        while (i < code.length && /[0-9a-fA-F]/.test(code[i])) i++;
                        if (i === hexStart) throw new Error(`Invalid hex number at line ${line}`);
                        const raw = code.slice(start, i);
                        push("NUMBER", Number(raw));
                        continue;
                    }
                }

                while (i < code.length && /[0-9eE+.\-]/.test(code[i])) i++;
                const raw = code.slice(start, i);
                const parsed = Number(raw);
                if (Number.isNaN(parsed)) {
                    throw new Error(`Invalid number '${raw}' at line ${line}`);
                }
                push("NUMBER", parsed);
                continue;
            }

            if (/[a-zA-Z_]/.test(c)) {
                const start = i;
                i++;
                while (i < code.length && /[a-zA-Z0-9_]/.test(code[i])) i++;
                const word = code.slice(start, i);
                if (word === "true") push("BOOL", true);
                else if (word === "false") push("BOOL", false);
                else if (word === "null") push("NULL", null);
                else push("IDENTIFIER", word);
                continue;
            }

            throw new Error(`Unexpected character '${c}' at line ${line}`);
        }

        return tokens;
    }

    buildAST(tokens) {
        let pos = 0;
        const peek = () => tokens[pos];
        const consume = () => tokens[pos++];
        const expect = (type) => {
            const token = consume();
            if (!token || token.type !== type) {
                throw new Error(`Expected ${type}, got ${token ? token.type : "EOF"}`);
            }
            return token;
        };

        const parseKey = () => {
            const token = peek();
            if (!token) throw new Error("Unexpected end of input while parsing key");
            if (token.type === "STRING" || token.type === "IDENTIFIER") {
                consume();
                return token.value;
            }
            throw new Error(`Invalid object key token: ${token.type}`);
        };

        const parseValue = () => {
            const token = peek();
            if (!token) throw new Error("Unexpected end of input");

            if (token.type === "{") return parseObject();
            if (token.type === "[") return parseList();
            if (token.type === "STRING") return { type: "STRING", value: consume().value };
            if (token.type === "NUMBER") return { type: "NUMBER", value: consume().value };
            if (token.type === "BOOL") return { type: "BOOL", value: consume().value };
            if (token.type === "NULL") {
                consume();
                return { type: "NULL", value: null };
            }
            throw new Error(`Unexpected token: ${token.type}`);
        };

        const parseObject = () => {
            expect("{");
            const pairs = [];

            while (peek() && peek().type !== "}") {
                const key = parseKey();
                expect(":");
                const value = parseValue();
                pairs.push({ key, value });

                if (peek() && peek().type === ",") {
                    consume();
                    if (peek() && peek().type === "}") break;
                } else if (peek() && peek().type !== "}") {
                    throw new Error(`Expected ',' or '}', got ${peek().type}`);
                }
            }

            expect("}");
            return { type: "OBJECT", pairs };
        };

        const parseList = () => {
            expect("[");
            const items = [];

            while (peek() && peek().type !== "]") {
                items.push(parseValue());
                if (peek() && peek().type === ",") {
                    consume();
                    if (peek() && peek().type === "]") break;
                } else if (peek() && peek().type !== "]") {
                    throw new Error(`Expected ',' or ']', got ${peek().type}`);
                }
            }

            expect("]");
            return { type: "LIST", items };
        };

        const root = parseValue();
        if (pos !== tokens.length) {
            throw new Error(`Unexpected token after root value: ${tokens[pos].type}`);
        }
        return root;
    }

    astToJSON(ast) {
        switch (ast.type) {
            case "OBJECT": {
                const out = {};
                ast.pairs.forEach(({ key, value }) => {
                    out[key] = this.astToJSON(value);
                });
                return out;
            }
            case "LIST":
                return ast.items.map((item) => this.astToJSON(item));
            case "STRING":
            case "NUMBER":
            case "BOOL":
            case "NULL":
                return ast.value;
            default:
                return null;
        }
    }

    jsonToAst(value) {
        if (value === null) return { type: "NULL", value: null };
        if (typeof value === "boolean") return { type: "BOOL", value };
        if (typeof value === "number") return { type: "NUMBER", value };
        if (typeof value === "string") return { type: "STRING", value };
        if (Array.isArray(value)) {
            return { type: "LIST", items: value.map((item) => this.jsonToAst(item)) };
        }
        if (typeof value === "object") {
            return {
                type: "OBJECT",
                pairs: Object.entries(value).map(([key, item]) => ({
                    key,
                    value: this.jsonToAst(item),
                })),
            };
        }
        return { type: "NULL", value: null };
    }

    formatAST(ast) {
        return this.formatXon(ast, 4);
    }

    formatXon(ast, indentSize = 4, depth = 0) {
        const pad = " ".repeat(indentSize * depth);
        const childPad = " ".repeat(indentSize * (depth + 1));
        const isIdentifier = (key) => /^[A-Za-z_][A-Za-z0-9_]*$/.test(key);

        switch (ast.type) {
            case "OBJECT":
                if (ast.pairs.length === 0) return "{}";
                return `{\n${ast.pairs
                    .map(({ key, value }) => {
                        const formattedKey = isIdentifier(key) ? key : JSON.stringify(key);
                        return `${childPad}${formattedKey}: ${this.formatXon(value, indentSize, depth + 1)}`;
                    })
                    .join(",\n")}\n${pad}}`;
            case "LIST":
                if (ast.items.length === 0) return "[]";
                return `[\n${ast.items
                    .map((item) => `${childPad}${this.formatXon(item, indentSize, depth + 1)}`)
                    .join(",\n")}\n${pad}]`;
            case "STRING":
                return JSON.stringify(ast.value);
            case "NUMBER":
                return String(ast.value);
            case "BOOL":
                return ast.value ? "true" : "false";
            case "NULL":
                return "null";
            default:
                return "null";
        }
    }
}
