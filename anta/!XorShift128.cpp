//クラスのstatic変数ver.
	static unsigned xor128_x, xor128_y, xor128_z, xor128_w;
	unsigned xor128() {
		unsigned t = xor128_x ^ (xor128_x << 11);
		xor128_x = xor128_y; xor128_y = xor128_z; xor128_z = xor128_w;
		return xor128_w = xor128_w ^ (xor128_w >> 19) ^ (t ^ (t >> 8));
	}
unsigned C::xor128_x = 123456789, C::xor128_y = 362436069, C::xor128_z = 521288629, C::xor128_w = 88675123;



//structバージョン
struct Xor128 {
	unsigned x, y, z, w;
	Xor128(): x(123456789), y(362436069), z(521288629), w(88675123) { }
	unsigned next() {
		unsigned t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
	}
	unsigned long long nextll() {
		unsigned long long x = (unsigned long long)next() << 32;
		return x | next();
	}
};

unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

unsigned xor128_z = 521288629;
unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = xor128_z; xor128_z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}


unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = (unsigned)time(0), w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

unsigned xor128() {
	static unsigned x = 123456789;
	static unsigned y = 362436069;
	static unsigned z = 521288629;
	static unsigned w = 88675123;
	unsigned t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}




