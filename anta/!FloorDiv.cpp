//！C++11用！
template<typename T, typename U>
inline auto floordiv(T x, U y) -> decltype(x/y) {
	auto q = x / y, r = x % y;
	return q - ((r!=0) & ((r<0) ^ (y<0)));
}

//C++03用。型が違う時に注意！
template<typename T> inline T floordiv(T x, T y) {
	T q = x / y, r = x % y;
	return q - ((r!=0) & ((r<0) ^ (y<0)));
}
