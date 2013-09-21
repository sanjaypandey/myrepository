#ifndef LOGGER_ATOMICINT_GCC_X86_64_H
#define LOGGER_ATOMICINT_GCC_X86_64_H

#include <unistd.h>

typedef int atomic_t;
atomic_t atomicIncrement(volatile atomic_t& val);
atomic_t atomicDecrement(volatile atomic_t& val);
atomic_t atomicGet(volatile atomic_t& val);
atomic_t atomicSet(volatile atomic_t& val, atomic_t new_val);


#endif
