#include <memory>
#include <string>

template <typename T>
class dumb_iterator
{
};

class dc_exception: public std::exception {
  std::string msg {"Exception in dumb container"};
  public:
    dc_exception() = default;
    dc_exception(const char *s) : msg{s} {};
    ~dc_exception() = default;
    std::string& what() {return msg;}
};

template <typename T>
struct Node
{
  T value;
  Node *next;
  Node(T &&v) : value{v}, next{nullptr} {};
};

template <typename T, typename A = std::allocator<T>>
class dumb_container
{
public:
  dumb_container() = default;
  ~dumb_container() = default;

  template<typename TT>
  void append(TT &&item)
  {
    if (!head)
    {
      head = tail = current = node_alloc.allocate(1);
    }
    else
    {
      tail->next = node_alloc.allocate(1);
      tail = tail->next;
      tail->next = nullptr;
    }
    node_alloc.construct(tail, std::forward<T>(item));
  }

  bool hasNext() const
  {
    return (current != nullptr);
  }

  const T &next()
  {
    if(!current)
      throw dc_exception("nullptr reference in dumb_container::next()");
    T& v = current->value;
    current = current->next;
    return v;
  }

  void reset()
  {
    current = head;
  }

private:
  Node<T> *head{nullptr};
  Node<T> *tail{nullptr};
  Node<T> *current{nullptr};
  using NodeAllocator = typename A:: template rebind<Node<T>>::other;
  NodeAllocator node_alloc;
};

namespace std
{
  template <typename T>
  struct iterator_traits<dumb_iterator<T>>
  {
    using iterator_category = forward_iterator_tag;
    using value_type = T;
    using difference_type = int;
    using pointer = T *;
    using reference = T &;
  };
}
