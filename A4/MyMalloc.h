#pragma include once

#include <cstdlib>
#include "HashTable.h"

class Allocator {
public:
    MyHashTable alloc_table_;

    // constructors
    Allocator();
    ~Allocator() = default;

    // methods
    void* allocate(size_t bytesToAllocate);
    void deallocate(void* ptr);
};