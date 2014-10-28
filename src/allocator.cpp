#include <iostream>
#include "jconer/allocator.hpp"

Allocator alloc;

void* allocate(size_t size) throw (std::bad_alloc) {
  void* ptr = alloc.malloc(size);
  if (ptr == NULL) {
    throw std::bad_alloc();
  }
  return ptr;
}
