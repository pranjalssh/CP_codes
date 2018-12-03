#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <random>
#include <stdint.h>
#include <functional>
#include <algorithm>
#include <utility>
#include "../Util/MyAssert.hpp"


//Yuan, Hao, and Mikhail J. Atallah. "Data structures for range minimum queries in multidimensional arrays." Proceedings of the Twenty-First Annual ACM-SIAM Symposium on Discrete Algorithms. Society for Industrial and Applied Mathematics, 2010.
//<http://yuanresearch.com/rmq_highdim.pdf>
//BlockSize = 5..15 で lg blockTypes = 5,6,8,9,12,13,15,16,19,20,22
//
//TODO: 多次元は諦めて2次元版を実装する

//template<typename Val, typename Compare = std::less<Val>, int BlockSize = 10>
class YuanAtallahRMQ {
public:
	static const int D = 2; static const int PowD = 1 << D;
	typedef int Val; typedef std::less<Val> Compare; //static const int BlockSize = 10;

	typedef int Index; typedef char InBlockIndex; typedef unsigned BlockType;

	YuanAtallahRMQ(Compare comp_ = Compare()): comp(comp_) {
//		blockHeight = getTreeHeight(BlockSize);
//		blockTypes = BlockType(1) << calculateMaximumTypeSize(BlockSize);

//		resetBlockTables();
	}
	
	void build(const Val *a, Index ns[D]) {
		for(int d = 0; d < D; d ++) {
			blocks[d] = ns[d]; //(ns[d] + BlockSize - 1) / BlockSize;
			treeHeight[d] = getTreeHeight(blocks[d]);
		}

//		buildBlocksAll(a, ns);
		buildBlockTreesAll(a);
	}

	//[l,r]の閉区間
	Index query(const Val *a, Index l, Index r) const {
		/*
		Index x = l / BlockSize, y = r / BlockSize, z = y - x;
		if(z == 0) return queryBlock(a, x, l % BlockSize, r % BlockSize);
		Index edge = minIndex(a,
			queryBlock(a, x, l % BlockSize, BlockSize - 1),
			queryBlock(a, y, 0, r % BlockSize));
		if(z == 1) return edge;
		if(z == 2) return minIndex(a, queryBlock(a, x+1, 0, BlockSize-1), edge);
		++ x, -- y;
		Index h = (Index)(getLCAHeight(x, y) - 1) * blocks;
		Index left = leftMin[h + y], right = rightMin[h + x];
		return minIndex(a, minIndex(a, left, right), edge);
		*/
		return -1;
	}
	Val queryVal(const Val *a, Index l, Index r) const {
		return a[query(a, l, r)];
	}

private:
	Compare comp;

//	typedef InBlockIndex (BlockTable)[BlockSize][BlockSize];
//	int blockHeight; BlockType blockTypes;
//	std::unique_ptr<BlockTable[]> minCandidates[PowD];

	Index blocks[D];
	int treeHeight[D];

//	std::unique_ptr<BlockType[]> blockTypeLeft, blockTypeRight;
	std::unique_ptr<Index[]> treeMins[PowD];
	
	template<typename T> struct RefLeft {
		const T left;
		RefLeft(T a, Index n): left(a) { }
		inline T ref(Index i) const { return left + i; }
		Index getOffset(T p) const { return p - left; }
		inline static Index queryCandidate(Index l, Index r) { return r; }
	};
	
	template<typename T> struct RefRight {
		const T right;
		RefRight(T a, Index n): right(a + n - 1) { }
		inline T ref(Index i) const { return right - i; }
		Index getOffset(T p) const { return right - p + 1; }
		inline static Index queryCandidate(Index l, Index r) { return l; }
	};

