// TypeScript type definitions for Xon
declare module 'xon' {
    export type XonValue = null | boolean | number | string | XonObject | XonArray;
    
    export interface XonObject {
        [key: string]: XonValue;
    }
    
    export type XonArray = XonValue[];
    
    /**
     * Parse a .xon file from disk (Branded API)
     * @param path - Path to the .xon file
     * @returns Parsed XonValue or throws on error
     * @example
     * const config = xonify('config.xon');
     * console.log(config.server.port);
     */
    export function xonify(path: string): XonValue;
    
    /**
     * Parse a .xon string (Branded API)
     * @param input - Xon-formatted string
     * @returns Parsed XonValue or throws on error
     * @example
     * const data = xonifyString('{ name: "test", value: 42 }');
     */
    export function xonifyString(input: string): XonValue;
}
