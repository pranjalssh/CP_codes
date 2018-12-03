//<https://twitter.com/chokudai/status/400270440410923008>
//Grundy数が0の時後手必勝(先手必敗)で、0以外の時先手必勝。
//・g(s) = mex { g(t) | t \in next(s) }
//    ここで、mexは「その集合に存在しない最小の非負整数」。
//・複数の独立なゲームがあって、各ターンでそのどれか1つを進められる時、
//    ゲーム全体でのGrundy数はそれぞれのゲームのGrundy数のxor。
//
//・1つ以上のゲームを1度に進められる場合、各ゲームのGrundy数が全て0の時のみ後手必勝(先手必敗)。
//	・Grundy数は全ての山の単なる足し算
//・1つ以上のゲームを1度に進められるが全部は1度に進められない場合、各ゲームのGrundy数が全て等しい時のみ後手必勝(先手必敗)。
//・1つ以上k個以下のゲームを1度に進められる場合、
//    xorのかわりに2進数での各ビットごとの和を(k+1)で割った余りを用いる。
//・最後の1つを取った人が負けのNimの場合、全ての山の石の個数が1個以下の状況で数値と勝敗の関係が逆になる。

//・Nimのフェーズが分かれている(イメージ的には直列にNimが繋がっている)、つまり
//	ある山が0になってからしか次の山に移れない、という場合、
//	grundy(y :: x) = y + (grundy(x) <= y)
//	的な。

//<http://arxiv.org/pdf/math/0409408v1.pdf>
//以下でg(・)はGrundy数を表す。
//◆Maximum Nim
//今がx個の時に、1≦y≦f(x)のy個の石を任意に除去できるとする。これを"Maximum Nim"という。
//
//0 ≦ f(n) - f(n-1) ≦ 1 を満たすとき、fを"regular"であるという。
//Lemma 2.1. fがregularであるなら、Maximum-Nim(f) = g に関して以下が成り立つ：
//g(n) =
//    f(n)            if f(n) - f(n-1) == 1
//    g(n - f(n) - 1) if f(n) - f(n-1) == 0
//
//◆Minimum Nim
//今がx個の時に、f(x)＜y≦xのy個の石を任意に除去できるとする。これを"Minimum Nim"という。
//   (*strictly* greater thanであることに注意)
//Assumuption: n - f(n) → ∞ (13)
//   つまり、f(n) ∈ n - ω(1) ということ。
//数列sに対して、s'(n) = min{ k : s(k) = n } で「ある値が最初に現れる位置」を表す。
//Proposition 4.1. fがregularで(13)を満たすとき、Minimum-nim(f) = hに関して以下が成り立つ：
//    h'(0) = 0
//    h'(n) = q(h'(n-1))
//where
//    q(k) = min{ j : j - f(j) ＞ k } (14)
//Lemma 4.2. fがregularで(13)を満たす時、Maximum-Nim(f) = gに関して以下が成り立つ：
//    g(q(n)) = g(n)
//ここで、q(・)は(14)のもの。
//Proposition 4.3. fがregularで(13)を満たし、Maximum-Nim(f) = g, Minimum-Nim(f) = hに関して以下が成り立つ：
//    h(n) = #{0＜k≦n : g(k) = 0}
//ただし、#SはSの要素数を表す。
//◆Serial Nim
//非空の左端の山からしか取れないルールを考える。これをSerial Nimという。
//Proposition 5.1.
//   let m = min{j | a_j ≠ a_1}
//   if (m is odd) xor (a_m ＜ a_1)
//   then Serial-Nim([a_1,...,a_k]) = a_1
//   else Serial-Nim([a_1,...,a_k]) = a_1 - 1
//   ただし、a_k = 0とする。


//例: <http://www.codechef.com/SHSTRA14/problems/SHASGAME>
//f(x) = x/idx
ll getGrandyNumber(int idx, ll x) {
	ll y = x / idx;
	if(x % idx == 0) return y;
	else return getGrandyNumber(idx, x - y - 1);
}
