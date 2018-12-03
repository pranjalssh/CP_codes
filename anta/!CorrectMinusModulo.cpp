/*
	rem‚Å‚È‚­mod‚ÌŽÀ‘•
*/

template<typename T>T mod(T x, T y) {
	if(x > 0) return x % y;
	else return (y - -x % y) % y;
}

double fmod2(double x, double y) {
	if(x > 0) return fmod(x, y);
	else return fmod(y - fmod(abs(x), y), y);
}
