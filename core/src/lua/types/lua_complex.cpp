#include <iostream>
#include <vector>
#include <unordered_map>
#include <complex>
#include "lua.hpp"

#include "Naming.hpp"
#include "ParameterType.hpp"
#include "def.hpp"
#include "ParameterTypeSystem.hpp"
#include "LoggingSystem.hpp"
#include "lua/lua_static.hpp"
#include "lua/types/lua_complex.hpp"

const std::string complexMetaName = (std::string("meta:") + std::string(Naming::Type_Complex));

void luat_complex_init(lua_State* const L, const int& type)
{
    lua_pushcfunction (L, lua_ignore_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction (L, lua_complex_equal);
    lua_setfield(L, -2, "__eq");
    lua_pushcfunction (L, lua_complex_abs);
    lua_setfield(L, -2, "__len");
    lua_pushcfunction (L, lua_complex_conj);
    lua_setfield(L, -2, "__bnot");
    lua_pushcfunction (L, lua_complex_pow);
    lua_setfield(L, -2, "__pow");
    lua_pushcfunction (L, lua_complex_add);
    lua_setfield(L, -2, "__add");
    lua_pushcfunction (L, lua_complex_mul);
    lua_setfield(L, -2, "__mul");
    lua_pushcfunction (L, lua_complex_div);
    lua_setfield(L, -2, "__div");
    lua_pushcfunction (L, lua_complex_unm);
    lua_setfield(L, -2, "__unm");
    lua_pushcfunction (L, lua_complex_tostring);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction (L, lua_complex_index);
    lua_setfield(L, -2, "__index");
}

void luat_complex_delete(lua_State* const L, void * const value, const int& type)
{
    delete (std::complex<double> *)value;
}

void push_complex_library(lua_State* const L)
{
    void** lib = (void**)lua_newuserdata(L, sizeof(void*));
    *lib = nullptr;
    lua_newtable(L);
    lua_pushcfunction (L, lua_ignore_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pushstring (L, "Library");
    lua_setfield(L, -2, "__name");
    lua_pushinteger (L, ParameterTypeSystem::pid_library);
    lua_setfield(L, -2, "__type");
    lua_pushcfunction (L, lua_complex_new);
    lua_setfield(L, -2, "__call");
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushcfunction (L, lua_complex_newPolar);
    lua_setfield(L, -2, "polar");
    lua_pushcfunction (L, lua_complex_equal);
    lua_setfield(L, -2, "equal");
    lua_pushcfunction (L, lua_complex_abs);
    lua_setfield(L, -2, "abs");
    lua_pushcfunction (L, lua_complex_arg);
    lua_setfield(L, -2, "arg");
    lua_pushcfunction (L, lua_complex_conj);
    lua_setfield(L, -2, "conj");
    lua_rawset(L, -3);
    lua_setmetatable(L, -2);
}

int lua_complex_new(lua_State* const L)
{
    if(lua_gettop(L) != 3)
    {
        LoggingSystem::Error("complex.new requires exactly 2 real arguments");
        return 0;
    }
    if(!lua_isnumber(L, 2))
    {
        LoggingSystem::Error("argument `1` to complex.new is mot a real number");
        return 0;
    }
    if(!lua_isnumber(L, 3))
    {
        LoggingSystem::Error("argument `2` to complex.new is mot a real number");
        return 0;
    }
    double a = lua_tonumber(L, 2);
    double b = lua_tonumber(L, 3);

    std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
    *c = new std::complex<double>(a, b);
    luaL_setmetatable(L, complexMetaName.c_str());
    return 1;
}

int lua_complex_newPolar(lua_State* const L)
{
    if(lua_gettop(L) != 2)
    {
        LoggingSystem::Error("complex.new requires exactly 2 real arguments");
        return 0;
    }
    if(!lua_isnumber(L, 1))
    {
        LoggingSystem::Error("argument `1` to complex.new is mot a real number");
        return 0;
    }
    if(!lua_isnumber(L, 2))
    {
        LoggingSystem::Error("argument `2` to complex.new is not a real number");
        return 0;
    }
    double a = lua_tonumber(L, 1);
    double b = lua_tonumber(L, 2);

    std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
    *c = new std::complex<double>(std::polar(a, b));
    luaL_setmetatable(L, complexMetaName.c_str());

    return 1;
}

int lua_complex_abs(lua_State* const L)
{
    std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
    if(a)
    {
        lua_pushnumber(L, std::abs(**a));
        return 1;
    }
    else
    {
        LoggingSystem::Error("argument `1` to complex.abs is not a complex number");
        return 0;
    }
}

int lua_complex_arg(lua_State* const L)
{
    std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
    if(a)
    {
        lua_pushnumber(L, std::arg(**a));
        return 1;
    }
    else
    {
        LoggingSystem::Error("argument `1` to complex.arg is not a complex number");
        return 0;
    }
}

int lua_complex_conj(lua_State* const L)
{
    std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
    if(a)
    {
        std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
        *c = new std::complex<double>(std::conj(**a));
        luaL_setmetatable(L, complexMetaName.c_str());
        return 1;
    }
    else
    {
        LoggingSystem::Error("argument `1` to complex.conj is not a complex number");
        return 0;
    }
}

int lua_complex_tostring(lua_State* const L)
{
    std::complex<double>* c = *((std::complex<double>**)lua_touserdata(L, -1));
    if(c->imag() < 0)
    {
        lua_pushfstring(L, "%f%fi", c->real(), c->imag());
    }
    else
    {
        lua_pushfstring(L, "%f+%fi", c->real(), c->imag());
    }

    return 1;
}

int lua_complex_index(lua_State* const L)
{
    if(lua_isstring(L, 2))
    {
        std::complex<double>* c = *((std::complex<double>**)lua_touserdata(L, 1));
        std::string item(lua_tostring(L, 2));
        if(item == "real")
        {
            lua_pushnumber(L, c->real());
            return 1;
        }
        else if(item == "imag")
        {
            lua_pushnumber(L, c->imag());
            return 1;
        }
        else if(item == "length")
        {
            lua_pushnumber(L, std::abs(*c));
            return 1;
        }
        else if(item == "arg")
        {
            lua_pushnumber(L, std::arg(*c));
            return 1;
        }
        else if(item == "conj")
        {
            std::complex<double>** c2 = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            *c2 = new std::complex<double>(std::conj(*c));
            luaL_setmetatable(L, complexMetaName.c_str());
            return 1;
        }
        else
        {
            LoggingSystem::Error("no valid index lookup for complex number");
            return 0;
        }
    }
    else
    {
        LoggingSystem::Error("no valid index lookup for complex number");
        return 0;
    }
}

int lua_complex_add(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)lua_touserdata(L, 1);
        std::complex<double> **b = (std::complex<double> **)lua_touserdata(L, 2);
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            luaL_setmetatable(L, complexMetaName.c_str());
            if(a)
            {
                if(b)
                {
                    *c = new std::complex<double>(**a + **b);
                }
                else
                {
                    *c = new std::complex<double>(**a + lua_tonumber(L, 2));
                }
            }
            else
            {
                *c = new std::complex<double>(lua_tonumber(L, 1) + **b);
            }

            return 1;
        }
    }
    LoggingSystem::Error("invalid arguments to complex.add");
    return 0;
}

