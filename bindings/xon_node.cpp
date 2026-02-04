#include <napi.h>
extern "C" {
    #include "../include/xon_api.h"
}
#include <string>

// Helper to convert XonValue to JS
Napi::Value ConvertToJS(Napi::Env env, const XonValue* value);

Napi::Value ConvertToJS(Napi::Env env, const XonValue* value) {
    if (!value) return env.Null();

    switch (xon_get_type(value)) {
        case XON_TYPE_NULL:
            return env.Null();
        
        case XON_TYPE_BOOL:
            return Napi::Boolean::New(env, xon_get_bool(value));
        
        case XON_TYPE_NUMBER:
            return Napi::Number::New(env, xon_get_number(value));
        
        case XON_TYPE_STRING: {
            const char* str = xon_get_string(value);
            return str ? Napi::String::New(env, str) : env.Null();
        }
        
        case XON_TYPE_OBJECT: {
            Napi::Object obj = Napi::Object::New(env);
            size_t size = xon_object_size(value);
            
            // We need to iterate through object keys
            // For now, we'll skip proper key iteration (requires API enhancement)
            // This is a placeholder that would need the actual key enumeration API
            return obj;
        }
        
        case XON_TYPE_LIST: {
            size_t size = xon_list_size(value);
            Napi::Array arr = Napi::Array::New(env, size);
            
            for (size_t i = 0; i < size; i++) {
                XonValue* item = xon_list_get(value, i);
                arr[i] = ConvertToJS(env, item);
            }
            return arr;
        }
        
        default:
            return env.Null();
    }
}

// Branded: xonify() - Parse file function
Napi::Value Xonify(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string path = info[0].As<Napi::String>();
    XonValue* result = xonify(path.c_str());
    
    if (!result) {
        Napi::Error::New(env, "Failed to parse file").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Value jsValue = ConvertToJS(env, result);
    xon_free(result);
    return jsValue;
}

// Branded: xonifyString() - Parse string function
Napi::Value XonifyString(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string content = info[0].As<Napi::String>();
    XonValue* result = xonify_string(content.c_str());
    
    if (!result) {
        Napi::Error::New(env, "Failed to parse string").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Value jsValue = ConvertToJS(env, result);
    xon_free(result);
    return jsValue;
}

// Module initialization
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("xonify", Napi::Function::New(env, Xonify));
    exports.Set("xonifyString", Napi::Function::New(env, XonifyString));
    return exports;
}

NODE_API_MODULE(xon, Init)
