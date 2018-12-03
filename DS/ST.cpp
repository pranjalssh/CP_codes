template<class node>
struct ST {
#define mid ((l+r)>>1)
  int n;
  vector<node> T;
  ST(int n): n(n), T(4 * n + 10) {}

  void build(int no, int l, int r, vector<int> &arr) {
    if (l == r) {T[no] = node(arr[l]); return;}

    build(no << 1, l, mid, arr);
    build(no << 1 | 1, mid + 1, r, arr);
    T[no] = T[no << 1] + T[no << 1 | 1];
  }
  void upd(int no, int l, int r, int pos, int val) {
    if (l == r) {/*FUCK*/ T[no] = node(val); return;}
    if (pos <= mid) upd(no << 1, l, mid, pos, val);
    else upd(no << 1 | 1, mid + 1, r, pos, val);

    T[no] = T[no << 1] + T[no << 1 | 1];
  }

  node qry(int no, int l, int r, int u, int v) {
    if (l >= u and r <= v)return T[no];
    if (v <= mid) return qry(no << 1, l, mid, u, v);
    else if (u >= mid + 1) return qry(no << 1 | 1, mid + 1, r, u, v);

    return qry(no << 1, l, mid, u, v) + qry(no << 1 | 1, mid + 1, r, u, v);
  }

#undef mid
};

struct node {
  /*FUCK*/
  int x;
  node(): x(0) {}
  node operator +(const node&rhs) {
    return node(min(x, rhs.x));
  }
};