#pragma once

#include <AK/String.h>
#include <AK/Function.h>
#include <AK/Types.h>
#include <AK/WeakPtr.h>

class CObject;

class CEvent {
public:
    enum Type {
        Invalid = 0,
        Quit,
        Timer,
        NotifierRead,
        NotifierWrite,
        DeferredInvoke,
        ChildAdded,
        ChildRemoved,
        Custom,
    };

    CEvent() {}
    explicit CEvent(unsigned type)
        : m_type(type)
    {
    }
    virtual ~CEvent() {}

    unsigned type() const { return m_type; }

    bool is_accepted() const { return m_accepted; }
    void accept() { m_accepted = true; }
    void ignore() { m_accepted = false; }

private:
    unsigned m_type { Type::Invalid };
    bool m_accepted { true };
};

class CDeferredInvocationEvent : public CEvent {
    friend class CEventLoop;

public:
    CDeferredInvocationEvent(Function<void(CObject&)> invokee)
        : CEvent(CEvent::Type::DeferredInvoke)
        , m_invokee(move(invokee))
    {
    }

private:
    Function<void(CObject&)> m_invokee;
};

class CTimerEvent final : public CEvent {
public:
    explicit CTimerEvent(int timer_id)
        : CEvent(CEvent::Timer)
        , m_timer_id(timer_id)
    {
    }
    ~CTimerEvent() {}

    int timer_id() const { return m_timer_id; }

private:
    int m_timer_id;
};

class CNotifierReadEvent final : public CEvent {
public:
    explicit CNotifierReadEvent(int fd)
        : CEvent(CEvent::NotifierRead)
        , m_fd(fd)
    {
    }
    ~CNotifierReadEvent() {}

    int fd() const { return m_fd; }

private:
    int m_fd;
};

class CNotifierWriteEvent final : public CEvent {
public:
    explicit CNotifierWriteEvent(int fd)
        : CEvent(CEvent::NotifierWrite)
        , m_fd(fd)
    {
    }
    ~CNotifierWriteEvent() {}

    int fd() const { return m_fd; }

private:
    int m_fd;
};

class CChildEvent final : public CEvent {
public:
    CChildEvent(Type, CObject& child, CObject* insertion_before_child = nullptr);
    ~CChildEvent();

    CObject* child() { return m_child.ptr(); }
    const CObject* child() const { return m_child.ptr(); }

    CObject* insertion_before_child() { return m_insertion_before_child.ptr(); }
    const CObject* insertion_before_child() const { return m_insertion_before_child.ptr(); }

private:
    WeakPtr<CObject> m_child;
    WeakPtr<CObject> m_insertion_before_child;
};

class CCustomEvent : public CEvent {
public:
    CCustomEvent(int custom_type, void* data = nullptr)
        : CEvent(CEvent::Type::Custom)
        , m_custom_type(custom_type)
        , m_data(data)
    {
    }
    ~CCustomEvent() {}

    int custom_type() const { return m_custom_type; }
    void* data() { return m_data; }
    const void* data() const { return m_data; }

private:
    int m_custom_type { 0 };
    void* m_data { nullptr };
};