	/*
	void buildBlocksAll(const Val *a, Index ns[D]) {
		std::unique_ptr<InBlockIndex[]> tmpTree(new InBlockIndex[BlockSize * blockHeight]);
		std::unique_ptr<Val[]> tmpValues(new Val[BlockSize]);

		blockTypeLeft.reset(new BlockType[blocks]);
		buildBlocks<RefLeft>(a, n, minCandidateLeft.get(), blockTypeLeft.get(), tmpTree.get(), tmpValues.get());

		blockTypeRight.reset(new BlockType[blocks]);
		buildBlocks<RefRight>(a, n, minCandidateRight.get(), blockTypeRight.get(), tmpTree.get(), tmpValues.get());
	}

	void resetBlockTables() {
		for(int k = 0; k < PowD; k ++)
			resetBlockTable(minCandidates[k]);
	}

	void resetBlockTable(std::unique_ptr<BlockTable[]> &p) {
		const BlockType types = blockTypes;
		p.reset(new BlockTable[types]);
		BlockTable *q = p.get();
		for(BlockType i = 0; i < types; i ++)
			q[i][0][0] = -1;	//未初期化であるとしてマークする
	}
	
	template<template<typename T> class Ref>
	void buildBlocks(const Val *a, Index n, BlockTable blockTable[], BlockType blockTypeTable[], InBlockIndex tmpTree[], Val tmpValues[]) {
		for(Index i = 0; i < blocks; i ++) {
			const Val *block = getBlock(a, n, i, tmpValues);
			blockTypeTable[i] = buildBlock<Ref>(block, blockTable, tmpTree);
		}
	}

	//端っこのブロック用。コピコンとか要求するのがあれなので、配列に余分なBlockSizeの領域を用意しておくことで回避も可能
	const Val *getBlock(const Val *a, Index n, Index i, Val tmpValues[]) const {
		Index offset = i * BlockSize;
		if(offset + BlockSize <= n)
			return a + offset;
		for(Index j = offset; j < n; j ++)
			tmpValues[j - offset] = a[j];
		Val maxVal = a[offset];
		for(Index j = offset+1; j < n; j ++)
			if(comp(maxVal, a[j])) maxVal = a[j];
		std::fill(tmpValues + (n - offset), tmpValues + BlockSize, maxVal);
		return tmpValues;
	}

	template<template<typename T> class Ref>
	BlockType buildBlock(const Val *a, BlockTable blockTable[], InBlockIndex tmpTree[]) {
		for(int i = 0; i < BlockSize; i ++)
			tmpTree[i] = i;
		BlockType blockType = buildTree<Ref>(a, BlockSize, blockHeight, tmpTree, true);

		BlockTable &entry = blockTable[blockType];
		bool computed = entry[0][0] != -1;
//		if(entry[0][0] != -1) return blockType;

		for(int l = 0; l < BlockSize; l ++) for(int r = l; r < BlockSize; r ++) {
			InBlockIndex t = entry[l][r];
			if(l == r) {
				entry[l][r] = tmpTree[l];
			}else {
				int h = (getLCAHeight(l, r) - 1) * BlockSize;
				entry[l][r] = tmpTree[h + Ref<Index>::queryCandidate(l, r)];
			}
			if(computed) my_assert(t == entry[l][r]);
		}
		return blockType;
	}
	*/

	void buildBlockTreesAll(const Val *a) {
		leftMin.reset(new Index[blocks * treeHeight]);
		buildBlockTree<RefLeft>(a, leftMin.get());

		rightMin.reset(new Index[blocks * treeHeight]);
		buildBlockTree<RefRight>(a, rightMin.get());
	}

//	template<template<typename T> class Ref>
	void buildBlockTree(const Val *a, Index mins[]) const {
		Index n = blocks;
		for(Index i = 0; i < n; i ++)
			mins[i] = queryBlock(a, i, 0, BlockSize-1);
		buildTree<Ref>(a, n, treeHeight, mins, false);
	}

//	template<template<typename T> class Ref, typename LocalIndex>
	typedef InBlockIndex LocalIndex;
	//1. トポロジカル順序でブロックをイテレートする

