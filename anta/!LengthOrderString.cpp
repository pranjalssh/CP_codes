
//length order & maybe. “ñ‚Â‚ğ“Z‚ß‚é‚Ì‚Í‚Ç‚¤‚È‚ñ‚¾cH‚Ü‚ ‚¢‚¢‚©
struct lstr {
	string s;
	bool inf;
	lstr(string s,bool t=false):s(s),inf(t){}
	lstr(const char*s):s(s),inf(0){}
	lstr(bool t):s(""),inf(t){}
	string infdef(string def) const{return inf ? def : s;}
};
lstr operator+(const char*x,const lstr&y){return lstr(x+y.s,y.inf);}
bool operator<(const lstr&x,const lstr&y){
	if(x.inf&&y.inf) return false;
	else if(x.inf) return false;
	else if(y.inf) return true;
	else if(x.s.size() != y.s.size()) return x.s.size() < y.s.size();
	else return x.s < y.s;
}
