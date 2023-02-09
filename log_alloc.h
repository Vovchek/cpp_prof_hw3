#include <iostream>

// #define USE_PRETTY 1
// #define DO_LOG

template <typename T, int sz_reserve>
struct logging_allocator
{
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind
    {
        using other = logging_allocator<U, sz_reserve>;
    };

    logging_allocator() = default;
    ~logging_allocator()
    {
        if (mem)
        {
#if defined(USE_PRETTY) || defined(DO_LOG)
            std::cout << "mem free @" << mem << '\n';
#endif
            std::free(mem);
        }
    }

    T *allocate(std::size_t n)
    {
#ifdef USE_PRETTY
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << '\n';
#elif defined(DO_LOG)
        std::cout << "allocate: [n = " << n << "]" << '\n';
#endif
        if (!mem && n <= sz_reserve)
        {
            mem = reinterpret_cast<T *>(std::malloc(sz_reserve * sizeof(T)));
#if defined(USE_PRETTY) || defined(DO_LOG)
            std::cout << "mem allocated @" << mem << '\n';
#endif
        }
        if (!mem || (allocated + n) > sz_reserve)
            throw std::bad_alloc();
        auto p = mem + allocated;
        allocated += n;
        return p;
    }
    void deallocate(T *p, std::size_t n)
    {
#ifdef USE_PRETTY
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << ", n = " << n << "]" << '\n';
#elif defined(DO_LOG)
        std::cout << "deallocate: [p = " << p << ", n = " << n << "]" << '\n';
#endif
        if (p == (mem + allocated - n)) // can free only upper chunks
            allocated -= n;
        else
        {
            // ???
        }
    }
    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
#ifdef USE_PRETTY
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]\n";
#elif defined(DO_LOG)
        std::cout << "construct: [p = " << p << "]\n";
#endif
        new (p) U(std::forward<Args>(args)...);
    }
    void destroy(T *p)
    {
#ifdef USE_PRETTY
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]\n";
#elif defined(DO_LOG)
        std::cout << "destroy: [p = " << p << "]\n";
#endif
        p->~T();
    }

private:
    T *mem{nullptr};
    size_t allocated{0};
};

// Return true if allocators b and a can be safely interchanged. "Safely interchanged" means that b could be
// used to deallocate storage obtained through a and vice versa.
template <class T1, class T2, int N1, int N2>
bool operator==(const logging_allocator<T1, N1> &a, const logging_allocator<T2, N2> &b) noexcept
{
    if (!std::is_same<T1, T2>::value)
        return false;
    return a == b;
}