	BlockType buildTree(const Val *a, Index ns[D], int heights[D], LocalIndex mins[], bool getType) const {
		Index N = 1;
		for(int d = 0; d < D; d ++) N *= ns[d];
		int maxSizeSum[D+1];
		maxSizeSum[D] = 0;
		for(int d = D-1; d >= 0; d --)
			maxSizeSum[d] = (heights[d]-1) + maxSizeSum[d+1];
		int sizes[D], remsizes[D+1];

		std::vector<int> stk;
		//総和がsizekになる長さDの組み合わせを求めたい。dfsで
		stk.push_back(0);
		sizes[0] = -1; remsizes[0] = 1;
		while(!stk.empty()) {
			{	int di = stk.back(); stk.pop_back();
				if(di != D) {
					if(sizes[di] == -1)
						sizes[di] = std::max(0, remsizes[di] - maxSizeSum[di+1]);
					else
						sizes[di] ++;
					if(sizes[di] > remsizes[di]) {
						sizes[di] = -1;
						if(di == 0 && remsizes[0]+1 <= maxSizeSum[0]) {
							remsizes[0] ++;
							stk.push_back(0);
						}
					}else {
						remsizes[di+1] = remsizes[di] - sizes[di];
						stk.push_back(di);
						stk.push_back(di + 1);
					}
					continue;
				}
			}
			//sizes[0..D-1]がサイズ
			for(int d = 0; d < D; d ++)
				my_assert(sizes[d] < heights[d]);
			my_assert(remsizes[D] == 0);

			int longestD = 0;
			for(int d = 0; d < D; d ++)
				if(sizes[d] > sizes[longestD]) longestD = d;
			Index sizeIndex = 0, prevSizeIndex = 0;
			for(int d = 0; d < D; d ++) {
				sizeIndex = sizeIndex * heights[d] + sizes[d];
				prevSizeIndex = prevSizeIndex * heights[d] + (sizes[d] - (d == longestD));
			}
			LocalIndex *cnt = mins + sizeIndex * N, *prev = mins + prevSizeIndex * N;

		}
	}

	/*
	BlockType buildTree(const Val *a, Index n, int height, LocalIndex mins[], bool getType) const {
		LocalIndex *prev = mins, *cnt = mins + n;
		Index comparisons = 0; BlockType blockType = 0;

		for(int i = 1; i < height; i ++) {
			Index pm = (Index)1 << (i-1), m = pm << 1;
			for(Index j = 0; j < n; j += m) {
				Index nodeSize = std::min((Index)(n - j), m);
				Ref<const Val*> rval(a + j, nodeSize);
				Ref<LocalIndex *> rcnt(cnt + j, nodeSize), rprev(prev + j, nodeSize);
				Index leftSize = rval.getOffset(a + std::min(n, (Index)(j + pm)));

				for(Index k = 0; k < leftSize; k ++)
					*rcnt.ref(k) = *rprev.ref(k);
				if(leftSize == nodeSize) continue;
				if(leftSize == 0) {
					for(Index k = 0; k < nodeSize; k ++)
						*rcnt.ref(k) = *rprev.ref(k);
					continue;
				}
				Index l = leftSize, r = nodeSize;
				LocalIndex leftMin = *rprev.ref(leftSize - 1);
				while(r - l > 0) {
					Index mid = l + (r - l) / 2;
					Index x = *rprev.ref(mid);
					if(comp(a[x], a[leftMin]) || (!comp(a[leftMin], a[x]) && x < leftMin)) {
						r = mid;
						if(getType) blockType |= BlockType(1) << comparisons;
					}else {
						l = mid+1;
					}
					comparisons ++;
				}
				for(Index k = leftSize; k < l; k ++)
					*rcnt.ref(k) = leftMin;
				for(Index k = l; k < nodeSize; k ++)
					*rcnt.ref(k) = *rprev.ref(k);
			}
			prev = cnt; cnt += n;
		}

		if(getType) blockType |= BlockType(1) << comparisons;
		return blockType;
	}
	*/

	/*
	Index queryBlock(const Val *a, Index i, InBlockIndex l, InBlockIndex r) const {
		Index base = i * BlockSize;
		Index cl = base + minCandidateLeft[blockTypeLeft[i]][l][r];
		Index cr = base + minCandidateRight[blockTypeRight[i]][l][r];
		return minIndex(a, cl, cr);
	}
	*/

	inline Index minIndex(const Val *a, Index l, Index r) const {
		return comp(a[l], a[r]) || (!comp(a[r], a[l]) && l < r) ? l : r;
	}

	static inline int getLCAHeight(Index l, Index r) {
		Index x = l ^ r;
		int k = 0;
		if(x & 0xffff0000) x >>= 16, k += 16;
		if(x & 0x0000ff00) x >>= 8,  k += 8;
		if(x & 0x000000f0) x >>= 4,  k += 4;
		if(x & 0x0000000c) x >>= 2,  k += 2;
		return k + (x >> 1) + (x != 0);
	}

	static inline int getTreeHeight(Index n) {
		int x = 1;
		while((1 << x) < n) x ++;
		return x;
	}

