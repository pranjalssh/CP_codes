struct SuffixAutomaton {
    State st[N];
    int sz, last; // sz = size of the automata
    SuffixAutomaton() {
        // clear next map if required
        for (int i = 0; i < N; ++i)
            st[i].next.clear();
        sz = last = 0; st[0].len = 0; st[0].link = -1; ++sz;
    }
    void addCharacter(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        int p = last;
        for (; p != -1 and !st[p].next.count(c); p = st[p].link)
            st[p].next[c] = cur;
        if (p == -1)
            st[cur].link = 0;
        else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len)
                st[cur].link = q;
            else {
                int clone = sz++;
                st[clone] = st[q];
                st[clone].len = st[p].len + 1;
                st[q].link = st[cur].link = clone;
                for (; p != -1 and st[p].next.count(c) and st[p].next[c] == q; p = st[p].link)
                    st[p].next[c] = clone;
            }
        }
        last = cur;
    }
};
