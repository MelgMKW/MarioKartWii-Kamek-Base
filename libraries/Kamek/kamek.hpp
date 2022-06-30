/*
 * Kamek Standard Library
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

#define abs(n) __abs(n)


#ifndef __KAMEK_H
#define __KAMEK_H

/*#ifndef __MWERKS__
#error "Kamek requires the CodeWarrior compiler!"
#endif*/

#define NULL 0

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;
typedef volatile s64 vs64;
typedef volatile s32 vs32;
typedef volatile s16 vs16;
typedef volatile s8 vs8;
typedef volatile u64 vu64;
typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u8 vu8;
typedef volatile f64 vf64;
typedef volatile f32 vf32;
typedef int UnkType;
typedef unsigned long size_t;

#define NODE_TO_ITEM(node, type, field) \
    ((type *)((char *)node - offsetof(type, field)))
    
#define offsetof(st, m) \
    ((u32)&(((st *)0)->m))

struct ptmf{
    int this_delta;
    int vtableOffset;
    void *functionPointer;
};//total size 0xc

struct ptmfHolder{
    u32 vtable;
    void* subject; //Pointer to the class instance, most times the class inheriting from PushButton (such as the class for the buttons of the selectionScreen)
    ptmf ptmf;
};//total size 0x14

template <typename Ret>
struct PtmfHolderBase_0A {
    virtual Ret operator()() = 0;
};

template <class Subject, typename Ret>
struct PtmfHolder_0A : PtmfHolderBase_0A<Ret> {
    Subject * subject;
    Ret (Subject::*ptmf)();
    virtual Ret operator()() {
        return (subject->*ptmf)();
    }
};

template <typename Ret, typename A1>
struct PtmfHolderBase_1A {
    virtual Ret operator()(A1 a1) = 0;
};

template <class Subject, typename Ret, typename A1>
struct PtmfHolder_1A : PtmfHolderBase_1A<Ret, A1> {
    Subject * subject;
    Ret (Subject::*ptmf)(A1);
    virtual Ret operator()(A1 a1) {
        return (subject->*ptmf)(a1);
    }
};

template <typename Ret, typename A1, typename A2>
struct PtmfHolderBase_2A {
    virtual Ret operator()(A1 a1, A2 a2) = 0;
};
template <class Subject, typename Ret, typename A1, typename A2>
struct PtmfHolder_2A : PtmfHolderBase_2A<Ret, A1, A2> {
    Subject * subject;
    Ret (Subject::*ptmf)(A1, A2);
    virtual Ret operator()(A1 a1, A2 a2) {
        return (subject->*ptmf)(a1, a2);
    }
};

template <typename Ret, typename A1, typename A2, typename A3>
struct PtmfHolderBase_3A {
    virtual Ret operator()(A1 a1, A2 a2, A3 a3) = 0;
};
template <class Subject, typename Ret, typename A1, typename A2, typename A3>
struct PtmfHolder_3A : PtmfHolderBase_3A<Ret, A1, A2, A3> {
    Subject * subject;
    Ret (Subject::*ptmf)(A1, A2, A3);
    virtual Ret operator()(A1 a1, A2 a2, A3 a3) {
        return (subject->*ptmf)(a1, a2, a3);
    }
};

#ifdef __INTELLISENSE__
#define ASM(...)
#define asm
#define __attribute(...)
#define __sync(...)
#define __isync(...)
#else
#define ASM(...) __VA_ARGS__
#endif

#include "base/hooks.hpp"
#include "base/c_stdlib.hpp"
#include "base/rvl_sdk.hpp"
#include "base/math.hpp"

#endif

