#include <erl_nif.h>
#include <stddef.h>  // Required for size_t and ptrdiff_t and NULL
#include <stdexcept> // Required for std::length_error
 
template <class T>
struct EnifAllocator
{
    typedef T value_type;

    EnifAllocator () = default;
    template <class U>
    constexpr EnifAllocator (const EnifAllocator <U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        if (auto p = static_cast<T*>(enif_alloc(n*sizeof(T))))
            return p;

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept { enif_free(p); }
};

template <class T, class U>
bool operator==(const EnifAllocator <T>&, const EnifAllocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const EnifAllocator <T>&, const EnifAllocator <U>&) { return false; }