	//とりあえずgreedyなシミュ
	static int calculateMaximumTypeSize(int n) {
		int h = 1; while(1 << h < n) ++ h;
		int res = 0;
		for(int i = 1; i < h; i ++) {
			Index pm = (Index)1 << (i-1), m = pm << 1;
			for(Index j = 0; j < n; j += m) {
				Index nodeSize = std::min((Index)(n - j), m);
				Index leftSize = std::min((Index)(n - j), (Index)pm);
				if(leftSize == nodeSize) continue;
				if(leftSize == 0) continue;
				int l = std::min(leftSize, nodeSize - leftSize), r = nodeSize;
				while(r - l > 0) {
					Index mid = l + (r - l) / 2;
					if(mid - l > r - (mid+1)) {
						r = mid;
					}else {
						l = mid+1;
					}
					res ++;
				}
			}
		}
		res ++;
		return res;
	}
};

//#include "DirectRMQ.hpp"
#include "../Util/MyAssert.hpp"
#include "../Util/TimeIt_Simple.hpp"
#include "../Util/GetCurrentHeapSize.hpp"

int main() {
#if 0
	int a[] = {0,0,0}; int n = sizeof(a) / sizeof(*a);
	YuanAtallahRMQ<int,std::less<int>,3> rmq;
	rmq.build(a, n);
	for(int i = 0; i < n; i ++) for(int j = i+1; j < n; j ++) {
		int x = rmq.query(a, i, j);
		std::cout << "[" << i << ", " << j << "]" << ": a[" << x << "] = " << a[x] << std::endl;
	}
//	for(int i = 1; i <= 20; i ++)
//		std::cout << i << ": " << rmq.calculateMaximumTypeSize(i) << std::endl;
#endif
#if 0
	void testRMQs();
	void performanceTests();
//	testRMQs();
	performanceTests();
#endif
	return 0;
}

#if 0
template<typename Val, int BlockSize>
void performanceTest(int N, int Q, std::default_random_engine &re, int cases);

void performanceTests() {
	std::default_random_engine re;
//	performanceTest<char,10>(100000000, 10000000, re, 0);

	performanceTest<int,6>(100000, 10000000, re, 0);
	performanceTest<int,6>(1000000, 10000000, re, 0);
	performanceTest<int,6>(10000000, 10000000, re, 0);
	performanceTest<int,8>(100000, 10000000, re, 0);
	performanceTest<int,8>(1000000, 10000000, re, 0);
	performanceTest<int,8>(10000000, 10000000, re, 0);
	performanceTest<int,10>(100000, 10000000, re, 0);
	performanceTest<int,10>(1000000, 10000000, re, 0);
	performanceTest<int,10>(10000000, 10000000, re, 0);
	performanceTest<int,12>(100000, 10000000, re, 0);
	performanceTest<int,12>(1000000, 10000000, re, 0);
	performanceTest<int,12>(10000000, 10000000, re, 0);
	performanceTest<int,14>(100000, 10000000, re, 0);
	performanceTest<int,14>(1000000, 10000000, re, 0);
	performanceTest<int,14>(10000000, 10000000, re, 0);

}

template<typename Val, int BlockSize>
void performanceTest(int N, int Q, std::default_random_engine &re, int cases) {
	std::unique_ptr<Val[]> a(new Val[N]);
	std::unique_ptr<int[][2]> queries(new int[Q][2]);
	long long baseHeapSize = getCurrentHeapSize();

	typedef YuanAtallahRMQ<Val, std::less<Val>, BlockSize> RMQ;
	std::unique_ptr<RMQ> rmq;
	std::cout << "YuanAtallahRMQ<" << typeid(Val).name() << "," << BlockSize << "> "
		<< "N: " << N << ", " << "Q: " << Q << " #" << cases << ":" << std::endl;
	TIMEIT("construct") rmq.reset(new RMQ());
	std::uniform_int_distribution<Val> rng_val;
	for(int i = 0; i < N; i ++)
		a[i] = rng_val(re);
	std::uniform_int_distribution<int> rng_index(0, N-1);
	for(int i = 0; i < Q; i ++) {
		int l = rng_index(re), r = rng_index(re);
		if(l > r) std::swap(l, r);
		queries[i][0] = l;
		queries[i][1] = r;
	}
	TIMEIT("build") rmq->build(a.get(), N);
	int queriesXor = 0;
	TIMEIT("queries") for(int i = 0; i < Q; i ++) {
		queriesXor ^= rmq->query(a.get(), queries[i][0], queries[i][1]);
	}
	std::cout << "memory: " << (long long)((getCurrentHeapSize()-baseHeapSize) * 1. / 1024 / 1024 * 1000) / 1000. << "MB used" << std::endl;
	std::cout << "queriesXor: " << queriesXor << std::endl;
	rmq.reset();
	std::cout << std::endl;
}

