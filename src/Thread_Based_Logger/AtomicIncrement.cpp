#ifndef LOGGER_ATOMICINT_GCC_X86_64_CPP
#define LOGGER_ATOMICINT_GCC_X86_64_CPP
#include "AtomicIncrement.h"

atomic_t atomicIncrement(volatile atomic_t& val)
{
         asm __volatile__(
                "   lock       ;"
                "   addl %1,%0 ;"
                : "=m"  (val)
                : "ir"  (1), "m" (val)
            );
}

atomic_t atomicDecrement(volatile atomic_t& val)
{
            asm __volatile__(
                "   lock       ;"
                "   addl %1,%0 ;"
                : "=m"  (val)
                : "ir"  (-1), "m" (val)
            );
}

atomic_t atomicGet(volatile atomic_t& val)
{
    asm volatile ("mfence" : : : "memory");
    return val;
}

atomic_t atomicSet(volatile atomic_t& val, atomic_t new_val)
{
    val=new_val;
    asm volatile ("mfence" : : : "memory");
}

 #endif

