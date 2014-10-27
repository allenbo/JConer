#include <iostream>
#include "jconer/allocator.hpp"

Allocator alloc;

void* operator new(size_t size) throw (std::bad_alloc) {
  void* ptr = alloc.malloc(size);
  if (ptr == NULL) {
    throw std::bad_alloc();
  }
  return ptr;
}

void operator delete(void* ptr) throw() {
}

