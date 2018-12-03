//実例を集めよう。
//・実例たち
//線形式(とても簡単な形): http://www.e-olimp.com/en/solutions/1302911
//多項式(stringstreamを使っている): https://judge.npca.jp/submissions/detail/3673/

//よく使いそうなもの
typedef const char *Pos;

inline void expect(char c, Pos &p) {
	assert(*p == c);
	++ p;
}

inline void expect(const char *s, Pos &p) {
	while(*s) {
		assert(*s == *p)
		++ p;
	}
}

//intであることに注意(適切に書き換えること)
//また、マイナスに対応する時にこれをサブルーチンとして使うと-2^31でバグることに注意
int natural(Pos &p) {
	int c = 0;
	while(isdigit(*p)) {
		c = c * 10 + (*p - '0');
		++ p;
	}
	return c;
}

//============================================
//色々なんでも張っておこう！

int time24hhmm(Pos &p) {
	int h = natural(p);
	expect(p, ':');
	int m = natural(p);
	return h * 60 + m;
}
