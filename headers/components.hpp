#pragma once

#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <strings.h>
#include <vector>
extern int s_component_counter = 0;
const int MAX_ENTITIES = 5000000;
// -------------> Components -------------->

// -------------> Scene Class ->
// typedef unsigned long long EntityID;
template <class T> int GetId() {
  static int s_componentId = s_component_counter++;
  return s_componentId;
}

class IComponent {};

struct Transform {
  int pos;
};

struct Shape {
  int color;
}; // -----------> Component Pool
struct ComponentPool {
  ComponentPool(size_t elementSize) {
    elementsize = elementSize;
    pData = new char[elementsize * MAX_ENTITIES];
  }

  ~ComponentPool() { delete[] pData; }

  inline void *get(size_t index) {
    // looking up the Components

    // std::cout << "Index: " << index << ", Offset: " << elementsize <<
    // std::endl;
    return pData + index * elementsize;
  }

  // Function to add data to the component pool
  inline void addData(size_t index, const void *data) {
    if (index < MAX_ENTITIES) {
      std::memcpy(pData + index * elementsize, data, elementsize);
    } else {
      std::cerr << "Error: Index out of bounds." << std::endl;
    }
  }

  char *pData{nullptr};
  size_t elementsize{0};
};
//
// template <typename T> struct ComponentPool {
//   T *get(int index) { return &pool.at(index); };
//   void *set(int index, T value) { &pool.insert(index, value); }
//   std::map<int, T> pool;
// };
//
// -------------> Componenets ------------->
