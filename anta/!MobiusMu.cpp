//！すぱその実装はnが素数の時にΘ(n)時間かかる！
//μ(n) =
//  0               (if 素因数分解に同じ素数を2度以上含む場合)
//  (-1)^素因数の数 (otherwise)
//この定義から簡単にわかるので、適切に素因数分解テーブルを作っておいたり事前計算しておいたりすること。

int mu(ll n) {
	int parity = 0;
	for(ll d = 2; d*d <= n; d ++) {
		if(n % (d*d) == 0) return 0;
		else if(n % d == 0) n /= d, parity ++;
	}
	if(n != 1) parity ++;
	return parity % 2 ? -1 : 1;
}
