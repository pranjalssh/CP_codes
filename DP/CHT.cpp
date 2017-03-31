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


class DynamicConvexHull {
private:
    
    struct Point {
        
        /*
         Defines the progression of the form x * param + y
         */
        
        long long x;
        long long y;
        bool is_query_point;
        mutable set<Point>::iterator par;
        set<Point> *master; // the set, containing this point
        
        Point () { }
        Point (long long x, long long y, bool is_query_point, set<Point> *master) : x(x), y(y), is_query_point(is_query_point), master(master) { }
        inline bool operator < (const Point &you) const {
            
            /*
             There are two possible situations:
             - We're adding a point to the convex hull;
             - We're looking for the optimal possible value of x * param + b for some param = x;
             
             In the first case, we will act just like we sort the points in Graham's algo.
             */
            
            if (!is_query_point && !you.is_query_point)
                return (x > you.x);
            
            /*
             In the second case, we will use the fact that first, the progressions value will decrease, and after some point will start to increase again.
             */
            
            if (is_query_point) {
                /*
                 In this case, my object is a query with param = x.
                 */
                if (you.par == master->end())
                    return false;
                if (you.par->x < you.x)
                    return ((you.y - you.par->y) <= x * (you.par->x - you.x));
                else
                    return ((you.y - you.par->y) >= x * (you.par->x - you.x));
            } else {
                /*
                 This is the previous case, vice-versa.
                 */
                if (par == master->end())
                    return true;
                if (par->x < x)
                    return (y - par->y) > you.x * (par->x - x);
                else
                    return (y - par->y) < you.x * (par->x - x);
            }
        }
    } ;
    
    typedef set<Point>::iterator position;
    
    set<Point> hull;
    
    /*
     The following routine removes the point, denoted by the iterator from the hull.
     */
    
    inline void remove_point (position me) {
        /*
         We only need to ensure that the next point has a correct predecessor pointer.
         */
        position next = me;
        ++next;
        if (next != hull.end())
            next->par = me->par;
        hull.erase(me);
    }
    
    inline bool is_bad_turn (position center) {
        position next = center, prev = center;
        ++next;
        if (center == hull.begin() || next == hull.end())
            return false;
        --prev;
        Point a = *prev, b = *center, c = *next;
        
        bool check_type = false;
        if (b.x < a.x)
            check_type ^= true;
        if (b.x < c.x)
            check_type ^= true;
        
        if (check_type)
            return (c.y - b.y+bc) * (b.x - a.x) - (a.y - b.y+bc) * (b.x - c.x) >=-1e-18;
        else
            return (c.y - b.y+bc) * (b.x - a.x) - (a.y - b.y+bc) * (b.x - c.x) <= 1e-18;
    }
    
public:
    
    inline void add_progression (long long x, long long y) {
        Point me(x, y, false, &hull);
        position pos = hull.find(me);
        if (pos != hull.end() && pos->y >= y)
            return ;
        if (pos != hull.end())
            remove_point(pos);
        hull.insert(me);
        
        /* Recalculate the pointer to the previous point for the next point.*/
        
        pos = hull.find(me);
        position next = pos;
        ++next;
        if (next != hull.end())
            next->par = pos;
        
        /* Calculate the pointer to the previous point for the current point.*/
        
        if (pos != hull.begin()) {
            position prev = pos;
            --prev;
            pos->par = prev;
        } else pos->par = hull.end();
        
        /*
         If the point is lying above the current hull, we can safely get rid of it.
         */
        
        if (is_bad_turn(pos)) {
            remove_point(pos);
            return ;
        }
        
        /*
         Delete the points that became unnecessary and lay after the current one.
         */
        
        while (next != hull.end() && is_bad_turn(next)) {
            remove_point(next);
            next = pos;
            ++next;
        }
        
        /*
         Delete the points that became unnecessary and lay before the current one.
         */
        
        if (pos != hull.begin()) {
            position prev = pos;
            --prev;
            while (pos != hull.begin() && is_bad_turn(prev)) {
                remove_point(prev);
                prev = pos;
                --prev;
            }
        }
    }
    
    inline long long get_min_value_at (long long x) {
        if (hull.size() == 0)
            return INF;
        Point me(x, x, true, &hull);
        position opt = hull.lower_bound(me);
        --opt;
        return opt->x * x + opt->y;
    }
    
};