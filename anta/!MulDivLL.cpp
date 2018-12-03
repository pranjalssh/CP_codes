//商を求めることも簡単にできる

//invを事前計算しておけば高速化が可能。ただし誤差は多少大きくなる
//double ver.は2^51程度までは大丈夫かな
inline long long mulmodll(long long a, long long b, long long m) {
	long long quot = (long long)((double)a * b / m + .5);	//+ cで、c >= 誤差である必要がある。
	long long res = a * b - m * quot;
	if(res < 0) res += m;
	return res;
}

inline long long mulmodll(long long a, long long b, long long m) {
	long long quot = (long long)((long double)a * b / m + 0.5);
	long long res = a * b - m * quot;
	res = a * b - m * quot;
	if(res < 0) res += m;
	return res;
}

//80-bit ver.無駄にinline assemblyなんか使ってみたり(初心者なので間違ってるかも)
//80-bit extended precision …！ m < 2^62まで大丈夫っぽい？10^18は大丈夫
inline long long mulmodll(long long a, long long b, long long m) {
	long long quot, res;
	const unsigned HALF_OF_ONE = 0x3f000000;
#ifdef __GNUC__
//80bitモードを仮定してコメントアウト
//#pragma STDC FENV_ACCESS on
//	unsigned short fcw;
//	asm ("fnstcw %0;" : "=m" (fcw));
//	unsigned short fcw_s = fcw;
//	fcw |= 3<<8;
//	asm ("fldcw %0;" : : "m" (fcw));
	asm (
		"fildq %1;\n\t"
		"fildq %2;\n\t"
		"fmulp;\n\t"
		"fildq %3;\n\t"
		"fdivrp;\n\t"
		"fadd %4;\n\t"
		"fisttpq %0;"
		: "=m" (quot)
		: "m" (a), "m" (b), "m" (m), "m"(HALF_OF_ONE)
		: "st(2)"
		);
//	asm ("fldcw %0;" : : "m" (fcw_s));
//#pragma STDC FENV_ACCESS off
#else
#pragma STDC FENV_ACCESS on
	unsigned short fcw;
	//一旦80bitモードにして戻す
	__asm {
		fnstcw word ptr [fcw];
		mov ax, [fcw];
		or word ptr [fcw], 3<<8;
		fldcw word ptr [fcw];

		fild qword ptr [a];
		fild qword ptr [b];
		fmulp st(1), st(0);
		fild qword ptr [m];
		fdivp st(1), st(0);
		fadd dword ptr [HALF_OF_ONE];
		fisttp qword ptr [quot];

		mov [fcw], ax;
		fldcw word ptr [fcw];
	};
#pragma STDC FENV_ACCESS off
#endif
	res = a * b - m * quot;
	if(res < 0) res += m;
	return res;
}
//doubleのver. doubleで足りる時はlong doubleのver.より速いかな
//下の証明はなんかだめだと思う…何故か1ulpでなく0.5ulpにならないのがよくわからない
//u = 2^{-53}, 常に d <= u
//a' = a(1 + e_a), b' = b(1 + e_b), m' = m(1 + e_m) where abs(e_i) <= u
//fl(a' * b') = (a' b')(1 + d)
//            = ab(1 + e_a)(1 + e_b)(1 + d)
//            = ab(1 + d_1)
//d_1 = e_a + e_b + e_a e_b + d + d e_a + d e_b + d e_a e_b
//|d_1| <= |e_a| + |e_b| + d + O(u e) where e = max(|e_a|,|e_b|)
//      <= |e_a| + |e_b| + u + O(u e)
//fl(fl(a' * b') / m')
//            = (ab(1 + d_1) / m')(1 + d)
//            = ab(1 + d_1)(1 + d) / m(1 + e_m)
//            = (ab / m)(1 + d_1)(1 + d)(1 + e_m)^{-1}
//            = (ab / m)(1 + d_2)
//1/x <= y <-> x >= 1/y
//d_2    = (e_m + d + d_1 + d d_1)/(1 + e_m)
//e = max(|e_a|,|e_b|,|e_c|)
//1/(1+e_m) <= 1/(1-e) = 1 + e + e^2 + e^3 + ... = 1 + e + O(e^2)
//|d_2| <= (|e_m| + d + d_1 + d d_1)(1 + e + O(e^2))
//      <= |e_m| + d + d_1 + u^2 + O(e u)
//      <= |e_a| + |e_b| + |e_m| + 2 u + u^2 + O(e u)
//
//a, b, m < 2^52 なら、e_a = e_b = e_m。
//そのとき|d_2| <= 2 u + u^2。
//また、a, b < m のとき a * b / m < a <= 2^52-1 であるので、
//|fl(fl(a * b) / m) - a * b / m|
//       = (a * b / m) d_2
//      <= (a * b / m) (2u + u^2)
//      < ((2^52-1) * 2u) * (1 + u/2)
//      = (1 - 2u) * (1 + u/2)
//      < 1
//よって誤差は1未満。■
//また、mによって誤差の上限が示せるので(+ c)の部分を小さくして高速化が可能である

//m <= 2^52としている (2^52 > 4e15)。これは誤差に応じて適切に書き換えることによってある程度大きくできる
//また、a, b は正規形である必要がある(0 <= a, b < m である必要がある)
inline long long mulmodll(long long a, long long b, long long m) {
	long long quot = (long long)((double)a * b / m + 1.);	//+ cで、c >= 誤差である必要がある。
	//resはオーバーフロー前提で2^64のmodular arithmeticで計算されていると考えられる。
	//a * bのmod mを取れば良いのだが、estimatedquotのおかげである程度解の近くに来ている。そのためループができる。
	//本来の|a * b - m * quot| >= 2^63 でオーバーフローしてしまうと間違う。
	long long res = a * b - m * quot;
	//誤差 + c <= num + eps であるnum回、"res += m"する必要がある
	if(res < 0) {
		res += m;
		if(res < 0) res += m;
	}
	//while(res >= m) res -= m;
	return res;
}
