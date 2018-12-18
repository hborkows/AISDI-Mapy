#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>
#include <functional>

namespace aisdi
{

  const size_t capacity = 65537;

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair< key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  std::list<value_type > hashTable[capacity + 1];
  size_type size;

  void insert(value_type val)
  {
    size_type destination = std::hash<key_type >()(val.first) % capacity;

    hashTable[destination].push_back(val);
    size++;
  }

  void removeAll()
  {
    for(size_type i = 0; i < capacity; i++)
    {
      hashTable[i].clear();
    }
    size = 0;
  }

public:
  HashMap() : size(0)
  {
    hashTable[capacity].push_back(value_type(key_type(), mapped_type()));
  }

  HashMap(std::initializer_list<value_type> list)
  {
    size = 0;
    hashTable[capacity].push_back(value_type(key_type(), mapped_type()));

    for(auto it = list.begin(); it != list.end(); ++it)
    {
      insert(*it);
    }
  }

  HashMap(const HashMap& other)
  {
    size = 0;
    hashTable[capacity].push_back(value_type(key_type(), mapped_type()));

    for(auto it = other.cbegin(); it != other.cend(); ++it)
    {
      insert(*it);
    }
  }

  HashMap(HashMap&& other) noexcept
  {
    size = other.size;
    other.size = 0;

    for(size_type i = 0; i <= capacity; i++)
    {
      hashTable[i] = std::move(other.hashTable[i]);
      other.hashTable[i].clear();
    }
  }

  HashMap& operator=(const HashMap& other)
  {
    if(*this == other)
      return *this;

    removeAll();

    for(auto it = other.cbegin(); it != other.cend(); ++it)
    {
      insert(*it);
    }

    return *this;
  }

  HashMap& operator=(HashMap&& other) noexcept
  {
    if(*this == other)
      return *this;

    removeAll();

    size = other.size;
    other.size = 0;

    for(size_type i = 0; i < capacity; i++)
    {
      hashTable[i] = std::move(other.hashTable[i]);
      other.hashTable[i].clear();
    }

    return *this;
  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    iterator it = find(key);

    if(it == end())
    {
      insert(value_type(key, mapped_type()));
      it = find(key);
    }

      return (*it).second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    const_iterator it = find(key);

    if(it == cend())
      throw std::out_of_range("Key not found!");
    else
      return (*it).second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    iterator it = find(key);

    if(it == end())
      throw std::out_of_range("Key not found!");
    else
      return (*it).second;
  }

  const_iterator find(const key_type& key) const
  {
    size_type hashKey = std::hash<key_type >()(key) % capacity;
    typename std::list<value_type >::const_iterator it = hashTable[hashKey].begin();

    while((*it).first != key && it != hashTable[hashKey].end())
    {
      it++;
    }

    if((*it).first != key || it == hashTable[hashKey].end())
    {
      return cend();
    }
    else
    {
      const_iterator result = ConstIterator(const_cast<HashMap *>(this), hashKey, it);
      return result;
    }
  }

  iterator find(const key_type& key)
  {
    size_type hashKey = std::hash<key_type >()(key) % capacity;
    typename std::list<value_type >::iterator it = hashTable[hashKey].begin();

    while((*it).first != key && it != hashTable[hashKey].end())
    {
      it++;
    }

    if((*it).first != key || it == hashTable[hashKey].end())
    {
      return end();
    }
    else
    {
      iterator result = Iterator(this, hashKey, it);
      return result;
    }
  }

  void remove(const key_type& key)
  {
    size_type hashKey = std::hash<key_type >()(key) % capacity;
    typename std::list<value_type >::iterator it = hashTable[hashKey].begin();

    while((*it).first != key && it != hashTable[hashKey].end())
    {
      it++;
    }

    if((*it).first == key)
    {
      hashTable[hashKey].erase(it);
      size--;
    }
    else
    {
      throw std::out_of_range("Attempt to remove by wrong key!");
    }
  }

  void remove(const const_iterator& it)
  {
    if(it == cend())
      throw std::out_of_range("Attempt to remove end iterator!");

    if(it.it != hashTable[it.index].cend())
    {
      hashTable[it.index].erase(it.it);
      size--;
    }
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    bool foundDif = size != other.getSize();

    for(auto iter = other.cbegin(); iter != other.cend() && !foundDif; ++iter)
    {
      auto found = find(iter->first);

      if(found == cend() || found->second != iter->second)
        foundDif = true;
    }

    return !foundDif;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    typename std::list<value_type >::iterator bIt;
    size_type i = 0;
    iterator it;

    while(hashTable[i].empty() && i < capacity)
    {
      i++;
    }

    bIt = hashTable[i].begin();

    it = Iterator(this, i, bIt);

    return it;
  }

  iterator end()
  {
    typename std::list<value_type >::iterator bIt;
    iterator it;

    bIt = hashTable[capacity].begin();
    it = Iterator(this, capacity, bIt);

    return it;
  }

  const_iterator cbegin() const
  {
    typename std::list<value_type >::const_iterator bIt;
    size_type i = 0;
    const_iterator it;

    while(hashTable[i].empty() && i < capacity)
    {
      i++;
    }

    bIt = hashTable[i].begin();

    it = ConstIterator(const_cast<HashMap *>(this), i, bIt);

    return it;
  }

  const_iterator cend() const
  {
    typename std::list<value_type >::const_iterator eIt;
    const_iterator it;

    eIt = hashTable[capacity].begin();
    it = ConstIterator(const_cast<HashMap *>(this), capacity, eIt);

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
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

protected:
  friend class HashMap<key_type ,mapped_type >;

  HashMap<key_type , mapped_type >* collection;
  size_type index;
  typename std::list<value_type >::const_iterator it;

public:
  explicit ConstIterator(HashMap<key_type , mapped_type >* coll, size_type newIndex, typename std::list<value_type>::const_iterator iter)
  {
    collection = coll;
    index = newIndex;
    it = iter;
  }



  ConstIterator() : it()
  {
    collection = nullptr;
    index = 0;
  }

  ConstIterator(const ConstIterator& other)
  {
    collection = other.collection;
    index = other.index;
    it = other.it;
  }

  ConstIterator& operator++()
  {
    if(*this == collection->cend())
      throw std::out_of_range("Attempt to reach past end iterator!");

    typename std::list<value_type >::const_iterator last = --(collection->hashTable[index].cend());

    if(it != last)
    {
      it++;
      return *this;
    }

    for(size_type i = ++index; i < capacity; i++)
    {
      if(!(collection->hashTable[i].empty()))
      {
        index = i;
        it = collection->hashTable[i].cbegin();
        return *this;
      }
    }

    index = capacity;
    it = collection->hashTable[capacity].cbegin();
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
    if(*this == collection->cbegin())
      throw std::out_of_range("");

    if(it != collection->hashTable[index].cbegin())
    {
      it--;
      return *this;
    }

    for(int i = --index; i > 0; i--)
    {
      if(!(collection->hashTable[i].empty()))
      {
        index = i;
        it = --(collection->hashTable[i].cend());
        return *this;
      }
    }

    index = 0;
    it = --(collection->hashTable[0].cend());
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

    return *it;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return collection == other.collection && index == other.index && it == other.it;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator(HashMap<key_type , mapped_type >* coll, size_type newIndex, typename std::list<value_type>::iterator iter)
          : ConstIterator(coll, newIndex, iter)
  {}

  Iterator() : ConstIterator()
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

#endif /* AISDI_MAPS_HASHMAP_H */
