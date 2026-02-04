#include <stdio.h>
#include "../include/xon_api.h"

int main() {
    printf("=== Xon Library Usage Example ===\n\n");

    // Parse a file using the branded xonify() function
    XonValue* config = xonify("examples/config.xon");
    if (!config) {
        fprintf(stderr, "Failed to parse file\n");
        return 1;
    }

    printf("✅ File parsed successfully!\n\n");

    // ============ Access String Values ============
    XonValue* app_name = xon_object_get(config, "app_name");
    if (xon_is_string(app_name)) {
        printf("📦 App Name: %s\n", xon_get_string(app_name));
    }

    XonValue* version = xon_object_get(config, "version");
    if (xon_is_string(version)) {
        printf("🔢 Version: %s\n", xon_get_string(version));
    }

    // ============ Access Boolean Values ============
    XonValue* debug = xon_object_get(config, "debug");
    if (xon_is_bool(debug)) {
        printf("🐛 Debug Mode: %s\n", xon_get_bool(debug) ? "ON" : "OFF");
    }

    // ============ Access Nested Objects ============
    XonValue* server = xon_object_get(config, "server");
    if (xon_is_object(server)) {
        printf("\n🖥️  Server Configuration:\n");
        
        XonValue* host = xon_object_get(server, "host");
        if (xon_is_string(host)) {
            printf("   Host: %s\n", xon_get_string(host));
        }
        
        XonValue* port = xon_object_get(server, "port");
        if (xon_is_number(port)) {
            printf("   Port: %.0f\n", xon_get_number(port));
        }
        
        XonValue* ssl = xon_object_get(server, "ssl");
        if (xon_is_bool(ssl)) {
            printf("   SSL: %s\n", xon_get_bool(ssl) ? "enabled" : "disabled");
        }
    }

    // ============ Access Database Object ============
    XonValue* database = xon_object_get(config, "database");
    if (xon_is_object(database)) {
        printf("\n💾 Database Configuration:\n");
        
        XonValue* db_type = xon_object_get(database, "type");
        if (xon_is_string(db_type)) {
            printf("   Type: %s\n", xon_get_string(db_type));
        }
        
        XonValue* db_name = xon_object_get(database, "name");
        if (xon_is_string(db_name)) {
            printf("   Database: %s\n", xon_get_string(db_name));
        }
        
        XonValue* pool_size = xon_object_get(database, "pool_size");
        if (xon_is_number(pool_size)) {
            printf("   Pool Size: %.0f (parsed from hex 0x14)\n", xon_get_number(pool_size));
        }
    }

    // ============ Access Lists ============
    XonValue* features = xon_object_get(config, "features");
    if (xon_is_list(features)) {
        printf("\n✨ Features:\n");
        size_t count = xon_list_size(features);
        for (size_t i = 0; i < count; i++) {
            XonValue* item = xon_list_get(features, i);
            if (xon_is_string(item)) {
                printf("   - %s\n", xon_get_string(item));
            }
        }
    }

    // ============ Parse from String ============
    printf("\n=== Parsing from String ===\n");
    const char* xon_str = "{ name: \"Test\", value: 42, enabled: true }";
    XonValue* parsed = xonify_string(xon_str);
    if (parsed) {
        printf("✅ String parsed successfully!\n");
        XonValue* name = xon_object_get(parsed, "name");
        if (xon_is_string(name)) {
            printf("   Name: %s\n", xon_get_string(name));
        }
        xon_free(parsed);
    }

    // Clean up
    xon_free(config);
    printf("\n✅ Memory cleaned up!\n");
    
    return 0;
}
