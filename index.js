// JavaScript wrapper for the native Xon module
try {
    const xon = require('./build/Release/xon.node');
    
    module.exports = {
        // Branded API - xonify for parsing files
        xonify: xon.xonify,
        
        // Branded API - xonifyString for parsing strings
        xonifyString: xon.xonifyString,
        
        // Aliases for convenience
        parseFile: xon.xonify,
        parse: xon.xonifyString
    };
} catch (err) {
    console.error('Failed to load Xon native module. Have you run `npm install`?');
    console.error(err.message);
    throw err;
}
