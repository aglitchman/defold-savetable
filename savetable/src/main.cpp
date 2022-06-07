#define LIB_NAME "savetable"
#define MODULE_NAME "savetable"

#include <dmsdk/dlib/crypt.h>
#include <dmsdk/sdk.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace dmScript {
uint32_t CheckTable(lua_State *L, char *buffer, uint32_t buffer_size, int index);
void PushTable(lua_State *L, const char *buffer, uint32_t buffer_size);
} // namespace dmScript

static int Serialize(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    int buflen = 512 * 1024;
    if (lua_isnumber(L, 2)) {
        buflen = lua_tonumber(L, 2);
    }
    char *buf = (char *)malloc(buflen);
    uint32_t n_used = dmScript::CheckTable(L, buf, buflen, 1);

    lua_pushlstring(L, (char *)buf, n_used);
    free(buf);
    return 1;
}

static int Deserialize(lua_State *L) {
    size_t srclen;
    const char* src = luaL_checklstring(L, 1, &srclen);

    dmScript::PushTable(L, src, srclen);
    return 1;
}

static int Encode_Base64(lua_State* L)
{
    size_t srclen;
    const char* src = luaL_checklstring(L, 1, &srclen);

    uint32_t dstlen = srclen * 4 / 3 + 4;
    uint8_t* dst = (uint8_t*)malloc(dstlen);
    if (!dmCrypt::Base64Encode((uint8_t *)src, srclen, dst, &dstlen)) {
        free(dst);
        return luaL_error(L, "Can't encode data into Base64 string.");
    }
    
    lua_pushlstring(L, (char *)dst, dstlen);
    free(dst);
    return 1;
}

static int Decode_Base64(lua_State* L)
{
    size_t srclen;
    const char* src = luaL_checklstring(L, 1, &srclen);

    uint32_t dstlen = srclen * 3 / 4;
    uint8_t* dst = (uint8_t*)malloc(dstlen);
    if (!dmCrypt::Base64Decode((const uint8_t*)src, srclen, dst, &dstlen))
    {
        free(dst);
        return luaL_error(L, "Can't decode Base64 string.");
    }

    lua_pushlstring(L, (char*)dst, dstlen);
    free(dst);
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] = {
    {"serialize", Serialize},
    {"deserialize", Deserialize},
    // Base64
    {"encode_base64", Encode_Base64},
    {"decode_base64", Decode_Base64},
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