template<typename Val, int BlockSize> void smalltestRMQ(int N, int X);
void testRMQs() {
	int n1 = 15, x1 = 2; int n2 = 9, x2 = 3;
//	int n1 = 10, x1 = 2; int n2 = 5, x2 = 3;
	//醜いけれどtemplate meta programmingはもっと醜いかなぁって
	smalltestRMQ<bool,1>(n1,x1);
	smalltestRMQ<int,1>(n1,x1);
	smalltestRMQ<int,1>(n2,x2);
	//*
	smalltestRMQ<bool,2>(n1,x1);
	smalltestRMQ<int,2>(n1,x1);
	smalltestRMQ<int,2>(n2,x2);
	smalltestRMQ<bool,3>(n1,x1);
	smalltestRMQ<int,3>(n1,x1);
	smalltestRMQ<int,3>(n2,x2);
	smalltestRMQ<bool,4>(n1,x1);
	smalltestRMQ<int,4>(n1,x1);
	smalltestRMQ<int,4>(n2,x2);
	smalltestRMQ<bool,5>(n1,x1);
	smalltestRMQ<int,5>(n1,x1);
	smalltestRMQ<int,5>(n2,x2);
	smalltestRMQ<bool,6>(n1,x1);
	smalltestRMQ<int,6>(n1,x1);
	smalltestRMQ<int,6>(n2,x2);
	smalltestRMQ<bool,7>(n1,x1);
	smalltestRMQ<int,7>(n1,x1);
	smalltestRMQ<int,7>(n2,x2);
	smalltestRMQ<bool,8>(n1,x1);
	smalltestRMQ<int,8>(n1,x1);
	smalltestRMQ<int,8>(n2,x2);
	smalltestRMQ<bool,9>(13,x1);
	smalltestRMQ<int,9>(13,x1);
	smalltestRMQ<int,9>(8,x2);
	smalltestRMQ<bool,10>(10,x1);
	smalltestRMQ<int,10>(10,x1);
	smalltestRMQ<int,10>(6,x2);
	//*/
}

template<typename Val, int BlockSize, typename Compare>
void testRMQ(const Val *a, int n) {
	Compare comp = Compare();
	YuanAtallahRMQ<Val, Compare, BlockSize> rmq;
	rmq.build(a, n);
	for(int l = 0; l < n; l ++) {
		int minIndex = l;
		Val minVal = a[l];
		for(int r = l; r < n; r ++) {
			if(comp(a[r], minVal)) {
				minVal = a[r];
				minIndex = r;
			}
			int q = rmq.query(a, l, r);
			my_assert(q == minIndex);
			Val v = rmq.queryVal(a, l, r);
			my_assert(a[q] == v);
		}
	}
}

template<typename Val, int BlockSize>
void smalltestRMQ(int N, int X) {
	smalltestRMQ_compare<Val, BlockSize, std::less<Val> >(N, X);
	smalltestRMQ_compare<Val, BlockSize, std::greater<Val> >(N, X);
}

#pragma warning(disable: 4800)
template<typename Val, int BlockSize, typename Compare>
void smalltestRMQ_compare(int N, int X) {
	std::unique_ptr<Val[]> buf(new Val[N]);
	int tests = 0;
	for(int n = 0, combs = 1; n < N; n ++, combs *= X) {
		for(int x = 0; x < combs; x ++) {
			if(tests++ % 4096 == 0) {
				std::printf("\rsmallTests<%s,%d,%s>(%d, %d): %d, %d..."
					, typeid(Val).name(), BlockSize, typeid(Compare).name()
					, N, X, n, x);
			}
			int y = x;
			for(int i = 0; i < n; i ++) {
				buf[i] = (Val)(y % X);
				y /= X;
			}
			testRMQ<Val, BlockSize, Compare>(buf.get(), n);
		}
	}

}


