#include <iostream>
#include <vector>
#include <memory>
#include <limits>


template<typename T>
class temp_allocator
{
public:

private:
}; 

namespace lstl
{
template<typename T>
class tracked_allocator
{
public:

  using value_type =  T;   // only essential alias
  using pointer = T*;          // remaining aliases can be deduced by allocator_traits
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void *;
  using size_type  = size_t;
  using difference_type = std::ptrdiff_t;

  template<typename U>
  struct rebind
  {
    using other = tracked_allocator<U>;
  };  


  tracked_allocator() = default;
  template<typename U> 
  tracked_allocator(const tracked_allocator<U>& other) noexcept {}
  ~tracked_allocator() = default;

  pointer allocate(size_type n)
  {
    m_allocations += n;
    return static_cast<pointer>(operator new(sizeof(T)*n));
  }

  pointer allocate(size_type n, const_void_pointer hint)
  {
    return allocate(n);
  }

  void deallocate(pointer p, size_type count)
  {
    operator delete(p);
  }

  size_type max_size() const
  {
    return std::numeric_limits<size_type>::max();
  }

  template<typename U, class... Args>
  void construct(U* p, Args && ...args)
  {
    new(p) U(std::forward<Args>(args)...);
  }

  template<typename U>
  void destroy(U* p)
  {
    p->~U();
  }

  size_type get_allocations() const
  {
    return m_allocations;
  }

private:
  static size_type m_allocations;
};

template<typename T>
typename tracked_allocator<T>::size_type tracked_allocator<T>::m_allocations=0;

template<class T>
bool operator==(tracked_allocator<T> const&, tracked_allocator<T> const& );

template<class T>
bool operator!=(tracked_allocator<T> const&, tracked_allocator<T> const& );







  /*template<class T>
  class minimal_allocator
  {
  public:

    typedef T  value_type;
   
    minimal_allocator()=default;
    template<class U> minimal_allocator(const minimal_allocator<U>& other) noexcept {}
    ~minimal_allocator()=default;
    
    template<class U>
    bool operator==(const minimal_allocator<U>& other) const noexcept
    {
      return true;
    }
    template<class U>
    bool operator!=(const minimal_allocator<U>& other) const noexcept
    {
      return false;
    }
    
    T* allocate(std::size_t n) const
    {
      return static_cast<T*>(operator new(sizeof(T)*n));
    }
    void deallocate(T* const p, std::size_t)
    {
      operator delete(p);
    }
  };
}; */





  template<class T>
  class minimal_allocator
  {
  public:

    typedef T  value_type;
   
    minimal_allocator()=default;
    template<class U> minimal_allocator(const minimal_allocator<U>& other) noexcept {}
    ~minimal_allocator()=default;
    
    template<class U>
    bool operator==(const minimal_allocator<U>& other) const noexcept
    {
      return true;
    }
    template<class U>
    bool operator!=(const minimal_allocator<U>& other) const noexcept
    {
      return false;
    }
    
    T* allocate(std::size_t n) const
    {
      return static_cast<T*>(operator new(sizeof(T)*n));
    }
    void deallocate(T* const p, std::size_t)
    {
      operator delete(p);
    }
  };

}

