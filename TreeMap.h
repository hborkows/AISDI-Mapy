#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  typedef struct node
  {
    value_type value;
    node* left;
    node* right;
    node* parent;

    node(): value(key_type(), mapped_type())
    {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
    }

    node(key_type key, mapped_type map): value(key,map)
    {
      left = nullptr;
      right = nullptr;
      parent = nullptr;
    }
  }node;

  node* root;
  node* guard;
  size_type size;

  friend class ConstIterator;
  friend class Iterator;

  void destroy(node*& current) //delete nodes from tree
  {
    if(current != nullptr)
    {
      destroy(current->left);
      destroy(current->right);
      delete current;
      current = nullptr;
    }
  }

  node* lookFor(node* current, const key_type& key) const //look for node with given key in tree, if not found returns nullptr
  {
    if(current == nullptr || current->value.first == key)
    {
      return current;
    }
    else if(current->value.first < key)
    {
      return lookFor(current->right, key);
    }
    else
    {
      return lookFor(current->left, key);
    }
  }

  void remove(node*& current, key_type number) //remove node with given key from tree
  {
    if(current != nullptr)
    {
        if(current->value.first == number) //found correct node
        {
            if(current->left == nullptr && current->right == nullptr) //current node is a leaf
            {
                if(current == root)
                {
                  guard->left = nullptr;
                }
                delete current;
                current = nullptr;
            }
            else if(current->left == nullptr)
            {
                if(current == root)
                {
                  guard->left = current->right;
                  current->right->parent = guard;
                }
                node* tmp = current;
                current = current->right;
                delete tmp;
            }
            else if(current->right == nullptr)
            {
                if(current == root)
                {
                  guard->left = current->left;
                  current->left->parent = guard;
                }
                node* tmp = current;
                current = current->left;
                delete tmp;
            }
            else
            {
                node* tmp = current;
                tmp = tmp->right;
                node* prevtmp = nullptr;

                while(tmp->left != nullptr)
                {
                    prevtmp = tmp;
                    tmp = tmp->left;
                }

                if(prevtmp != nullptr)
                {
                    prevtmp->left = tmp->right;
                }

                tmp->left = current->left;

                if(current->right != tmp)
                {
                    tmp->right = current->right;
                }
                if(current == root)
                {
                  guard->right = tmp;
                  tmp->parent = guard;
                }
                delete current;
                current = tmp;

            }
        }
        else if(current->value.first > number)
        {
            if(current->left != nullptr)
            {
                if(current->left->value.first == number)
                {
                    node* tmp = current;
                    if(tmp->left->left == nullptr && tmp->left->right == nullptr)
                    {
                        delete tmp->left;
                        tmp->left = nullptr;
                    }
                    else if(tmp->left->left == nullptr)
                    {
                        node* tmp2 = tmp->left->right;
                        delete tmp->left;
                        tmp->left = tmp2;
                    }
                    else if(tmp->left->right == nullptr)
                    {
                        node* tmp2 = tmp->left->left;
                        delete tmp->left;
                        tmp->left = tmp2;
                    }
                    else
                    {
                        node* last = tmp->left->right;
                        node* prevlast = nullptr;
                        while(last->left != nullptr)
                        {
                            prevlast = last;
                            last = last->left;
                        }

                        if(prevlast != nullptr)
                        {
                            prevlast->left = last->right;
                        }

                        last->left = tmp->left->left;

                        if(tmp->left->right != last)
                        {
                            last->right = tmp->left->right;
                        }
                        delete tmp->left;
                        tmp->left = last;
                    }
                }
                else if(current->left->value.first > number)
                {
                    node* tmp = current;
                    tmp = tmp->left;
                    remove(tmp,number);
                }
                else
                {
                    node* tmp = current;
                    tmp = tmp->left;
                    remove(tmp,number);
                }
            }
        }
        else if(current->value.first < number)
        {
            if(current->right != nullptr)
            {
                if(current->right->value.first == number)
                {
                    node* tmp = current;
                    if(tmp->right->left == nullptr && tmp->right->right == nullptr)
                    {
                        delete tmp->right;
                        tmp->right = nullptr;
                    }
                    else if(tmp->right->left == nullptr)
                    {
                        node* tmp2 = tmp->right->right;
                        delete tmp->right;
                        tmp->right = tmp2;
                    }
                    else if(tmp->right->right == nullptr)
                    {
                        node* tmp2 = tmp->right->left;
                        delete tmp->right;
                        tmp->right = tmp2;
                    }
                    else
                    {
                        node* last = tmp->right->right;
                        node* prevlast = nullptr;
                        while(last->left != nullptr)
                        {
                            prevlast = last;
                            last = last->left;
                        }

                        if(prevlast != nullptr)
                        {
                            prevlast->left = last->right;
                        }

                        last->left = tmp->right->left;

                        if(tmp->right->right != last)
                        {
                            last->right = tmp->right->right;
                        }
                        delete tmp->right;
                        tmp->right = last;
                    }
                }
                else if(current->right->value.first > number)
                {
                    node* tmp = current;
                    tmp = tmp->right;
                    remove(tmp,number);
                }
                else
                {
                    node* tmp = current;
                    tmp = tmp->right;
                    remove(tmp,number);
                }
            }
        }
    }
  }

  bool checkDiff(node* first, node* second) const //check trees for differences
  {
    bool foundDiff = false;
    if(first != nullptr && second != nullptr)
    {
      foundDiff = (first->value.first != second->value.first || first->value.second != second->value.second)
                  || checkDiff(first->left,second->left)
                  || checkDiff(first->right,second->right);
    }
    else if((first == nullptr && second != nullptr) || (first != nullptr && second == nullptr))
    {
      foundDiff = true;
    }
    return foundDiff;
  }

  void insert(node*& current,node* newNode) //inserts node into a tree
  {
    if(current == nullptr)
    {
      current = newNode;
      if(size == 0)
      {
        current->parent = guard;
        guard->left = current;
      }
      size++;
    }
    else
    {
      if(current->value.first > newNode->value.first)
      {
        if(current->left == nullptr)
        {
          current->left = newNode;
          newNode->parent = current;
          size++;
        }
        else
        {
          node* tmp = current->left;
          insert(tmp,newNode);
        }
      }
      else if(current->value.first < newNode->value.first)
      {
        if(current->right == nullptr)
        {
          current->right = newNode;
          newNode->parent = current;
          size++;
        }
        else
        {
          node* tmp = current;
          tmp = tmp->right;
          insert(tmp,newNode);
        }
      }
    }
  }

  void copy(node*& current,node* other, node* prev) //copies other tree to current tree
  {
    if(other != nullptr)
    {
      current = new node(other->value.first,other->value.second);
      current->parent = prev;
      if(size == 0)
      {
        guard->left = current;
      }

      size++;
      copy(current->left,other->left,current);
      copy(current->right,other->right,current);
    }
  }

  node* minVal(node* current) const //returns node with minimal key
  {
    if(current->left == nullptr)
    {
      return current;
    }
    else
    {
      return minVal(current->left);
    }
  }

  void addToVector(std::vector<value_type>& vec, node* current) const //add values from tree to vector (pre-order)
  {
    if(current != nullptr)
    {
      addToVector(vec, current->left);
      vec.push_back(current->value);
      addToVector(vec, current->right);
    }
  }

  bool areEqiuvalent(node* first, node*second) const
  {
    std::vector<value_type > firstVec;
    std::vector<value_type > secondVec;

    addToVector(firstVec,first);
    addToVector(secondVec,second);

    return firstVec == secondVec;
  }

