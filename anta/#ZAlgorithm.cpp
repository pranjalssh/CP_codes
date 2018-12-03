//！未検証！

//s[0,n)とs[i,n)のLCAを全て求める。線形時間
void z_algorithm(const char *s, int n, int z[]) {
    int L = 0, R = 0;
    for(int i = 1; i < n; i ++) {
        if(i > R) {
            L = R = i;
            while(R < n && s[R-L] == s[R]) R ++;
            z[i] = R-L; R --;
        }else {
            int k = i-L;
            if(z[k] < R-i+1) z[i] = z[k];
            else {
                L = i;
                while(R < n && s[R-L] == s[R]) R ++;
                z[i] = R-L; R --;
            }
        }
    }
}

//LCAを用いた辞書順比較。左辺がs[0,・)に固定されている
bool lex_lt_to_init(const char *s, const int z[], int i, int x, int y) {
    if(x != y) return x < y;
    int t = z[i];
    if(t >= x) return false;
    return s[t] < s[i+t];
}
