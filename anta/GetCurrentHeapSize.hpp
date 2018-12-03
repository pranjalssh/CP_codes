#pragma once
#include <cstdint>

struct PROCESS_HEAP_ENTRY {
	void *lpData; uint32_t cbData;
	uint8_t cbOverhead; uint8_t iRegionIndex;
	uint16_t wFlags;
	union {
		struct {
			void *hMem;
			uint32_t dwReserved[3];
		} Block;
		struct {
			uint32_t dwCommittedSize, dwUnCommittedSize;
			void *lpFirstBlock, *lpLastBlock;
		} Region;
	};
};
extern "C" {
	uint32_t __stdcall GetProcessHeaps(uint32_t, void**);
	int __stdcall HeapWalk(void*, PROCESS_HEAP_ENTRY*);
};


static long long getCurrentHeapSize() {
	uint32_t numberOfHeaps = GetProcessHeaps(0, NULL);
	void** heaps = new void*[numberOfHeaps];
	GetProcessHeaps(numberOfHeaps, heaps);
	PROCESS_HEAP_ENTRY heapEntry;
	long long sizeSum = 0;
	for(uint32_t i = 0; i < numberOfHeaps; i ++) {
		void *heap = heaps[i];
		heapEntry.lpData = NULL;
		while(HeapWalk(heap, &heapEntry)) {
			uint32_t size = heapEntry.cbData;
			sizeSum += size;
		}
	}
	delete[] heaps;
	return sizeSum;
}
