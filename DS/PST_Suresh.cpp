template <typename node>
struct PST {
    #define  mid ((l + r) >> 1)

    vector <node> T;
    vector <int> ll, rr;
    int nodeIdx;

    PST() {
        T = vector <node>(SIZE, 0);
        ll.assign(SIZE, 0);
        rr.assign(SIZE, 0);
        nodeIdx = 1;
    }

    int update(int no, int l, int r, int index) {
        int cur = nodeIdx++;
        assert(nodeIdx <= SIZE);
        ll[cur] = ll[no];
        rr[cur] = rr[no];

        if (l <= index and index <= r) {
            T[cur].sum = T[no].sum + 1;
        }
        if (l == r) return cur;        
        if (index <= mid) {
            ll[cur] =  update(ll[no], l, mid, index);
        } else {
            rr[cur] = update(rr[no], mid + 1, r, index);
        }
        return cur;
    }

    int query(int l, int r, int u, int v, int anc, int k) {
        if (l == r) {
            return l;
        }

        int cnt = T[ll[u]].sum + T[ll[v]].sum - 2 * T[ll[anc]].sum;
        if (cnt >= k) {
            return query(l, mid, ll[u], ll[v], ll[anc], k);
        } else {
            return query(mid + 1, r, rr[u], rr[v], rr[anc], k - cnt);
        }
    }
    #undef mid
};