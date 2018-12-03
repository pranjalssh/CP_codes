/*
	安定マッチング, 安定結婚問題, Stable Matching, Stable Marrige Problem
	計算量: O(n^2)
	
	・説明
	http://www.prefield.com/algorithm/misc/stable_matching.html
	> 安定マッチングは次のような問題で説明できる．n 人の男性と n 人の女性が与えられる．各男性は女性に対して選好順位と呼ばれる全順序を持ち，各女性も同様に各男性に対して選好順位を持つ．男性と女性を一人づつペアにすることを考える．このとき，ペアとなっていない男女の対 (m, w) がブロッキングペアであるとは次の二条件を満たすことをいう．
	> 男性不満足：w は m の現在のペア w' よりも好ましい．
	> 女性不満足：m は w の現在のペア m' よりも好ましい．
	> 安定マッチングとは，ブロッキングペアの存在しないマッチングのことをいう．
	二部グラフがあって、優先順位が決まってて、優先順位が極大なマッチングを求める感じ。
	また、これは男性最良安定マッチングを求める。
		各男性ごとに、安定マッチングの中で最良の女性とペアになっている。(語弊があるかも？なんというか、極大な感じ)
		これは一意であることに注意。そのため、それを返す問題も作れる。
	
	・任意にダミーの人を置ける
		ダミーの人は他の全ての優先順位をダミーでない人より小さくすればいい
	・左右の人数が違ってもいい
		ダミーの人を置けばいい。
		ダミーの人と結婚しているというのは結婚相手がいないということ
	他に「絶対にペアを組みたくない男女対が存在する場合」「一夫多妻制」などもできるらしい
		(http://www.prefield.com/algorithm/misc/stable_matching.html参照)
	・問題例
		SRM 263 DIV1 Hard RobotRace
		「ものを両方の人が取る時には順位が決まっていて、それぞれ1つ以下欲しい物を取りたいとき、
			(同時に考えて)全ての人が最良を選ぶことがわかっている時の最良」
		人とものの二部グラフで、(それの欲しさ, それを取るときの順番)で左最良安定マッチング。
		ブロッキングが「xのものよりyの方が欲しくて、かつyを取ってる人より先に取れる」というふうになる。
		最良は、単純に一番欲しいものを取る感じで。
*/

/*
	(n,n)の完全二部グラフを要求する通常のもの。
	
	v: 各左頂点に対する右頂点の順番(順番通りに右頂点を並べる)
	u: 各右頂点に対する左頂点の順番(同上)
	戻り値: 各`右'頂点に対応する`左'頂点
		！右から左であることに注意！
*/
vi stableMatching(const vector<vi>& v, const vector<vi>& u) {
	int n = v.size();
	vector<vi> p(n, vi(n+1, n));
	vi match(n, n), s(n, 0);
	rep(w, n) rep(i, n) p[w][u[w][i]] = i;
	rep(m0, n) for(int m = m0; m < n; ) {
		int w = v[m][s[m]++];
		if(p[w][m] < p[w][match[w]]) swap(m, match[w]);
	}
	return match;
}

/*
	左右のサイズが違ってもよく、かつ完全二部グラフでなくてもいいバージョン
	引数は上記のものと同じ。戻り値は左から右への割り当てを返す。
	戻り値で-1の要素はその頂点に結ばれる辺が無いことを表す。
*/
vi stableMatching(const vector<vi>& v, const vector<vi>& u) {
	int nm = v.size(), nw = u.size();
	vector<queue<int> > p(nm);
	rep(i, nm) each(j, v[i]) p[i].push(*j);
	queue<int> q;
	rep(i, nm) if(v[i].size()) q.push(i);
	vector<vi> wr(nw, vi(nm, -1));
	rep(i, nw) rep(j, u[i].size()) wr[i][u[i][j]] = j;
	vi wm(nw, -1), mw(nm, -1);
	while(!q.empty()) {
		int m = q.front(); q.pop();
		while(!p[m].empty()) {
			int w = p[m].front(); p[m].pop();
			if(wm[w] == -1) {
				wm[w] = m; mw[m] = w;
				break;
			}else {
				int d = wm[w];
				if(wr[w][m] < wr[w][d]) {
					wm[w] = m; mw[m] = w;
					q.push(d); mw[d] = -1;
					break;
				}
			}
		}
	}
	return mw;
}