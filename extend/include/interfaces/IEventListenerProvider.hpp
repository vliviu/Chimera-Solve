#ifndef IEVENTLISTENERPROVIDER_H
#define IEVENTLISTENERPROVIDER_H


class IEventListenerProvider
{
    public:
        IEventListenerProvider() {}
        virtual ~IEventListenerProvider() {}
        virtual IEventListener* provideListener(size_t id, void* args) = 0;
    protected:
    private:
};

#endif // IEVENTLISTENERPROVIDER_H
