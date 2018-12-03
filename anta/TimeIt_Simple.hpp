#pragma once
#include <cstdio>

#ifdef WIN32
extern "C" int __stdcall QueryPerformanceFrequency(long long*);
extern "C" int __stdcall QueryPerformanceCounter(long long*);
double getTime() {
	long long c, freq;
	QueryPerformanceCounter(&c);
	QueryPerformanceFrequency(&freq);
	return c * 1. / freq;
}
#else
#include <sys/time.h>
double getTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1e6;
}
#endif

struct TimeIt {
	double c1;
	const char *msg;

	TimeIt(const char *msg_): msg(msg_) {
		c1 = getTime();
	}
	~TimeIt() {
		double c2 = getTime();
		fprintf(stderr, "%s: %.6f sec.\n", msg, c2 - c1);
	}

	operator bool() { return true; }	//if(TimeIt _ = "msg")Ç≈êÈåæÇ∑ÇÈóp
};
#define TIMEIT(s) if(TimeIt timeit_##__LINE__ = s)
