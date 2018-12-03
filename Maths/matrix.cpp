


const int mod = 1e9 + 7;
const int SZ = 3;
struct vec{
    int arr[SZ];
    void reset(){
        memset(arr , 0 , sizeof(arr));
    }
};
struct matrix{
    int arr[SZ][SZ];
    void reset(){
        memset(arr , 0 , sizeof(arr));
    }
    void makeiden(){
        reset();
        for(int i = 0 ; i < SZ ; ++i){
            arr[i][i] = 1;
        }
    }
    inline matrix operator * (const matrix &o) const {
        matrix res;
        for(int i = 0 ; i < SZ ; ++i){
            for(int j = 0 ; j < SZ ; ++j){
                unsigned long long sum = 0;
                for(int k = 0 ; k < SZ ; ++k){
                    sum += 1ULL * arr[i][k] * o.arr[k][j];
                }
                res.arr[i][j] = sum % mod;
            }
        }
        return res;
    }
    inline vec operator * (const vec &o) const {
        vec res;
        for(int i = 0 ; i < SZ ; ++i){
            unsigned long long sum = 0;
            for(int k = 0 ; k < SZ ; ++k){
                sum += 1ULL * arr[i][k] * o.arr[k];
            }
            res.arr[i] = sum % mod;
        }
        return res;
    }
};
