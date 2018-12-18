#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{


  template <typename K, typename V>
  using HashMap = aisdi::HashMap<K, V>;
  template <typename K, typename V>
  using TreeMap = aisdi::TreeMap<K,V>;
  using time_type = std::chrono::time_point<std::chrono::system_clock>;
  using duration_type = std::chrono::duration<double>;

  void performTreeMapTest(size_t n)
  {
    time_type start, end;
    duration_type timeElapsed;
    TreeMap<size_t,std::string> collection;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(n,n);
    size_t index;

    std::cout << "TreeMap tests: " << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;


    //appending
    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < n; i++)
    {
      index = static_cast<size_t >(distribution(generator));
      if(collection.find(index) == collection.cend())
      {
        collection[index] = "Funny element name";
      }
    }
    size_t size = collection.getSize();
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Adding (plus find time)" << size << " elements takes: " << timeElapsed.count() << "s" << std::endl;


    //searching
    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < n; i++)
    {
      index = static_cast<size_t >(distribution(generator));
      collection.find(index);
    }
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Searching for " << n << " elements takes: " << timeElapsed.count() << "s" << std::endl;


    //removing
    start = std::chrono::system_clock::now();
    for(auto it = collection.begin(); it != collection.end(); ++it)
    {
      collection.remove(it);
    }
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Removing " << size << " elements takes: " << timeElapsed.count() << "s" << std::endl;

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
  }

  void performHashMapTest(size_t n)
  {
    time_type start, end;
    duration_type timeElapsed;
    HashMap<size_t,std::string> collection;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(n,n);
    size_t index;

    std::cout << "HashMap tests: " << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;

    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < n; i++)
    {
      index = static_cast<size_t >(distribution(generator));
      if(collection.find(index) == collection.cend())
      {
        collection[index] = "Another funny element name";
      }
    }
    size_t size = collection.getSize();
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Adding (plus find time)" << size << " elements takes: " << timeElapsed.count() << "s" << std::endl;

    start = std::chrono::system_clock::now();
    for(size_t i = 0; i < n; i++)
    {
      index = static_cast<size_t >(distribution(generator));
      collection.find(index);
    }
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Searching for " << n << " elements takes: " << timeElapsed.count() << "s" << std::endl;

    start = std::chrono::system_clock::now();
    auto it = collection.begin();
    while(it != collection.end())
    {
      collection.remove(it++);
    }
    end = std::chrono::system_clock::now();
    timeElapsed = end - start;
    std::cout << "Removing " << size << " elements takes: " << timeElapsed.count() << "s" << std::endl;

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
  }

  void perfomTest(size_t n)
  {
    performTreeMapTest(n);
    performHashMapTest(n);
  }

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 500000;
  perfomTest(repeatCount);
  return 0;
}