/*
ver.2.0 2013/09/21 18:36

DirectRMQ<int,6> N: 100000, Q: 10000000 #0:
construct: 0.000006 sec.
build: 0.004196 sec.
queries: 0.572987 sec.
memory: 0.953MB used
queriesXor: 85442

DirectRMQ<int,6> N: 1000000, Q: 10000000 #0:
construct: 0.000006 sec.
build: 0.049873 sec.
queries: 2.439712 sec.
memory: 12.079MB used
queriesXor: 304683

DirectRMQ<int,6> N: 10000000, Q: 10000000 #0:
construct: 0.000004 sec.
build: 0.607103 sec.
queries: 3.350044 sec.
memory: 139.872MB used
queriesXor: 8653118

DirectRMQ<int,8> N: 100000, Q: 10000000 #0:
construct: 0.000098 sec.
build: 0.003837 sec.
queries: 0.495505 sec.
memory: 0.667MB used
queriesXor: 35623

DirectRMQ<int,8> N: 1000000, Q: 10000000 #0:
construct: 0.000097 sec.
build: 0.041503 sec.
queries: 2.212698 sec.
memory: 8.106MB used
queriesXor: 770235

DirectRMQ<int,8> N: 10000000, Q: 10000000 #0:
construct: 0.000095 sec.
build: 0.502731 sec.
queries: 3.282955 sec.
memory: 104.904MB used
queriesXor: 9531436

DirectRMQ<int,10> N: 100000, Q: 10000000 #0:
construct: 0.001400 sec.
build: 0.005461 sec.
queries: 0.649317 sec.
memory: 2.135MB used
queriesXor: 37782

DirectRMQ<int,10> N: 1000000, Q: 10000000 #0:
construct: 0.001390 sec.
build: 0.043984 sec.
queries: 2.794425 sec.
memory: 8.086MB used
queriesXor: 661373

DirectRMQ<int,10> N: 10000000, Q: 10000000 #0:
construct: 0.001443 sec.
build: 0.446009 sec.
queries: 4.012880 sec.
memory: 81.71MB used
queriesXor: 2388222

DirectRMQ<int,12> N: 100000, Q: 10000000 #0:
construct: 0.021710 sec.
build: 0.008024 sec.
queries: 1.076967 sec.
memory: 28.566MB used
queriesXor: 46133

DirectRMQ<int,12> N: 1000000, Q: 10000000 #0:
construct: 0.021798 sec.
build: 0.067656 sec.
queries: 3.385155 sec.
memory: 33.97MB used
queriesXor: 297526

DirectRMQ<int,12> N: 10000000, Q: 10000000 #0:
construct: 0.021687 sec.
build: 0.543269 sec.
queries: 4.411227 sec.
memory: 95.323MB used
queriesXor: 3361172

DirectRMQ<int,14> N: 100000, Q: 10000000 #0:
construct: 0.265459 sec.
build: 0.009410 sec.
queries: 1.504874 sec.
memory: 499.906MB used
queriesXor: 109643

DirectRMQ<int,14> N: 1000000, Q: 10000000 #0:
construct: 0.341290 sec.
build: 0.093893 sec.
queries: 4.013235 sec.
memory: 504.538MB used
queriesXor: 337634

DirectRMQ<int,14> N: 10000000, Q: 10000000 #0:
construct: 0.325318 sec.
build: 0.787264 sec.
queries: 5.375413 sec.
memory: 557.127MB used
queriesXor: 8693539

*/

