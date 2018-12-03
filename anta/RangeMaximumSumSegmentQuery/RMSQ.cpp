#pragma once
#include <algorithm>
#include <memory>
#include <utility>
#include "../RangeMinimumQuery/DirectRMQ.hpp"

//Chen, K.Y., Chao, K.M.:
//"On the Range Maximum-Sum Segment Query Problem"
//<http://www.csie.ntu.edu.tw/~kmchao/seq05fall/RMSQ.pdf>
//Tested
class RangeMaximumSumSegmentQuery {
public:
	typedef int Val;
	typedef long long ValSum;
	typedef int Index;
	typedef DirectRMQ<ValSum, std::less<ValSum> > RMQ_Min;
	typedef DirectRMQ<ValSum, std::greater<ValSum> > RMQ_Max;
	typedef std::pair<Index, Index> Segment;

	RangeMaximumSumSegmentQuery() { }

	void build(const Val *a, Index n) {
		preprocess1(a, n);
		preprocess2(a, n);
		rmq_max_c.build(&c[0], n + 1);
	}

	//区間は閉区間。L∈[i,j],R∈[i,j],L≦Rの[L,R]の総和を最大化する
	Segment query(Index i, Index j) const {
		Index r = rmq_max_m.query(&m[0], i, j);
		if(p[r] < i) {
			Index r1 = rmq_min_c.query(&c[0], i, r);
			if(r != j) {
				Index r2 = rmq_max_m.query(&m[0], r + 1, j);
				if(c[r+1] - c[r1] < m[r2])
					return Segment(p[r2], r2);
			}
			return Segment(r1, r);
		}else {
			return Segment(p[r], r);
		}
	}

	//区間は閉区間。L∈[i,j],R∈[k,l],L≦Rの[L,R]の総和を最大化する
	Segment query(Index i, Index j, Index k, Index l) const {
		if(k < i) k = i;
		if(j > l) j = l;
		if(j <= k) {
			return query_sub(i, j, k, l);
		}else {
			Segment r1 = query_sub(i, k, k, l);
			Segment r2 = query_sub(k, j, j, l);
			Segment r3 = query(k, j);
			ValSum s1 = sum(r1), s2 = sum(r2), s3 = sum(r3);
			return s1 >= s2 && s1 >= s3 ? r1 : s2 >= s3 ? r2 : r3;
		}
	}

	inline ValSum sum(Index i, Index j) const { return c[j+1] - c[i]; }
	inline ValSum sum(Segment s) const { return sum(s.first, s.second); }
private:
	std::unique_ptr<ValSum[]> c;	//入力の累積和。[0..n]
	RMQ_Min rmq_min_c;
	RMQ_Max rmq_max_c;				//一般のRMSQ用
	std::unique_ptr<Index[]> l;		//左にある大きな値。[0..n-1]。build時の一時変数
	std::unique_ptr<Index[]> p;		//good partner。[0..n-1]
	std::unique_ptr<ValSum[]> m;	//good partnerとの組の区間の和
	RMQ_Max rmq_max_m;

	inline Segment query_sub(Index i, Index j, Index k, Index l) const {
		return Segment(
			rmq_min_c.query(&c[0], i, j),
			rmq_max_c.query(&c[0], k + 1, l + 1) - 1);
	}

	void preprocess1(const Val *a, Index n) {
		c.reset(new ValSum[n + 1]);
		l.reset(new Index[n]);
		c[0] = 0;
		for(Index i = 0; i < n; i ++) {
			c[i+1] = c[i] + a[i];
			l[i] = i - 1;
			while(l[i] >= 0 && c[l[i]+1] < c[i+1])
				l[i] = l[l[i]];
		}
		rmq_min_c.build(&c[0], n + 1);
	}

	void preprocess2(const Val *a, Index n) {
		p.reset(new Index[n]);
		m.reset(new ValSum[n]);
		for(Index i = 0; i < n; i ++) {
			p[i] = rmq_min_c.query(&c[0], l[i] + 1, i);
			m[i] = c[i+1] - c[p[i]];
		}
		l.release();	//使わなくなったら早めにreleaseしとく
		rmq_max_m.build(&m[0], n);
	}
};
