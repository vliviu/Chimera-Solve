#ifndef ICONNECTEVENTHANDLER_H
#define ICONNECTEVENTHANDLER_H

class ParameterType;
class EntryPoint;
class Registry;

typedef struct lua_State lua_State;

class IConnectEventHandler
{
    public:
        virtual void notifyLoad(ParameterType const * const p) = 0;
        virtual void notifyUnload(ParameterType const * const p) = 0;
        virtual void notifyLoad(EntryPoint* ep) = 0;
        virtual void notifyUnload(EntryPoint* ep) = 0;
        virtual void notifyLoad(const std::string& name, EntryPoint* const ep, Registry const * const reg) = 0;
        virtual void notifyUnload(const std::string& name, EntryPoint* const ep, Registry* const reg) = 0;
        virtual lua_State* getLuaState() = 0;
    protected:
    private:
};

#endif // ICONNECTEVENTHANDLER_H
