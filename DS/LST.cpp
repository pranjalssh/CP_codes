template<class node>
struct ST {
#define mid ((l+r)>>1)
	int n;
	vector<node> T;
	ST(int n): n(n), T(4 * n + 10) {}

	inline void resolve(int no, int l, int r) {
		/*FUCK*/
		T[no].sum += T[no].lazy;
		T[no].mx.F += T[no].lazy;
		if (l != r) {
			/*FUCK*/
			T[no << 1].lazy += T[no].lazy ;
			T[no << 1 | 1].lazy += T[no].lazy ;
		}
		T[no].lazy = 0;
	}
	void build(int no, int l, int r) {
		if (l == r) {
			/*FUCK*/
			T[no].mx.S = l;
			return;
		}
		build(no << 1, l, mid), build(no << 1 | 1, mid + 1, r);
		T[no] = T[no << 1] + T[no << 1 | 1];
	}
	void upd(int no, int l, int r, int pos, int val) {
		resolve(no, l, r);

		if (l == r) {/*FUCK*/ T[no].sum = val, T[no].mx = {val, l}; return;}
		if (pos <= mid) upd(no << 1, l, mid, pos, val);
		else upd(no << 1 | 1, mid + 1, r, pos, val);

		T[no] = T[no << 1] + T[no << 1 | 1];
	}

	node qry(int no, int l, int r, int u, int v) {
		resolve(no, l, r);

		if (l >= u and r <= v)return T[no];
		if (v <= mid) {return qry(no << 1, l, mid, u, v);}
		else if (u >= mid + 1) {return qry(no << 1 | 1, mid + 1, r, u, v);}

		return qry(no << 1, l, mid, u, v) + qry(no << 1 | 1, mid + 1, r, u, v);
	}

	void updr(int no, int l, int r, int u, int v, int val) {
		resolve(no, l, r);

		if (l >= u and r <= v) {T[no].lazy = val ; resolve(no, l, r); return;}
		if (r < u or l > v) return;

		updr(no << 1, l, mid, u, v, val), updr(no << 1 | 1, mid + 1, r, u, v, val);
		T[no] = T[no << 1] + T[no << 1 | 1];
	}
#undef mid
};


struct node {
	/*FUCK*/
	int sum, lazy;
	ii mx;
	node(int sum = 0, int lazy = 0, ii mx = {0, 0}): sum(sum), lazy(lazy), mx(mx) {}
	node operator +(const node&rhs) {
		return node(sum + rhs.sum, 0, max(mx, rhs.mx));
	}
};
