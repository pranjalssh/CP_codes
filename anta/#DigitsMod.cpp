int digitsMod(char s[], int n, int m) {
	int x = 0, pow10 = 1;
	for(int i = n-1; i >= 0; i --) {
		if((x += pow10 * (s[i] - '0') % m) >= m) x -= m;
		pow10 = pow10 * 10 % m;
	}
	return x;
}
