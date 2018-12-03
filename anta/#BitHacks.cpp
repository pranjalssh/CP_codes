inline int popcount(unsigned x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

inline int select32(unsigned x, int k) {
	unsigned a, b, c; int t, s;
	a = (x & 0x55555555) + ((x >> 1) & 0x55555555);
	b = (a & 0x33333333) + ((a >> 2) & 0x33333333);
	c = (b & 0x0f0f0f0f) + ((b >> 4) & 0x0f0f0f0f);
	t = (c & 0xff) + ((c >> 8) & 0xff);
	s = 0;
	s += ((t - k - 1) & 128) >> 3; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 16, k -= t;
	t = (c >> s) & 0xf;
	s += ((t - k - 1) & 128) >> 4; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 8, k -= t;
	t = (b >> s) & 0x7;
	s += ((t - k - 1) & 128) >> 5; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 4, k -= t;
	t = (a >> s) & 0x3;
	s += ((t - k - 1) & 128) >> 6; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 2, k -= t;
	t = (x >> s) & 0x1;
	s += ((t - k - 1) & 128) >> 7; //if(k >= t) s += 1;
	return s;
}
