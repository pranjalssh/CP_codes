template<typename T, typename U>
inline auto floordiv(T x, U y) -> decltype(x/y) {
	auto q = x / y, r = x % y;
	return q - ((r!=0) & ((r<0) ^ (y<0)));
}
template<typename T, typename U>
inline auto ceildiv(T x, U y) -> decltype(x/y) {
	auto q = x / y, r = x % y;
	return q + ((r!=0) & !((r<0) ^ (y<0)));
}
