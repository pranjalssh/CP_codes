//sum_(k=0)^Infinity poly(k+1) x^k の収束先を求める。多項式同士の商になる。
pair<Polynomial,Polynomial> polynomialCoefficientPowerSeries(const Polynomial &poly) {
	int deg = poly.degree1();
	Polynomial resnum, resden(1), den(2);
	resden.coef[0] = 1; den.coef[0] = 1; den.coef[1] = -1;
	vector<Polynomial::Val> A(deg);
	A[0] = 1;
	for(int i = 0; i < deg; i ++) {
		for(int j = i; j > 0; j --)
			A[j] = A[j-1] * (i-j) + A[j] * (j+1);
		Polynomial p(i+2);
		for(int j = 0; j <= i; j ++)
			p.coef[j] = poly.coef[i] * A[j];
		resnum = resnum * den + p;
		resden = resden * den;
	}
	return make_pair(resnum, resden);
}


//sum_(k=0)^Infinity poly(k) x^k を求める
//= x sum_(k=0)^Infinity poly(k+1) x^k を使う。
//が、0^0のせいか、定数項があるとうまくいかない。定数項にはxをかけないようにすればいい
pair<Polynomial,Polynomial> polynomialCoefficientPowerSeries2(const Polynomial &poly) {
	Polynomial p = poly, q(1), den(2), x(2);
	if(p.degree1() >= 1) p.coef[0] = 0;
	pair<Polynomial,Polynomial> r = polynomialCoefficientPowerSeries(p);
	q.coef[0] = poly.coef[0];
	pair<Polynomial,Polynomial> u = polynomialCoefficientPowerSeries(q);
	den.coef[0] = 1; den.coef[1] = -1; x.coef[1] = 1;
	for(int i = 2; i < r.second.degree1(); i ++)
		u.first = u.first * den;
	r.first = r.first * x + u.first;
	return r;
}