public:
  TreeMap()
  {
      guard = new node();
      root = nullptr;
      size = 0;
  }

  TreeMap(std::initializer_list<value_type> list)
  {
    guard = new node();
    root = nullptr;
    size = 0;
    node* newNode;
    for(auto it = list.begin(); it < list.end(); ++it)
    {
      newNode = new node(it->first,it->second);
      insert(root,newNode);
    }
  }

  TreeMap(const TreeMap& other)
  {

    root = nullptr;
    size = 0;

    guard = new node();
    copy(root,other.root,guard);
  }

  TreeMap(TreeMap&& other) noexcept
  {
    guard = other.guard;
    root = other.root;
    size = other.size;

    other.guard = nullptr;
    other.root = nullptr;
    other.size = 0;
  }

  ~TreeMap()
  {
    destroy(root);
    delete guard;
    guard = nullptr;
    size = 0;

  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(*this == other)
      return *this;

    destroy(root);
    delete guard;
    size = 0;
    guard = new node();
    copy(root,other.root,guard);

    return *this;
  }

  TreeMap& operator=(TreeMap&& other) noexcept
  {
    if(*this == other)
      return *this;

    destroy(root);
    delete guard;

    root = other.root;
    guard = other.guard;
    size = other.size;

    other.root = nullptr;
    other.guard = nullptr;
    other.size = 0;

    return *this;
  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    node* target = lookFor(root,key);

    if(target == nullptr)
    {
      target = new node(key,mapped_type());
      insert(root,target);
    }

    return target->value.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    ConstIterator it = find(key);
    return it->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    Iterator it = find(key);
    return it->second;
  }

  const_iterator find(const key_type& key) const
  {
    node* target = lookFor(root,key);
    if(target == nullptr) target = guard;

    ConstIterator it = ConstIterator(const_cast<TreeMap *>(this), target);
    return it;
  }

  iterator find(const key_type& key)
  {
    node* target = lookFor(root,key);
    if(target == nullptr) target = guard;

    Iterator it = Iterator(this,target);
    return it;
  }

  void remove(const key_type& key)
  {
    if(lookFor(root,key) == nullptr)
      throw std::out_of_range("Element not in collection. Cannot remove.");

    remove(root,key);
    size--;
  }

  void remove(const const_iterator& it)
  {
    remove(root,it->first);
    size--;
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
  {
    if(size != other.size)
      return false;

    node* tmp = root;
    node* tmpOther = other.root;

    bool foundDifference = checkDiff(tmp,tmpOther);

    if(!foundDifference)
      return true;
    else
      return areEqiuvalent(tmp,tmpOther);
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    iterator it;

    if(size == 0)
      it = Iterator(this,guard);
    else
      it = Iterator(this,minVal(root));

    return it;
  }

  iterator end()
  {
    iterator it = Iterator(this,guard);
    return it;
  }

  const_iterator cbegin() const
  {
    const_iterator it;

    if(size == 0)
      it = ConstIterator(const_cast<TreeMap *>(this),guard);
    else
      it = ConstIterator(const_cast<TreeMap *>(this), minVal(root));

    return it;
  }

  const_iterator cend() const
  {
    const_iterator it = ConstIterator(const_cast<TreeMap *>(this), guard);
    return it;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

protected:
  TreeMap* collection;
  node* selectedNode;

  friend class TreeMap;

  node* successor()
  {
    node* tmp = nullptr;
    if(selectedNode->right != nullptr)
    {
      tmp = selectedNode->right;
      while(tmp->left != nullptr)
      {
        tmp = tmp->left;
      }
    }
    else
    {
      tmp = selectedNode->parent;
      while(tmp != nullptr && selectedNode == tmp->right)
      {
        selectedNode = tmp;
        tmp = tmp->parent;
      }
    }
    return tmp;
  }

  node* predecessor()
  {
    node* tmp = nullptr;
    if(selectedNode->left != nullptr)
    {
      tmp = selectedNode->left;
      while(tmp->left != nullptr)
      {
        tmp = tmp->left;
      }
    }
    else
    {
      tmp = selectedNode->parent;
      while(tmp != nullptr && selectedNode == tmp->left)
      {
        selectedNode = tmp;
        tmp = tmp->parent;
      }
    }
    return tmp;
  }

public:

  ConstIterator(): collection(nullptr), selectedNode(nullptr)
  {}

  explicit ConstIterator(TreeMap* tree,node* newNode)
  {
    collection = tree;
    selectedNode = newNode;
  }

  ConstIterator(const ConstIterator& other)
  {
    collection = other.collection;
    selectedNode = other.selectedNode;
  }

  ConstIterator& operator++()
  {
    node* tmp = successor();

    if(tmp == nullptr)
      throw std::out_of_range("Attempt to reach past last element!");

    selectedNode = tmp;

    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator org = *this;
    ++(*this);
    return org;
  }

  ConstIterator& operator--()
  {
    node* tmp = predecessor();

    if(tmp == nullptr)
      throw std::out_of_range("Attempt to reach before first element!");

    selectedNode = tmp;

    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator org = *this;
    --(*this);
    return org;
  }

  reference operator*() const
  {
    if(*this == collection->end())
      throw std::out_of_range("Attempt to dereference end iterator!");

    return selectedNode->value;
  }

  pointer operator->() const
  {
    if(*this == collection->end())
      throw std::out_of_range("Attempt to access field of end iterator!");

    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return collection == other.collection && selectedNode == other.selectedNode;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return collection != other.collection || selectedNode != other.selectedNode;
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

private:
  friend class TreeMap;

public:
  explicit Iterator(): ConstIterator()
  {}

  Iterator(TreeMap* tree, node* current): ConstIterator(tree, current)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_MAP_H */
