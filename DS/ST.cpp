

template <typename node>
struct ST {

  #define mid ((l+r)>>1)


  int n;
  vector<node> T;

  ST(int n): n(n), T(4 * n + 10) {}

  void build(int no, int l, int r, vector<int> &arr) {
    if (l == r) {T[no] = arr[l]/*FUCK*/ ; return;}

    build(no << 1, l, mid, arr);
    build(no << 1 | 1, mid + 1, r, arr);
  }
  inline void LAZY(int no, int l, int r) {
    /*FUCK*/
    if (l != r) {
      /*FUCK*/
      T[no << 1].lazy += T[no].lazy ;
      T[no << 1 | 1].lazy += T[no].lazy ;
    }

    T[no].lazy = 0;
  }
  void upd(int no, int l, int r, int pos, int val) {
    LAZY(no, l, r);

    if (l == r) {/*FUCK*/ T[no] = val; return;}
    if (pos <= mid) upd(no << 1, l, mid, pos, val);
    else upd(no << 1 | 1, mid + 1, r, pos, val);

    T[no] = node::merge(T[no << 1], T[no << 1 | 1]);
  }

  node qry(int no, int l, int r, int u, int v) {
    LAZY(no, l, r);

    if (l >= u and r <= v)return T[no];
    if (v <= mid) {if (l != r)LAZY(no << 1 | 1, mid + 1, r); return qry(no << 1, l, mid, u, v);}
    else if (u >= mid + 1) {if (l != r)LAZY(no << 1, l, mid); return qry(no << 1 | 1, mid + 1, r, u, v);}

    return node::merge(qry(no << 1, l, mid, u, v), qry(no << 1 | 1, mid + 1, r, u, v));
  }

  void updr(int no, int l, int r, int u, int v, int val) {
    LAZY(no, l, r);

    if (l >= u and r <= v) {/*FUCK*/T[no].lazy = val ; LAZY(no, l, r); return;}
    if (r < u or l > v) return;

    updr(no << 1, l, mid, u, v), updr(no << 1 | 1, mid + 1, r, u, v);
    T[no] = node::merge(T[no << 1], T[no << 1 | 1]);
  }
  #undef mid
};

struct node {
  /*FUCK*/
  node() {}
  static node merge(const node&x, const node&y) {

  }
};
