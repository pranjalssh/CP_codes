
struct treap {
    #define T int
    struct node {
        T val;
        int prior, size;
        node *l, *r;
        node (const T& val) : val(val), prior(rand()), size(1), l(0), r(0) {}
    };

    node * root;
    treap() : root(0) {}

    inline int size(node * t) {
        return t ? t->size : 0;
    }
    inline void upd_size(node * t) {
        if (t) t->size = size(t->l) + 1 + size(t->r);
    }
    void split(node * t, node *& l, node *& r, const T& key) {
        if (!t) l = r = NULL;
        else if (t->val <= key) split(t->r, t->r, r, key), l = t;
        else split(t->l, l, t->l, key), r = t;
        upd_size(t);
    }
    void merge(node *& t, node * l, node * r) {
        if (!l || !r) t = l ? l : r;
        else if (l->prior > r->prior)merge(l->r, l->r, r), t = l;
        else merge(r->l, l, r->l), t = r;
        upd_size(t);
    }
    //more :
    inline node* search(const T&val) {return search(root, val);}
    node * search(node *t, const T& val) {
        if (!t)return 0;
        if (val == t->val) return t;
        if (val < t->val) return search(t->l, val);
        return search(t->r, val);
    }

    inline T kth(int k) {return kth(root, k);}
    T kth(node *t, int k)
    {
        int x = size(t->l);
        if (k <= x)return kth(t->l, k);
        if (k == x + 1) return t->val;
        return kth(t->r, k - x - 1);
    }

    inline int count(const T& x) {return count(root, x);}
    int count(node *t, const T& x)
    {
        if (!t)return 0;
        if (t->val >= x) return count(t->l, x);
        return size(t->l) + 1 + count(t->r, x);
    }

    inline bool insert(const T& val) {
        node * yo = new node(val);
        if (search(val))return 0;
        node  *l, *r;
        split(root, l, r, val);
        node * x;
        merge(x, l, yo);
        merge(root, x, r);
        return 1;
    }
    inline void erase(const T&val) {
        node *l, *r;
        split(root, l, r, val);
        node * l1, *r1;
        split(l, l1, r1, val - 1);
        delete r1;
        merge(root, l1, r);
    }
    #undef T
};