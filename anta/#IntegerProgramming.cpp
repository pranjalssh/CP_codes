//SRM 488 DIV1 Hard‚Å—p‚¢‚½‚à‚Ì
typedef double Num;
typedef short Var;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;
const Num Inf = numeric_limits<Num>::infinity();
const Num NaN = numeric_limits<Num>::quiet_NaN();
#define IsNaN(x) (x != x)
const Num EPS = 1e-10;
inline bool isInteger(Num x) { return abs(floor(x) - x) <= EPS; }

Num simplex(const Mat& A, const Vec& b, const Vec& c, vector<Num> &out_assigns) {
	const int m = A.size(), n = A[0].size();
	Mat D(m+1, Vec(n+1)); vi id(n+m+1);
	rer(i, 0, n+m) id[i] = i;
	rep(i, m) {
		rep(j, n) D[i][j] = -A[i][j];
		D[i][n] = b[i];
	}
	rep(j, n) D[m][j] = c[j];
	while(1) {
		int r = m, s = n+m;
		rep(i, m) if(D[i][n] < -EPS && id[n+r] > id[n+i]) r = i;
		rep(j, n) if(D[m][j] > EPS && id[s] > id[j]) s = j;
		if(r == m && s == n + m) break;
		if(id[n + r] < id[s]) {
			s = n + m;
			rep(j, n) if(D[r][j] > EPS && id[s] > id[j]) s = j;
		}else {
			r = m;
			rep(i, m) if(D[i][s] < -EPS && id[n+r] > id[n+i]) r = i;
		}
		if(r == m) { /* Unbounded */ return Inf; }
		if(s == n + m) { /* NoSolution */ return NaN; }
		swap(id[s], id[n+r]);
		D[r][s] = 1. / D[r][s];
		rer(j, 0, n) if(j != s) D[r][j] *= -D[r][s];
		rer(i, 0, m) if(i != r && abs(D[i][s]) > EPS) {
			rer(j, 0, n) if(j != s) D[i][j] += D[r][j] * D[i][s];
			D[i][s] *= D[r][s];
		}
	}

	out_assigns.assign(n, 0);
	rep(i, m) if(id[n+i] < n)
		out_assigns[id[n+i]] = D[i][n];

	return D[m][n];
}

Num go_simplex(const unique_ptr<Var[]> &vars, int orderi, Mat A, Vec b, Vec c, vector<Num> &out_assigns) {
	int m = b.size();
	Num cs = 0;
	rep(i, orderi) {
		Num val = vars[i];
		rep(j, m) {
			b[j] -= A[j][i] * val;
			A[j][i] = 0;
		}
		cs += c[i] * val;
		c[i] = 0;
	}
	Num solution = simplex(A, b, c, out_assigns);
	if(!IsNaN(solution) && solution != Inf) {
		solution += cs;
		rep(i, orderi)
			out_assigns[i] = vars[i];
	}
	return solution;
}

pair<Num, Num> get_relaxedbound_simplex(const unique_ptr<Var[]> &vars, int orderi, const Mat &org_A, const Vec &org_b, const Vec &org_c, Num best) {
	if(IsNaN(best)) best = -Inf;
	int n = org_c.size(), m = org_b.size();
	Mat A(m+1, Vec(n - orderi)); Vec b(m+1), c(n - orderi);
	rep(j, m) b[j] = org_b[j];
	rep(i, orderi) {
		Num val = vars[i];
		rep(j, m)
			b[j] -= org_A[j][i] * val;
		best -= org_c[i] * val;
	}
	reu(i, orderi, n) {
		rep(j, m) A[j][i - orderi] = org_A[j][i];
		A[m][i - orderi] = -org_c[i];
	}
	b[m] = -best;
	vector<Num> tmp;
	c[0] = 1;
	Num upper = simplex(A, b, c, tmp);
	c[0] = -1;
	Num lower = -simplex(A, b, c, tmp);
	return make_pair(lower, upper);
}

