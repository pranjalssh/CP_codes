#pragma once
#include <iostream>
#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef NO_MYASSERT
static inline void my_assert(bool) { }
#else
__declspec(noinline) static void my_assert(bool b) {
	if(b) return;
	std::cerr << "\nassertion error!!!" << std::endl;
	std::cerr << std::endl;
#ifdef _MSC_VER
	__debugbreak();
#endif
}
#endif

