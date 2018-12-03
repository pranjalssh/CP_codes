template<typename T>
inline size_t &hash_combine(size_t &x, const T &y) {
	return x ^= hash<T>()(y) + 0x9e3779b9 + (x << 6) + (x >> 2);
}
