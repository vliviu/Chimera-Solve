#ifndef LUA_ENTRYPOINT_H
#define LUA_ENTRYPOINT_H

void lua_EntryPoint_init(lua_State* L, EntryPoint* ep);

int lua_EntryPoint_call(lua_State* L);

int lua_EntryPoint_len(lua_State* L);

int lua_EntryPoint_tostring(lua_State* L);

#endif // LUA_ENTRYPOINT_H
