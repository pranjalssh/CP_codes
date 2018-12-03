template<unsigned D> struct Div2By1_impl {
	template<unsigned X, int BIT> struct CalcSHIFTS {
		enum : int {
			val = (X >> BIT) == 0 ? BIT + CalcSHIFTS<X,BIT/2>::val : CalcSHIFTS<(X >> BIT),BIT/2>::val
		};
	};
	template<unsigned X> struct CalcSHIFTS<X,0> {
		enum : int { val = 0 };
	};
 
	enum : int { SHIFTS = CalcSHIFTS<D,16>::val };
	enum : unsigned {
		DEN = D << SHIFTS,
		IDEN = (unsigned)(0xffffffffffffffffULL / DEN)
	};
};
 
template<unsigned D> class Div2By1 {
	//let inv x = ((2^64 - 1) `div` x) - 2^32 -- これの前にSHIFTSを計算してシフト後の値で計算する
	typedef Div2By1_impl<D> impl;
 
	static inline void div2by1(unsigned numHigh, unsigned numLow, unsigned &quot, unsigned &rem) {
		unsigned long long q; unsigned qh, ql, r, mask;
		q = (unsigned long long)numHigh * impl::IDEN + numLow;
		qh = (unsigned)(q >> 32), ql = (unsigned)q;
		qh += numHigh + 1;
		r = numLow - qh * impl::DEN;
		mask = -(r > ql);
		qh += mask;
		r += mask & impl::DEN;
		if(r >= impl::DEN) {
			qh += 1;
			r -= impl::DEN;
		}
		quot = qh;
		rem = r;
	}
	//numHigh > Dの場合(の一部)で正しくない結果を返す
	static inline void unsafe_div2by1_shift(unsigned numHigh, unsigned numLow, unsigned &quot, unsigned &rem) {
		div2by1(numHigh << impl::SHIFTS | numLow >> (32 - impl::SHIFTS), numLow << impl::SHIFTS, quot, rem);
		rem >>= impl::SHIFTS;
	}
 
	static inline void div2by1_shift(unsigned numHigh, unsigned numLow, unsigned &quotHigh, unsigned &quotLow, unsigned &rem) {
		if(numHigh < D) {
			quotHigh = 0;
			div2by1(numHigh << impl::SHIFTS | numLow >> (32 - impl::SHIFTS), numLow << impl::SHIFTS, quotLow, rem);
		}else {
			rem = numHigh >> (32 - impl::SHIFTS);
			div2by1(rem, numHigh << impl::SHIFTS | numLow >> (32 - impl::SHIFTS), quotHigh, rem);
			div2by1(rem, numLow << impl::SHIFTS, quotLow, rem);
		}
		rem >>= impl::SHIFTS;
	}
 
public:
	static inline void unsafe_divmod(unsigned long long x, unsigned &q, unsigned &r) {
		unsafe_div2by1_shift((unsigned)(x >> 32), (unsigned)x, q, r);
	}
	static inline void divmod(unsigned long long x, unsigned long long &q, unsigned &r) {
		div2by1_shift((unsigned)(x >> 32), (unsigned)x, *((unsigned*)&q+1), *((unsigned*)&q), r);	//エンディアンとか仮定してる…
	}
 
	static inline unsigned unsafe_divide(unsigned long long x) {
		unsigned q, r;
		unsafe_div2by1_shift((unsigned)(x >> 32), (unsigned)x, q, r);
		return q;
	}
	static inline unsigned unsafe_modulo(unsigned long long x) {
		unsigned q, r;
		unsafe_div2by1_shift((unsigned)(x >> 32), (unsigned)x, q, r);
		return r;
	}
	static inline unsigned long long divide(unsigned long long x) {
		unsigned long long q; unsigned r;
		div2by1_shift((unsigned)(x >> 32), (unsigned)x, *((unsigned*)&q+1), *((unsigned*)&q), r);
		return q;
	}
	static inline unsigned modulo(unsigned long long x) {
		unsigned dummy; unsigned r;
		div2by1_shift((unsigned)(x >> 32), (unsigned)x, dummy, dummy, r);
		return r;
	}
};
