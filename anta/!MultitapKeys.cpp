const char *multitap_alphas[10] = {
	"","","ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ"
};
const int multitap_lens[10] = { 0,0,3,3,3,3,3,4,3,4 };
int multitap_key[128];
int multitap_idx[128];
void multitap_init() {
	rep(i, 10) {
		const char *p = multitap_alphas[i];
		int k = 0;
		while(*p) {
			multitap_key[*p] = i;
			multitap_idx[*p] = k;
			k ++; ++ p;
		}
	}
}