Num branch_and_bound(const vector<pair<Var,Var> > &bounds, const Mat &A, const Vec &b, const Vec &c, Vec &out_assigns) {
	int n = bounds.size(), m = b.size();
	assert(n == c.size()); assert(n == A[0].size()); assert(m == A.size());

	Num best = NaN;
	//BFS
	vector<unique_ptr<Var[]>> q, next;
	next.push_back(nullptr);
	int orderi = 0;
	while(!next.empty()) {
		q.swap(next);
		while(!q.empty()) {
			unique_ptr<Var[]> assigns = move(q.back()); Vec relaxed_assigns; q.pop_back();
			Num relaxed = go_simplex(assigns, orderi, A, b, c, relaxed_assigns);
			if(IsNaN(relaxed) || best >= relaxed - EPS) continue;
			
			bool bounds_ok = true;
			rep(i, n) {
				Num assign = relaxed_assigns[i];
				bool e = bounds[i].first - EPS <= assign && assign <= bounds[i].second + EPS;
				e &= abs(assign - floor(assign + EPS)) <= EPS;
				if(!(bounds_ok &= e)) break;
			}
			if(bounds_ok) {
				if(IsNaN(best) || best < relaxed) {
					best = relaxed;
					out_assigns = relaxed_assigns;
				}
				continue;
			}

			if(orderi == n) continue;
			
			pair<Num, Num> relaxed_bound = get_relaxedbound_simplex(assigns, orderi, A, b, c, best);
			Var lower = (Var)(max((Num)bounds[orderi].first, relaxed_bound.first) + 1 - EPS);
			Var upper = (Var)(min((Num)bounds[orderi].second, relaxed_bound.second) + EPS);
			
			for(int x = upper; x >= lower; x --) {
				unique_ptr<Var[]> nx(new Var[orderi+1]);
				for(int i = 0; i < orderi; i ++) nx[i] = assigns[i];
				nx[orderi] = x;
				next.push_back(move(nx));
			}
		}
		orderi ++;
	}
	
	return best;
}


//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//‹Œver.
#if 0
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cassert>
#define rep(i,n) for(int (i)=0;(i)<(int)(n);++(i))
#define rer(i,l,u) for(int (i)=(int)(l);(i)<=(int)(u);++(i))
#define reu(i,l,u) for(int (i)=(int)(l);(i)<(int)(u);++(i))
#define aut(v, x) __typeof(x) v = (x)
#define each(it,o) for(__typeof((o).begin()) it = (o).begin(); it != (o).end(); ++ it)
#define all(o) (o).begin(), (o).end()
#define pb(x) push_back(x)
#define mp(x,y) make_pair((x),(y))
#define mset(m,v) memset(m,v,sizeof(m))
#define INF 0x3f3f3f3f
#define INFL 0x3f3f3f3f3f3f3f3fLL

using namespace std;
typedef vector<int> vi; typedef pair<int,int> pii; typedef vector<pair<int,int> > vpii;
typedef long long ll; typedef vector<long long> vl; typedef pair<long long,long long> pll; typedef vector<pair<long long,long long> > vpll;
typedef vector<string> vs; typedef long double ld;


typedef double Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;
const Num Inf = numeric_limits<Num>::infinity();
const Num NaN = numeric_limits<Num>::quiet_NaN();
#define IsNaN(x) (x != x)
const Num EPS = 1e-10;
inline bool isInteger(Num x) { return abs(floor(x) - x) <= EPS; }

