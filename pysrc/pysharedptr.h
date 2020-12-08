#pragma once

#include <pybind11/cast.h>

// PySharedPtr is a smart pointer like std::shared_ptr, that retains shared ownership of an object
// through a pointer. Additionally, it provides a method to release the ownership of the
// internal pointer to let it be managed by the CTL. This prevents the memory pointed to by the
// internal pointer to be deleted more than once.
template<class T>
class PySharedPtr
{
public:
    constexpr PySharedPtr() noexcept;
    constexpr PySharedPtr(std::nullptr_t) noexcept;
    explicit PySharedPtr(T* ptr);
    PySharedPtr(const PySharedPtr& other) noexcept;
    PySharedPtr(PySharedPtr&& other) noexcept;
    PySharedPtr& operator=(const PySharedPtr& other) noexcept;
    PySharedPtr& operator=(PySharedPtr&& other) noexcept;
    ~PySharedPtr();

    T* get() const noexcept { return _ptr; }

    T* release_to_ctl() const;

    explicit operator bool() const noexcept { return _ptr != nullptr; }

private:
    struct ControlBlock
    {
        unsigned shared_count;
        bool managed_by_ctl;
        ControlBlock(unsigned shared_count, bool managed_by_ctl)
            : shared_count{shared_count}, managed_by_ctl{managed_by_ctl} {}
    };
    T* _ptr;
    ControlBlock* _ctrl;
};

template<class T, typename... Args>
PySharedPtr<T> make_pysharedptr(Args... args)
{
    return PySharedPtr<T>{new T{std::forward<Args>(args)...}};
}

PYBIND11_DECLARE_HOLDER_TYPE(T, PySharedPtr<T>);

template<class T>
constexpr PySharedPtr<T>::PySharedPtr() noexcept
    : _ptr{nullptr}, _ctrl{nullptr}
{
}

template<class T>
constexpr PySharedPtr<T>::PySharedPtr(std::nullptr_t) noexcept
    : PySharedPtr{}
{
}

template<class T>
PySharedPtr<T>::PySharedPtr(T* ptr)
    : _ptr{ptr}, _ctrl{new ControlBlock{1, false}}
{
}

template<class T>
PySharedPtr<T>::PySharedPtr(const PySharedPtr& other) noexcept
    : _ptr{other._ptr}, _ctrl{other._ctrl}
{
    if (_ctrl)
    {
        _ctrl->shared_count += 1;
    }
}

template<class T>
PySharedPtr<T>::PySharedPtr(PySharedPtr&& other) noexcept
    : _ptr{other._ptr}, _ctrl{other._ctrl}
{
    other._ptr = nullptr;
    other._ctrl = nullptr;
}

template<class T>
PySharedPtr<T>& PySharedPtr<T>::operator=(const PySharedPtr& other) noexcept
{
    if (other._ptr == _ptr)
    {
        return *this;
    }

    if (_ctrl && _ctrl->shared_count == 1)
    {
        delete _ptr;
        delete _ctrl;
    }

    _ptr = other._ptr;
    _ctrl = other._ctrl;
    if (_ctrl)
    {
        _ctrl->shared_count += 1;
    }
    return *this;
}

template<class T>
PySharedPtr<T>& PySharedPtr<T>::operator=(PySharedPtr&& other) noexcept
{
    if (other._ptr == _ptr)
    {
        return *this;
    }

    if (_ctrl && _ctrl->shared_count == 1)
    {
        delete _ptr;
        delete _ctrl;
    }

    _ptr = other._ptr;
    _ctrl = other._ctrl;

    _ptr = nullptr;
    _ctrl = nullptr;

    return *this;
}

template<class T>
PySharedPtr<T>::~PySharedPtr()
{
    if (_ctrl && _ctrl->shared_count > 1)
    {
        _ctrl->shared_count -= 1;
    }
    else if (_ctrl && _ctrl->shared_count == 1)
    {
        if (!_ctrl->managed_by_ctl)
        {
            delete _ptr;
        }
        delete _ctrl;
    }
}

template<class T>
T* PySharedPtr<T>::release_to_ctl() const
{
    if (_ctrl)
    {
        _ctrl->managed_by_ctl = true;
    }
    return _ptr;
}