/*
ver.1.01 2014/01/28 22:55

YuanAtallahRMQ<int,6> N: 100000, Q: 10000000 #0:
construct: 0.000010 sec.
build: 0.011928 sec.
queries: 1.077301 sec.
memory: 1.907MB used
queriesXor: 85442
left: used 20 / 64 (31.25%)
right: used 20 / 64 (31.25%)

YuanAtallahRMQ<int,6> N: 1000000, Q: 10000000 #0:
construct: 0.000006 sec.
build: 0.140903 sec.
queries: 3.567187 sec.
memory: 24.159MB used
queriesXor: 304683
left: used 20 / 64 (31.25%)
right: used 20 / 64 (31.25%)

YuanAtallahRMQ<int,6> N: 10000000, Q: 10000000 #0:
construct: 0.000004 sec.
build: 1.566871 sec.
queries: 4.804043 sec.
memory: 279.744MB used
queriesXor: 8653118
left: used 20 / 64 (31.25%)
right: used 20 / 64 (31.25%)

YuanAtallahRMQ<int,8> N: 100000, Q: 10000000 #0:
construct: 0.000070 sec.
build: 0.010764 sec.
queries: 0.989729 sec.
memory: 1.335MB used
queriesXor: 35623
left: used 100 / 512 (19.5313%)
right: used 100 / 512 (19.5313%)

YuanAtallahRMQ<int,8> N: 1000000, Q: 10000000 #0:
construct: 0.000077 sec.
build: 0.122527 sec.
queries: 3.303123 sec.
memory: 18.212MB used
queriesXor: 770235
left: used 100 / 512 (19.5313%)
right: used 100 / 512 (19.5313%)

YuanAtallahRMQ<int,8> N: 10000000, Q: 10000000 #0:
construct: 0.000076 sec.
build: 1.328909 sec.
queries: 4.917191 sec.
memory: 209.808MB used
queriesXor: 9531436
left: used 100 / 512 (19.5313%)
right: used 100 / 512 (19.5313%)

YuanAtallahRMQ<int,10> N: 100000, Q: 10000000 #0:
construct: 0.001561 sec.
build: 0.012384 sec.
queries: 1.237077 sec.
memory: 2.63MB used
queriesXor: 37782
left: used 613 / 8192 (7.48291%)
right: used 622 / 8192 (7.59277%)

YuanAtallahRMQ<int,10> N: 1000000, Q: 10000000 #0:
construct: 0.001401 sec.
build: 0.129249 sec.
queries: 3.415507 sec.
memory: 14.532MB used
queriesXor: 661373
left: used 660 / 8192 (8.05664%)
right: used 657 / 8192 (8.02002%)

YuanAtallahRMQ<int,10> N: 10000000, Q: 10000000 #0:
construct: 0.001378 sec.
build: 1.330331 sec.
queries: 4.664893 sec.
memory: 161.779MB used
queriesXor: 2388222
left: used 660 / 8192 (8.05664%)
right: used 660 / 8192 (8.05664%)

YuanAtallahRMQ<int,12> N: 100000, Q: 10000000 #0:
construct: 0.013829 sec.
build: 0.015360 sec.
queries: 1.758220 sec.
memory: 17.999MB used
queriesXor: 46133
left: used 2307 / 65536 (3.5202%)
right: used 2310 / 65536 (3.52478%)

YuanAtallahRMQ<int,12> N: 1000000, Q: 10000000 #0:
construct: 0.013244 sec.
build: 0.138091 sec.
queries: 4.877209 sec.
memory: 28.808MB used
queriesXor: 297526
left: used 3161 / 65536 (4.8233%)
right: used 3167 / 65536 (4.83246%)

YuanAtallahRMQ<int,12> N: 10000000, Q: 10000000 #0:
construct: 0.014977 sec.
build: 1.316977 sec.
queries: 6.429788 sec.
memory: 151.514MB used
queriesXor: 3361172
left: used 3299 / 65536 (5.03387%)
right: used 3294 / 65536 (5.02625%)

YuanAtallahRMQ<int,14> N: 100000, Q: 10000000 #0:
construct: 0.229048 sec.
build: 0.026083 sec.
queries: 3.430834 sec.
memory: 391.999MB used
queriesXor: 109643
left: used 4600 / 1048576 (0.43869%)
right: used 5089 / 1048576 (0.485325%)

YuanAtallahRMQ<int,14> N: 1000000, Q: 10000000 #0:
construct: 0.273845 sec.
build: 0.171416 sec.
queries: 5.449696 sec.
memory: 401.264MB used
queriesXor: 337634
left: used 11953 / 1048576 (1.13993%)
right: used 15384 / 1048576 (1.46713%)

YuanAtallahRMQ<int,14> N: 10000000, Q: 10000000 #0:
construct: 0.257783 sec.
build: 1.419439 sec.
queries: 6.728318 sec.
memory: 506.44MB used
queriesXor: 8693539
left: used 15731 / 1048576 (1.50023%)
right: used 20753 / 1048576 (1.97916%)

*/

#endif
