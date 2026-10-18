#include "lua.h"
#include "lauxlib.h"

#include <stddef.h>

#include "c_bindings.h"

#define MEGOLM_MESSAGE_MT "megolm_message"
#define DECRYPTION_SESSION_MT "decryption_session"

static void check_userdata_type(lua_State *L, int idx, const char *mt_name) {
    if (!luaL_checkudata(L, idx, mt_name)) {
        luaL_argerror(L, idx, lua_pushfstring(L, "expected %s userdata", mt_name));
    }
}

static int megolm_message_gc(lua_State *L) {
    HMegolmMessage **msg_ptr = (HMegolmMessage **)luaL_checkudata(L, 1, MEGOLM_MESSAGE_MT);
    if (*msg_ptr) {
        destroy_megolm_message(*msg_ptr);
        *msg_ptr = NULL;
    }
    return 0;
}

static int decryption_session_gc(lua_State *L) {
    HDecryptionSession **session_ptr = (HDecryptionSession **)luaL_checkudata(L, 1, DECRYPTION_SESSION_MT);
    if (*session_ptr) {
        destroy_decryption_session(*session_ptr);
        *session_ptr = NULL;
    }
    return 0;
}

static int new_message(lua_State *L) {
    // Handle both . and : calls
    int arg = lua_gettop(L) == 2 ? 2 : 1;
    const char *msg_b64 = luaL_checkstring(L, arg);

#if LUA_VERSION_NUM >= 504
    HMegolmMessage **msg_ptr = (HMegolmMessage **)lua_newuserdatauv(L, sizeof(HMegolmMessage *), 0);
#else
    HMegolmMessage **msg_ptr = (HMegolmMessage **)lua_newuserdata(L, sizeof(HMegolmMessage *));
#endif

    *msg_ptr = build_megolm_message(msg_b64);

    if (!*msg_ptr) {
        lua_pushnil(L);
        lua_pushstring(L, "failed to create message");
        return 2;
    }

    luaL_setmetatable(L, MEGOLM_MESSAGE_MT);
    return 1;
}

static int new_session(lua_State *L) {
    // Handle both . and : calls
    int arg = lua_gettop(L) == 2 ? 2 : 1;
    const char *key_b64 = luaL_checkstring(L, arg);

#if LUA_VERSION_NUM >= 504
    HDecryptionSession **session_ptr = (HDecryptionSession **)lua_newuserdatauv(L, sizeof(HDecryptionSession *), 0);
#else
    HDecryptionSession **session_ptr = (HDecryptionSession **)lua_newuserdata(L, sizeof(HDecryptionSession *));
#endif

    *session_ptr = build_decryption_session(key_b64);

    if (!*session_ptr) {
        lua_pushnil(L);
        lua_pushstring(L, "failed to create session");
        return 2;
    }

    luaL_setmetatable(L, DECRYPTION_SESSION_MT);
    return 1;
}

static int megolm_decrypt(lua_State *L) {
    int n = lua_gettop(L);
    int session_idx = 1;
    int message_idx = 2;

    // Adjust indices for colon calls
    if (n == 3) {
        session_idx = 2;
        message_idx = 3;
    }
    else if (n != 2) {
        return luaL_error(L, "expecting 2 arguments (session, message)");
    }

    check_userdata_type(L, session_idx, DECRYPTION_SESSION_MT);
    HDecryptionSession **session_ptr = (HDecryptionSession **)lua_touserdata(L, session_idx);

    check_userdata_type(L, message_idx, MEGOLM_MESSAGE_MT);
    HMegolmMessage **msg_ptr = (HMegolmMessage **)lua_touserdata(L, message_idx);

    DecryptedMessage *result = decrypt(*session_ptr, *msg_ptr);
    if (!result) {
        lua_pushnil(L);
        return 1;
    }

    // Create the result table
    lua_createtable(L, 0, 2);

    lua_pushlstring(L, result->data, result->len);
    lua_setfield(L, -2, "data");

    lua_pushinteger(L, result->message_index);
    lua_setfield(L, -2, "message_index");

    // Cleanup
    destroy_decrypted_message(result);
    return 1;
}


static const luaL_Reg megolm_functions[] = {
        {"new_message", new_message},
        {"new_session", new_session},
        {"decrypt", megolm_decrypt},
        {NULL, NULL}
};


static void create_metatables(lua_State *L) {
    luaL_newmetatable(L, MEGOLM_MESSAGE_MT);
    lua_pushcfunction(L, megolm_message_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    luaL_newmetatable(L, DECRYPTION_SESSION_MT);
    lua_pushcfunction(L, decryption_session_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);
}

int luaopen_megolm(lua_State *L) {
    luaL_newlib(L, megolm_functions);

    create_metatables(L);

    return 1;
}
