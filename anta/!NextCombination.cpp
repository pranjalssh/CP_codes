inline int nextCombination(int comb) {
	int x = comb & -comb, y = comb + x;
	return (((comb & ~y) / x) >> 1) | y;
}

//使用例。n個の中からm個取り出す組み合わせ
for(int comb = (1 << (m - 1)) - 1; comb < (1 << n); comb = nextCombination(comb)){
