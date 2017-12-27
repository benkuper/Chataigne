#ifndef NANO_SIGNAL_SLOT_HPP
#define NANO_SIGNAL_SLOT_HPP

#include "nano_function.hpp"
#include "nano_observer.hpp"

namespace Nano
{

template <typename RT> class Signal;
template <typename RT, typename... Args>
class Signal<RT(Args...)> : private Observer
{
    template <typename T>
    void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::remove(key, instance);
        instance->remove(key, this);
    }
    template <typename T>
    void insert_sfinae(DelegateKey const& key, ...)
    {
        Observer::insert(key, this);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, ...)
    {
        Observer::remove(key, this);
    }

    public:

    using Delegate = Function<RT(Args...)>;

    //-------------------------------------------------------------------CONNECT

    template <typename L>
    void connect(L* instance)
    {
        Observer::insert(Delegate::template bind (instance), this);
    }
    template <typename L>
    void connect(L& instance)
    {
        connect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------------------DISCONNECT

    template <typename L>
    void disconnect(L* instance)
    {
        Observer::remove(Delegate::template bind (instance), this);
    }
    template <typename L>
    void disconnect(L& instance)
    {
        disconnect(std::addressof(instance));
    }

    template <RT (* fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Delegate::template bind<fun_ptr>(), this);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::* mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::* mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }

    //----------------------------------------------------EMIT / EMIT ACCUMULATE

    #ifdef NANO_USE_DEPRECATED

    /// Will not benefit from perfect forwarding
    /// TODO [[deprecated]] when c++14 is comfortably supported

    void operator() (Args... args)
    {
        emit(std::forward<Args>(args)...);
    }
    template <typename Accumulate>
    void operator() (Args... args, Accumulate&& accumulate)
    {
        emit_accumulate<Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Args>(args)...);
    }

    #endif

    template <typename... Uref>
    void operator()(Uref&&... args)
    {
        Observer::onEach<Delegate>(std::forward<Uref>(args)...);
    }

    template <typename Accumulate, typename... Uref>
    void emit_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        Observer::onEach_Accumulate<Delegate, Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
    }

    //-------------------------------------------------------------------UTILITY

    bool empty() const
    {
        return Observer::isEmpty();
    }

    void removeAll()
    {
        Observer::removeAll();
    }

};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SIGNAL_SLOT_HPP
