# Xon Feature Roadmap

This file tracks concrete implementation state, tests, and next work items.

## Round 1: Expression Runtime (implemented)

Implemented in this workspace:

- Variable declarations and references (`let`, `const`) with scoped lookup and `xon_eval`.
- Expression grammar with precedence and associativity:
  - arithmetic `+`, `-`, `*`, `/`, `%`
  - boolean `&&`, `||`, `!`
  - comparisons `==`, `!=`, `<`, `<=`, `>`, `>=`
  - nullish coalescing `??`
- Conditional logic:
  - ternary style `if (cond) A else B`
  - expression form `cond ? A : B`
  - short-circuit for `&&`, `||`, and left-to-right `??`
- User-defined functions:
  - anonymous function expressions `(arg1, arg2, ...) => body`
  - captured closures via lexical environment
  - function calls with arity checks and runtime errors on under/over-argument.
- Built-ins:
  - `abs`, `len`, `max`, `min`, `str`, `upper`, `lower`, `keys`, `has`, `env`

Evidence in code:
- `src/xon_api.c` (lexer, scope, evaluator, built-ins, errors)
- `src/xon.lemon` (grammar for expressions and function syntax)
- `src/main.c` (`eval` CLI command)
- `tests/test_suite.c` (Round 1 coverage)

Notes:
- Identifier reference fallback works for unknown identifiers by reading environment variables.
- Declarations are not included in the evaluated output object; they are available through scope during evaluation.
- Comment syntax available today: `//`, `#`, and `/* */`.

## Second Round: Language Structure

3. Scoped evaluation context (shadowing rules + symbol table)
4. Function call stack + diagnostics
5. Control-flow forms (`if`/`while`/list comprehensions)
6. Module/import support (`import`, `include`)
7. Template interpolation support

## Third Round: Safety and Platform

8. Runtime safety limits (call depth, timeouts, allocations)
9. Deterministic diagnostics (line/col, stack traces, richer type errors)
10. Duplicate key policies
11. Type enforcement (strict/inferred)
12. Parity updates for Node/Python runtime bindings

## Reference behavior for this repo

Use `tests/test_suite.c` as the acceptance suite for Round 1 before moving onward.
