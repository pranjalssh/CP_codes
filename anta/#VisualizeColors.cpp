int hsv_to_rgb(double h, double s, double v) {
	h = fmod(h, 1);
	int h_i = min(max((int)(h * 6), 0), 5);
	double f = h * 6 - h_i;
	double p[4] = { v, v * (1 - s), v * (1 - f*s), v * (1 - (1 - f) * s) };
	static const int table[6][3] = {
		{0,3,1},{2,0,1},{1,0,3},{1,2,0},{3,1,0},{0,1,2}
	};
	int res = 0;
	rep(i, 3) {
		double x = p[table[h_i][i]];
		res = (res << 8) | max(0, min((int)(x * 256), 255));
	}
	return res;
}

//良さげなランダムな色相の色を取得する
void get_random_colors(int n, vector<int> &out_colors) {
	double s = 0.5, v = 0.95;
	double h = rand() * 1. / (RAND_MAX+1);
	rep(i, n) {
		if((h += 0.618033988749895) >= 1) h -= 1;
		out_colors.push_back(hsv_to_rgb(h, s, v));
	}
}

//#rrggbb形式、さらに引用符を付けて取得(graphviz用)
void get_random_colors_str(int n, vector<string> &out_colors) {
	vector<int> t_colors;
	get_random_colors(n, t_colors);
	out_colors.resize(n);
	rep(i, n) {
		static char tmp[10];
		sprintf(tmp, "\"#%06x\"", t_colors[i]);
		out_colors[i] = tmp;
	}
}
