//入力: first, second
//x: 一致してる数
//n: firstに欲しい(first - second)の数
//m: firstに要らない(second - first)の数
//インクリメントしてるところをpush_back等にすれば列挙も可能
//STLには無いのかなあ？

sort(all(first)); sort(all(second));
aut(i, first.begin());
aut(j, second.begin());
int x = 0, n = 0, m = 0;
while(i != first.end() || j != second.end()) {
	if(i == first.end() || (j != second.end() && *j < *i)) ++j, n ++;
	else if(j == second.end() || *j > *i) ++i, m ++;
	else x ++, ++i, ++j;
}
