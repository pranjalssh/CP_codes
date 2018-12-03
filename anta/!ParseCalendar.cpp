//！revcalcは未検証
//！1900年より前は扱えない(定数を変えれば良いが)

//"SRM 213 DIV1 Medium Uptime"で使ったもの
//そのままでは使えないと思うから、適当に抜き出したりして使うこと
//
//isleap: yがうるう年か？
//calc: 年月日から、1900/1/1からの経過日数を返す
//	O(Y), 計算量: (Y-1900)*軽
//revcalc: 1900/1/1からの経過日数から年月日を返す
//	O(Y), 計算量: (Y-1900)*軽

const string month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int days[2][12] =
	{{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	,{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
const int totaldays[2] = {365, 366};

inline bool isleap(ll y) {
	return (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0));
}
ll calc(ll y, ll m, ll d) {
	if(d != 1) return (d-1) + calc(y, m, 1);
	if(m != 1) {
		ll r = 0;
		reu(i, 1, m) r += days[isleap(y)][i-1];
		return r + calc(y, 1, 1);
	}
	ll r = 0;
	reu(i, 1900, y) {
		r += totaldays[isleap(i)];
	}
	return r;
}
ll parse(string t) {
	each(i, t) if(*i==':') *i = ' ';
	stringstream ss(t);
	ll D, M, Y, h, m, s; string _, ampm, Ms;
	ss>>D>>Ms>>Y>>_>>h>>m>>s>>ampm;
	if(h == 12) h = 0;
	if(ampm == "PM") h += 12;
	M = -1; rep(i, 12) if(Ms == month[i]) M = i+1; if(M == -1) cout << "!!! M: " << Ms << endl;
	return calc(Y, M, D)*86400+h*3600+m*60+s;
}
string pretty(ll x) {
	ll d = x / 86400, hms = x % 86400;
	ll h = hms / 3600, m = hms / 60 % 60, s = hms % 60;
	stringstream ss;
	if(d) ss << d << "d ";
	if(h) ss << h << "h ";
	if(m) ss << m << "m ";
	if(s) ss << s << "s ";
	return ss.str() == "" ? "" : ss.str().substr(0, ss.str().size()-1);
}

void revcalc(ll ymd, ll& y, ll& m, ll& d) {
	y = 1900;
	while(ymd - totaldays[isleap(y)] >= 0) ymd -= totaldays[isleap(y)], y ++;
	m = 1;
	while(ymd - days[isleap(y)][m-1] >= 0) ymd -= days[isleap(y)][m-1], m ++;
	d = 1 + ymd;
}