int lua_complex_sub(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
        std::complex<double> **b = (std::complex<double> **)luaL_checkudata(L, 2, complexMetaName.c_str());
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            luaL_setmetatable(L, complexMetaName.c_str());
            if(a)
            {
                if(b)
                {
                    *c = new std::complex<double>(**a - **b);
                }
                else
                {
                    *c = new std::complex<double>(**a - lua_tonumber(L, 2));
                }
            }
            else
            {
                *c = new std::complex<double>(lua_tonumber(L, 1) - **b);
            }

            return 1;
        }
    }
    LoggingSystem::Error("invalid arguments to complex.sub");
    return 0;
}

int lua_complex_mul(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
        std::complex<double> **b = (std::complex<double> **)luaL_checkudata(L, 2, complexMetaName.c_str());
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            luaL_setmetatable(L, complexMetaName.c_str());
            if(a)
            {
                if(b)
                {
                    *c = new std::complex<double>((**a) * (**b));
                }
                else
                {
                    *c = new std::complex<double>(**a * lua_tonumber(L, 2));
                }
            }
            else
            {
                *c = new std::complex<double>(lua_tonumber(L, 1) * (**b));
            }

            return 1;
        }
    }
    LoggingSystem::Error("invalid arguments to complex.mul");
    return 0;
}

int lua_complex_div(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
        std::complex<double> **b = (std::complex<double> **)luaL_checkudata(L, 2, complexMetaName.c_str());
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            luaL_setmetatable(L, complexMetaName.c_str());
            if(a)
            {
                if(b)
                {
                    *c = new std::complex<double>(**a / **b);
                }
                else
                {
                    *c = new std::complex<double>(**a / lua_tonumber(L, 2));
                }
            }
            else
            {
                *c = new std::complex<double>(lua_tonumber(L, 1) / **b);
            }

            return 1;
        }
    }
    LoggingSystem::Error("invalid arguments to complex.div");
    return 0;
}

int lua_complex_pow(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
        std::complex<double> **b = (std::complex<double> **)luaL_checkudata(L, 2, complexMetaName.c_str());
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
            luaL_setmetatable(L, complexMetaName.c_str());
            if(a)
            {
                if(b)
                {
                    *c = new std::complex<double>(std::pow(**a, **b));
                }
                else
                {
                    *c = new std::complex<double>(std::pow(**a, lua_tonumber(L, 2)));
                }
            }
            else
            {
                *c = new std::complex<double>(std::pow(lua_tonumber(L, 1), **b));
            }

            return 1;
        }
    }
    LoggingSystem::Error("invalid arguments to complex.pow");
    return 0;
}

int lua_complex_unm(lua_State* const L)
{
    std::complex<double> *a = *((std::complex<double> **)lua_touserdata(L, 1));
    std::complex<double>** c = (std::complex<double>**)lua_newuserdata(L, sizeof(std::complex<double>*));
    *c = new std::complex<double>(-a->real(), -a->imag());
    luaL_setmetatable(L, complexMetaName.c_str());
    return 1;
}

int lua_complex_equal(lua_State* const L)
{
    if(lua_gettop(L) == 2)
    {
        std::complex<double> **a = (std::complex<double> **)luaL_checkudata(L, 1, complexMetaName.c_str());
        std::complex<double> **b = (std::complex<double> **)luaL_checkudata(L, 2, complexMetaName.c_str());
        if((a || lua_isnumber(L, 1)) && (b || lua_isnumber(L, 2)))
        {
            if(a)
            {
                if(b)
                {
                    lua_pushboolean(L, **a == **b);
                }
                else
                {
                    lua_pushboolean(L, **a == lua_tonumber(L, 2));
                }
            }
            else
            {
                lua_pushboolean(L, lua_tonumber(L, 1) == **b);
            }
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}
