//あくまで「絶対もしくは相対で誤差1e-9以下」であるので、この値を計算に使う場合に精度が足りないかもしれないことに注意
//doubleにすると誤差が1e-9より大きくなりうる
//trueが返ったら不定。小さい順であることも保障する
bool solveQuadraticEquation(int a, int b, int c, vector<long double> &out_xs) {
	typedef long double Num;
	if(a == 0 && b == 0)
		return c == 0;
	else if(a == 0)
		out_xs.push_back(-Num(c) / b);
	else {
		long long bb = (long long)b * b, ac = (long long)a * c;
		Num a2 = Num(a) * 2;
		if(b % 2 == 0 && bb / 4 == ac)
			out_xs.push_back(-Num(b) / a2);
		else if(bb / 4 >= ac) {
			Num d;
			if(ac >= 0) d = sqrt(Num(bb - ac * 4));	//bb >= ac*4であることがわかっているので、ac*4はオーバーフローしない
			else d = sqrt(max(Num(0), Num(bb) - Num(ac) * 4));	//bb >= 0 であるので、ac < 0のとき情報落ちは無い
			Num x1 = (-d - (Num)b) / a2;	//情報落ちするけど…
			Num x2 = ( d - (Num)b) / a2;
			if(a < 0) swap(x1, x2);
			out_xs.push_back(x1);
			out_xs.push_back(x2);
		}
	}
	return false;
}
