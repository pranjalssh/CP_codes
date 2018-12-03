
typedef ll Number;
struct MaxPlus {
	Number t; struct MinusInf_tag {};
	explicit MaxPlus(Number x):t(x){}
	explicit MaxPlus(MinusInf_tag):t(-INF){}
	MaxPlus(){}
	inline bool isMinInf() const {return t==-INF;}
	MaxPlus operator+(const MaxPlus& y) const {
		if(isMinInf())return y;
		else if(y.isMinInf()) return *this;
		else return MaxPlus(max(t, y.t));
	}
	MaxPlus operator*(const MaxPlus& y) const {
		if(isMinInf() || y.isMinInf()) return MaxPlus(MaxPlus::MinusInf_tag());
		else return MaxPlus(t + y.t);
	}
};


//MinPlusÉoÅ[ÉWÉáÉì
typedef ll Number;
struct MinPlus {
	Number t; struct PlusInf_tag {};
	explicit MinPlus(Number x):t(x){}
	explicit MinPlus(PlusInf_tag):t(INF){}
	MinPlus(){}
	inline bool isPlusInf() const {return t==INF;}
	MinPlus operator+(const MinPlus& y) const {
		if(isPlusInf())return y;
		else if(y.isPlusInf()) return *this;
		else return MinPlus(min(t, y.t));
	}
	MinPlus operator*(const MaxPlus& y) const {
		if(isPlusInf() || y.isPlusInf()) return MinPlus(MinPlus::PlusInf_tag());
		else return MinPlus(t + y.t);
	}
};

