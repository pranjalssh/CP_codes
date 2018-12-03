//<http://koj.miz-miz.biz/view_code/101146>
	const char *names[12] = {
		"Do", "Do#", "Re", "Re#", "Mi", "Fa", "Fa#", "So", "So#", "Ra", "Ra#", "Ti"
	};
	const int basex = 9; double basehz = 440;
	vector<double> hz; vector<string> ts;
	rer(o, -15, 10) rep(x, 12) {
		int idx = (o * 12 + x) - basex;
		double p = basehz * pow(2., idx / 12.);
		hz.pb(p);
		stringstream ss;
		ss << names[x] << " " << o;
		ts.pb(ss.str());
	}
