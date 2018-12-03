long long MOD = 1000000007ll;
long long inv[1000010];
 
int main(void){
    int i;
    inv[1] = 1;
    for(i=2;i<=1000000;i++) inv[i] = inv[MOD%i] * (MOD - MOD/i) % MOD;
    return 0;
}
 
