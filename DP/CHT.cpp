typedef double T;
struct PT {
  pair<T, T> pt;
  int id;
  PT(){}
  PT(T m, T c, int id = -1):pt(m,c), id(id){}
};
//has precision issues in double
bool complee(const pair<T, T>& a, const pair<T, T>& b) {return a.F * b.S <= a.S * b.F;}
struct CHT {  // maintains min envelope
  
  vector<PT> info;

  vector<int> pts;   // stack of points on which ans changes
  vector<int> st; // stack of envelope

  void insert(PT x) { // add in desc. order of slope
    if (st.empty()) {
      info.push_back(x);
      st.push_back(sz(info) - 1);
      info.emplace_back(0, 1);
      pts.push_back(sz(info) - 1); // +ve queries
      return;
    }
    pair<T, T> now(x.pt.S - info[st.back()].pt.S, info[st.back()].pt.F - x.pt.F); //+ve denom as desc slope
    
    if (complee(now, info[pts.back()].pt)) {
      pts.pop_back(); st.pop_back();
      insert(x);
    } else {
      info.push_back(x);
      st.push_back(sz(info) - 1);
      info.emplace_back(now.F, now.S);
      pts.push_back(sz(info) - 1);
    }
  }
  T query(T x) {
    info.emplace_back(x, 1);
    auto it = upper_bound(all(pts), sz(info) - 1, [&](int a, int b){return complee(info[a].pt, info[b].pt);});
    info.pop_back();
    int pos = it - pts.begin(); pos--;
    return info[st[pos]].pt.F * x + info[st[pos]].pt.S;
  }
};

/*
pair<T, int> query_offline(T x) { 
    pair<T, T> yo(x, 1);
    ptr = min(ptr, sz(pts) - 1);
    while (ptr < sz(pts)) {
      if (complee(info[pts[ptr]].pt, yo)) {
        ++ptr;
        continue;
      }
      --ptr;
      break;
    }
    ptr = min(ptr, sz(pts) - 1);
    return {info[st[ptr]].pt.F * x + info[st[ptr]].pt.S, info[st[ptr]].id};
  }
*/

const LL is_query=+(1LL<<62);
struct Line{
  LL m,b;//compare two lines by increasing slope
  mutable function<const Line*()> succ;
  bool operator<(const Line& rhs)const{
    if(rhs.b!=is_query)return m>rhs.m;//> for min
    const Line* s=succ();
    if(!s)return 0;
    return b-s->b>(s->m-m)*rhs.m;//> for min
  }};
struct HullDynamic:public multiset<Line>{
  bool bad(iterator y){//maintains upper hull for max
    auto z=next(y);
    if(y==begin()){
      if(z==end())return 0;
      return y->m == z->m && y->b >= z->b;//>= for min
    }auto x=prev(y);
    if(z==end())
      return y->m==x->m && y->b>=x->b; // >= for min
    return (x->b-y->b)*(z->m-y->m)>=(y->b-z->b)*(y->m-x->m);
  }
  void insert_line(LL m,LL b){
    auto y=insert({ m,b});
    y->succ=[=]{return next(y)==end()?0:&*next(y);};
    if(bad(y)){erase(y);return;}
    while(next(y)!=end() && bad(next(y)))erase(next(y));
    while(y!=begin() && bad(prev(y)))erase(prev(y));
  }
  LL eval(LL x){
    auto l=*lower_bound((Line){x,is_query});
    return l.m*x +l.b;
  }
};