Num simplex(const Mat& A, const Vec& b, const Vec& c, vector<Num> &out_assigns) {
	const int m = A.size(), n = A[0].size();
	Mat D(m+1, Vec(n+1)); vi id(n+m+1);
	rer(i, 0, n+m) id[i] = i;
	rep(i, m) {
		rep(j, n) D[i][j] = -A[i][j];
		D[i][n] = b[i];
	}
	rep(j, n) D[m][j] = c[j];
	while(1) {
		int r = m, s = n+m;
		rep(i, m) if(D[i][n] < -EPS && id[n+r] > id[n+i]) r = i;
		rep(j, n) if(D[m][j] > EPS && id[s] > id[j]) s = j;
		if(r == m && s == n + m) break;
		if(id[n + r] < id[s]) {
			s = n + m;
			rep(j, n) if(D[r][j] > EPS && id[s] > id[j]) s = j;
		}else {
			r = m;
			rep(i, m) if(D[i][s] < -EPS && id[n+r] > id[n+i]) r = i;
		}
		if(r == m) { /* Unbounded */ return Inf; }
		if(s == n + m) { /* NoSolution */ return NaN; }
		swap(id[s], id[n+r]);
		D[r][s] = 1. / D[r][s];
		rer(j, 0, n) if(j != s) D[r][j] *= -D[r][s];
		rer(i, 0, m) if(i != r && abs(D[i][s]) > EPS) {
			rer(j, 0, n) if(j != s) D[i][j] += D[r][j] * D[i][s];
			D[i][s] *= D[r][s];
		}
	}

	out_assigns.assign(n, 0);
	rep(i, m) if(id[n+i] < n)
		out_assigns[id[n+i]] = D[i][n];

	return D[m][n];
}

Num go_simplex(const vector<Num> &vars, Mat A, Vec b, Vec c, vector<Num> &out_assigns) {
	int n = vars.size(), m = b.size();
	Num cs = 0;
	rep(i, n) {
		Num val = vars[i];
		if(IsNaN(val)) continue;
		rep(j, m) {
			b[j] -= A[j][i] * val;
			A[j][i] = 0;
		}
		cs += c[i] * val;
		c[i] = 0;
	}
	Num solution = simplex(A, b, c, out_assigns);
	if(!IsNaN(solution) && solution != Inf) {
		solution += cs;
		rep(i, n) if(!IsNaN(vars[i]))
			out_assigns[i] = vars[i];
	}
	return solution;
}

Num branch_and_bound(const vector<vector<Num> > &bounds, const Mat &A, const Vec &b, const Vec &c, vector<Num> &out_assigns, const vector<int> &order) {
	int n = bounds.size(), m = b.size();
	assert(n == c.size()); assert(n == A[0].size()); assert(m == A.size());

	Num best = NaN;
	//using BFS
	vector<vector<Num> > q, next;
	next.push_back(vector<Num>(n, NaN));
	int orderi = 0;
	while(!next.empty()) {
		q.swap(next);
		while(!q.empty()) {
			vector<Num> assigns = q.back(), relaxed_assigns; q.pop_back();
			Num relaxed = go_simplex(assigns, A, b, c, relaxed_assigns);
			if(IsNaN(relaxed)) continue;
			if(best >= relaxed - EPS) continue;
			if(IsNaN(best) || relaxed > best) {
				bool bounds_ok = true;
				rep(i, n) {
					Num assign = relaxed_assigns[i];
					const vector<Num> &bound = bounds[i];
					if(bound.empty()) continue;
					bool e = false;
					rep(j, bound.size())
						if(e |= abs(assign - bound[j]) <= EPS) break;
					if(!(bounds_ok &= e)) break;;
				}
				if(bounds_ok) {
					best = relaxed;
					out_assigns = relaxed_assigns;
				}
			}

			if(orderi == n) continue;
			int current_var = order[orderi];
			const vector<Num> &bound = bounds[current_var];
			if(bound.empty()) next.push_back(assigns);	//real var
			else for(int i = bound.size()-1; i >= 0; i --) {
				assigns[current_var] = bound[i];
				next.push_back(assigns);
			}
		}
		orderi ++;
	}
	
	return best;
}

