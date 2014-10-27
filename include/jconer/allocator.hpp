#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__
#include <stdio.h>
#include "jconer/arena.hpp"

class Allocator {
  public:
    Allocator() {
      arena_ =  arena_New();
      if (arena_ == NULL) {
        throw std::bad_alloc();
      }
    }
    virtual ~Allocator() {
      arena_Free(arena_);
    }
    
    void * malloc(size_t size) {
      return arena_Malloc(arena_, size);
    }
  private:
    Arena* arena_;
};


void* operator new(size_t size) throw (std::bad_alloc);

void operator delete(void* ptr) throw();

#endif
