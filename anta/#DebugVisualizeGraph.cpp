
struct RandomColor {
	double state, S, V;
	RandomColor(double S_ = 0.5, double V_ = 0.95): state(rand() * 1. / (RAND_MAX+1)), S(S_), V(V_) { }

	int get() {
		if((state += 0.618033988749895) >= 1) state -= 1;
		return hsv_to_rgb(state, S, V);
	}
	string get_str() {
		static char tmp[10];
		sprintf(tmp, "\"#%06x\"", get());
		return tmp;
	}

	static int hsv_to_rgb(double h, double s, double v) {
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
};

struct DebugVisualizeGraph {
	typedef int Type;
	bool directed;
	stringstream ss;
	map<string,int> vertices;
	RandomColor randcol;
	map<Type,string> randomcolors;

	DebugVisualizeGraph(bool directed_ = false): directed(directed_) {
		ss << (directed ? "digraph" : "graph") << " {\n";
	}

	string getcolor(Type i) {
		if(!randomcolors.count(i))
			randomcolors[i] = randcol.get_str();
		return randomcolors[i];
	}

	int vertex(const string &name, Type type = 0) {
		if(!vertices.count(name)) {
			int v = vertices.size();
			ss << v;
			ss << " [label=" << '"' << name << '"';
			ss << ",style=filled,fillcolor=" << getcolor(type);
			ss << "];\n";
			vertices[name] = v;
		}
		return vertices[name];
	}
	static string itos(int i) {
		stringstream t; t << i; return t.str();
	}
	int vertex(int i, Type type = 0) {
		return vertex(itos(i), type);
	}

	void addE(const string &v_s, const string &u_s, const string &label = "", const string &color = "") {
		int v = vertex(v_s), u = vertex(u_s);
		ss << v << (directed ? " -> " : " -- ") << u << " [";
		const char *comma = "";
		if(!label.empty()) ss << comma << "label=" << '"' << label << '"', comma = ",";
		if(!color.empty()) ss << comma << "color=" << color, comma = ",";
		ss << "];\n";
	}
	void addE(int i, int j, const string &label = "", const string &color = "") {
		return addE(itos(i), itos(j), label, color);
	}

	void out(ostream &o) { o << ss.str() << "}" << endl; }
	void out_file(const char *filename) {
		FILE *f = fopen(filename, "w");
		fputs(ss.str().c_str(), f);
		fputs("}\n", f);
		fclose(f);
	}
};