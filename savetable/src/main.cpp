#define LIB_NAME "savetable"
#define MODULE_NAME "savetable"

#include <dmsdk/dlib/crypt.h>
#include <dmsdk/sdk.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace dmScript {
extern const uint32_t MAX_BUFFER_SIZE = 512 * 1024;

extern union SaveLoadBuffer {
    uint32_t m_alignment;           // This alignment is required for js-web
    char m_buffer[MAX_BUFFER_SIZE]; // Resides in .bss
} g_saveload;

uint32_t CheckTable(lua_State *L, char *buffer, uint32_t buffer_size, int index);
void PushTable(lua_State *L, const char *buffer, uint32_t buffer_size);
} // namespace dmScript

static int Stringify(lua_State *L) {
    using namespace dmScript;
    using namespace dmCrypt;

    luaL_checktype(L, 1, LUA_TTABLE);
    uint32_t n_used = CheckTable(L, g_saveload.m_buffer, sizeof(g_saveload.m_buffer), 1);

    uint32_t b64_len = n_used * 4 / 3 + 4;
    uint8_t *b64 = (uint8_t *)malloc(b64_len);
    if (!Base64Encode((uint8_t *)g_saveload.m_buffer, n_used, b64, &b64_len)) {
        free(b64);
        return luaL_error(L, "Can't encode data into Base64 string.");
    }

    lua_pushlstring(L, (char *)b64, b64_len);
    free(b64);

    return 1;
}

static int ParseString(lua_State *L) {
    using namespace dmScript;
    using namespace dmCrypt;

    const char *b64 = luaL_checkstring(L, 1);
    const uint32_t b64_len = strlen(b64);
    if (b64_len / 4 * 3 > MAX_BUFFER_SIZE) {
        return luaL_error(L, "Data is too long. Max length is %d.", MAX_BUFFER_SIZE);
    }

    uint32_t nread = MAX_BUFFER_SIZE;
    if (!Base64Decode((const uint8_t *)b64, b64_len, (uint8_t *)g_saveload.m_buffer, &nread)) {
        return luaL_error(L, "Base64 string is invalid.");
    }

    PushTable(L, g_saveload.m_buffer, nread);
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] = {{"stringify", Stringify},
                                          {"parse_string", ParseString},
                                          /* Sentinel: */
                                          {NULL, NULL}};

static void LuaInit(lua_State *L) {
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result InitializeExtension(dmExtension::Params *params) {
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(savetable, LIB_NAME, 0, 0, InitializeExtension, 0, 0, 0)
