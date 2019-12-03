#include <iostream>
#include <vector>
#include <memory>
#include <limits>


template<typename T>
class lallocator
{
public:

  using value_type =  T;   // only essential
  using pointer = T*;
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void *;
  using size_type  = size_t;
  using difference_type = std::ptrdiff_t;

  template<typename U>
  struct rebind
  {
    using other = lallocator<U>;
  };  


  lallocator() = default;
  template<typename U> 
  lallocator(const lallocator<U>& other) {}
  ~lallocator() = default;

  pointer allocate(size_type count)
  {
    std::cout<<"Using\n";
    m_allocations += count;
    return static_cast<pointer>(operator new(sizeof(T)*count));
  }

  pointer allocate(size_type count, const_void_pointer hint)
  {
    return allocate(count);
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
typename lallocator<T>::size_type lallocator<T>::m_allocations=0;


int main()
{
  //using the std allocator to initialise std vector

  using lint = lallocator<int>;
  using ldoub = lint::rebind<double>::other;
  std::vector<int> v(5,std::allocator<int>());
  std::vector<int, lallocator<int>> vl(5);
  for(int v : vl) std::cout<<v<<"\n";
  std::cout<<vl.get_allocator().get_allocations()<<"\n";
  return 0;
}