inline bool binary_get_var(const vector<unsigned> &vars, int i) {
	return vars[i / 32] >> i % 32 & 1;
}
inline void binary_set_var(vector<unsigned> &vars, int i) {
	vars[i / 32] |= 1 << i % 32;
}
inline void binary_clear_var(vector<unsigned> &vars, int i) {
	vars[i / 32] &= ~(1 << i % 32);
}
inline int popcount(unsigned x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

Num binary_go_simplex(const vector<unsigned> &assigned, const vector<unsigned> &vars, Mat A, Vec b, Vec c, vector<Num> &out_assigns) {
	int n = vars.size(), m = b.size();
	Num cs = 0;
	rep(i, n) if(binary_get_var(assigned, i)) {
		bool val = binary_get_var(vars, i);
		rep(j, m) {
			b[j] -= A[j][i] * val;
			A[j][i] = 0;
		}
		cs += c[i] * val;
		c[i] = 0;
	}
	Num solution = simplex(A, b, c, out_assigns);
	if(!IsNaN(solution) && solution != Inf) {
		solution += cs;
		rep(i, n) if(!IsNaN(vars[i]))
			out_assigns[i] = vars[i];
	}
	return solution;
}

struct BinaryState {
	vector<unsigned> assigned, assign;
	Num relaxed_solution;
	int numberofassigned;
	BinaryState(const vector<unsigned> &d, const vector<unsigned> &a, Num r):
		assigned(d), assign(a), relaxed_solution(r) {
		numberofassigned = 0;
		rep(i, assigned.size()) numberofassigned += popcount(assigned[i]);
	}
};
bool operator<(const BinaryState &x, const BinaryState &y) {
	return x.numberofassigned < y.numberofassigned;
}

struct ImplyRule {
	int i;
	bool b;
	ImplyRule(int i_, bool b_): i(i_), b(b_) {}
};

Num binary_branch_and_bound(
	const Mat &A, const Vec &b, const Vec &c,
	vector<char> &out_assigns, const vector<vector<ImplyRule> > &implies) {
	int n = c.size(), m = b.size();
	assert(n == A[0].size()); assert(m == A.size());

	Num best = NaN;
	out_assigns.resize(n);
	//using BFS
	priority_queue<BinaryState> q;
	q.push(BinaryState(vector<unsigned>((n+31)/32), vector<unsigned>((n+31)/32), -Inf));
	vector<Num> relaxed_assigns(n);
	while(!q.empty()) {
		vector<unsigned> assigned = q.top().assigned;
		vector<unsigned> assigns = q.top().assign;
		q.pop();
		Num relaxed = binary_go_simplex(
			assigned, assigns, A, b, c, relaxed_assigns);
		if(IsNaN(relaxed)) continue;
		if(best >= relaxed - EPS) continue;
		if(IsNaN(best) || relaxed > best) {
			bool bounds_ok = true;
			rep(i, n) {
				Num assign = relaxed_assigns[i];
				if(abs(assign) > EPS && abs(assign-1) > EPS) break;
			}
			if(bounds_ok) {
				best = relaxed;
				rep(i, n) out_assigns[i] = (char)(relaxed_assigns[i] + .5);
			}
		}

		int next_var = -1;
		rep(i, n) if(!binary_get_var(assigned, i)) {
			next_var = i;
			break;
		}
		if(next_var == -1) continue;

		binary_set_var(assigned, next_var);
		rep(val, 2) {
			if(val) binary_set_var(assigns, next_var);
			vector<unsigned> t_assigned = assigned;
			//ImplyRule‚ðBFS‚Å“K—p‚³‚¹‚é
			queue<int> qq;
			qq.push(next_var);
			while(!qq.empty()) {
				int ivar = qq.front(); qq.pop();
				rep(i, implies[ivar].size()) {
					const ImplyRule &rule = implies[ivar][i];
					if(binary_get_var(t_assigned, rule.i) &&
						binary_get_var(assigns, rule.i) != rule.b) goto bad;
					binary_set_var(t_assigned, rule.i);
					if(rule.b) binary_set_var(assigns, rule.i);
					else binary_clear_var(assigns, rule.i);
					qq.push(rule.i);
				}
			}

			q.push(BinaryState(t_assigned, assigns, relaxed));
		bad:;
		}
	}
	
	return best;
}

#endif