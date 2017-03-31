


const double EPS =10e-11; // 10^-11

class VagueDouble{
public:
	double vd;
	VagueDouble(){ vd=nan(""); }
	VagueDouble(const double d){ vd=d; }
	inline VagueDouble operator-()const{ return VagueDouble(-vd); }
	inline VagueDouble operator+(const VagueDouble VD)const{ return VagueDouble(vd+VD.vd); }
	inline VagueDouble operator-(const VagueDouble VD)const{ return VagueDouble(vd-VD.vd); }
	inline VagueDouble operator*(const VagueDouble VD)const{ return VagueDouble(vd*VD.vd); }
	inline VagueDouble operator/(const VagueDouble VD)const{ return VagueDouble(vd/VD.vd); }
	inline VagueDouble operator+(const double d)const{ return VagueDouble(vd+d); }
	inline VagueDouble operator-(const double d)const{ return VagueDouble(vd-d); }
	inline VagueDouble operator*(const double d)const{ return VagueDouble(vd*d); }
	inline VagueDouble operator/(const double d)const{ return VagueDouble(vd/d); }
	inline bool operator< (const VagueDouble VD)const{ return vd+EPS<VD.vd && vd+EPS*abs(vd)<VD.vd; }
	inline bool operator> (const VagueDouble VD)const{ return vd-EPS>VD.vd && vd-EPS*abs(vd)>VD.vd; }
	inline bool operator==(const VagueDouble VD)const{ return !(*this<VD)&&!(*this>VD); }
	inline bool operator!=(const VagueDouble VD)const{ return  (*this<VD)|| (*this>VD); }
	inline bool operator<=(const VagueDouble VD)const{ return !(*this>VD); }
	inline bool operator>=(const VagueDouble VD)const{ return !(*this<VD); }
	inline bool operator< (const double d)const{ return *this< VagueDouble(d); }
	inline bool operator> (const double d)const{ return *this> VagueDouble(d); }
	inline bool operator==(const double d)const{ return *this==VagueDouble(d); }
	inline bool operator!=(const double d)const{ return *this!=VagueDouble(d); }
	inline bool operator<=(const double d)const{ return *this<=VagueDouble(d); }
	inline bool operator>=(const double d)const{ return *this>=VagueDouble(d); }
	inline void operator+=(const VagueDouble VD){ *this=*this+VD; }
	inline void operator-=(const VagueDouble VD){ *this=*this-VD; }
	inline void operator*=(const VagueDouble VD){ *this=*this*VD; }
	inline void operator/=(const VagueDouble VD){ *this=*this/VD; }
	inline friend VagueDouble operator+(const double d,const VagueDouble &VD){ return VagueDouble(d+VD.vd); }
	inline friend VagueDouble operator-(const double d,const VagueDouble &VD){ return VagueDouble(d-VD.vd); }
	inline friend VagueDouble operator*(const double d,const VagueDouble &VD){ return VagueDouble(d*VD.vd); }
	inline friend VagueDouble operator/(const double d,const VagueDouble &VD){ return VagueDouble(d/VD.vd); }
	inline friend bool operator< (const double d,const VagueDouble &VD){ return VagueDouble(d)< VD; }
	inline friend bool operator> (const double d,const VagueDouble &VD){ return VagueDouble(d)> VD; }
	inline friend bool operator==(const double d,const VagueDouble &VD){ return VagueDouble(d)==VD; }
	inline friend bool operator<=(const double d,const VagueDouble &VD){ return VagueDouble(d)<=VD; }
	inline friend bool operator>=(const double d,const VagueDouble &VD){ return VagueDouble(d)>=VD; }
	inline friend std::ostream& operator<<(std::ostream& os,const VagueDouble VD){ os<<VD.vd; return os; }
	inline friend std::istream& operator>>(std::istream& is,VagueDouble &VD){ double d; is>>d; VD=VagueDouble(d); return is; }
	inline friend VagueDouble abs(VagueDouble VD){ return (VD.vd<0)?(-VD.vd):VD.vd; }
	inline friend VagueDouble sqr(VagueDouble VD){ return VD.vd*VD.vd; }
	inline friend VagueDouble sqr(double d){ return d*d; }
	inline friend VagueDouble sqrt(VagueDouble VD){ return VagueDouble(sqrt(VD.vd)); }
	inline friend bool isnan(VagueDouble VD){ return isnan(VD.vd); }	
	inline double to_double(){ return vd; }
};
const VagueDouble PI = acos(-1);

class P{
public:
	VagueDouble x,y;
	P(){ x=nan(""); y=nan(""); }
	P(double _x,double _y){ x=_x; y=_y; }
	P(VagueDouble _x,VagueDouble _y){ x=_x; y=_y; }
	inline P operator-()const{ return P(-x,-y); }
	inline P operator+(const P p)const{ return P(x+p.x,y+p.y); }
	inline P operator-(const P p)const{ return P(x-p.x,y-p.y); }
	inline P operator*(const double d)const{ return P(x*d,y*d); }
	inline P operator/(const double d)const{ return P(x/d,y/d); }
	inline P operator*(const VagueDouble d)const{ return P(x*d,y*d); }
	inline P operator/(const VagueDouble d)const{ return P(x/d,y/d); }
	friend inline P operator*(const double d,const P p){ return P(p.x*d,p.y*d); }
	friend inline P operator*(const VagueDouble d,const P p){ return P(p.x*d,p.y*d); }
	inline bool operator< (const P &p)const{ return (x<p.x || (x==p.x && y<p.y)); }
	inline bool operator> (const P &p)const{ return (x>p.x || (x==p.x && y>p.y)); }
	inline bool operator==(const P &p)const{ return (x==p.x)&&(y==p.y); }
	inline bool operator!=(const P &p)const{ return (x!=p.x)||(y!=p.y); }
	inline bool operator<=(const P &p)const{ return !(*this>p); }
	inline bool operator>=(const P &p)const{ return !(*this<p); }
	inline void operator+=(const P &p){ *this=*this+p; }
	inline void operator-=(const P &p){ *this=*this-p; }
	inline void operator*=(const double &p){ *this=*this*p; }
	inline void operator/=(const double &p){ *this=*this/p; }	
	inline VagueDouble operator*(const P p)const{ return x*p.x+y*p.y; }
	inline VagueDouble operator^(const P p)const{ return x*p.y-y*p.x; }
	friend std::ostream& operator<<(std::ostream& os,const P &p){ os<<p.x.vd<<" "<<p.y.vd; return os; }
	friend std::istream& operator>>(std::istream& is,P &p){ double _x,_y; is>>_x>>_y; p=P(_x,_y); return is; }
	inline friend bool isnan(P p){ return isnan(p.x); }
	inline friend VagueDouble findArea_Point_Point_Point(P p0,P p1,P p2){ return abs((p1-p0)^(p2-p0))*0.5; }
	inline friend VagueDouble dist(P p0,P p1){ return sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)); }
};


VagueDouble findDist_Line_Point(P l0,P l1,P p){ 
	if(l0==l1) return dist(l0,p);
	return abs((p-l0)^(l1-l0))/dist(l0,l1); 
}

inline bool isPointOnLine(P p,P l0,P l1){ return findDist_Line_Point(l0,l1,p)==0; }
inline bool isPointOnSeg(P p,P l0,P l1){ 
	if(p.x<min(l0.x,l1.x) || max(l0.x,l1.x)<p.x) return false;
	if(p.y<min(l0.y,l1.y) || max(l0.y,l1.y)<p.y) return false;
	return findDist_Line_Point(l0,l1,p)==0;
}

VagueDouble findAngle_Line_Line(P from0,P to0,P from1,P to1){ // 0 <= res < PI * 2.0
	P v0=to0-from0,v1=to1-from1;
	VagueDouble res=atan2(v1.y.vd,v1.x.vd)-atan2(v0.y.vd,v0.x.vd);
	return (0<=res)?res:(res+PI*2.0);
}


P nearestPoint_LinePoint(P l0,P l1,P p){
	if(l0==l1) return P();
	VagueDouble a=l1.y-l0.y;
	VagueDouble b=l0.x-l1.x;
	VagueDouble c=-(a*l0.x+b*l0.y);
	VagueDouble d=(a*p.x+b*p.y+c)/(a*a+b*b);
	return P(p.x-a*d,p.y-b*d);
}

P linePos_Line_X(P l0,P l1,VagueDouble x){
	VagueDouble dx=l0.x-l1.x;
	VagueDouble dy=l0.y-l1.y;
	if(dx==0) return P();
	VagueDouble d = 1.0/dx;
	return P(x,x*dy*d+(l0.y-l0.x*dy*d));
}

P linePos_Line_Y(P l0,P l1,VagueDouble y){
	VagueDouble dx=l0.x-l1.x;
	VagueDouble dy=l0.y-l1.y;
	if(dy==0) return P();
	VagueDouble d = 1.0/dy;
	return P(y*dx*d+(l0.x-l0.y*dx*d),y);
}

bool isPointAboveLine(P p0,P l0,P l1){
	P p=linePos_Line_X(l0,l1,p0.x);
	if(isnan(p)) return l1.x<p0.x;
	return p.y<p0.y;
}

bool isPointBelowLine(P p0,P l0,P l1){
	P p=linePos_Line_X(l0,l1,p0.x);
	if(isnan(p)) return p0.x<l1.x;
	return p0.y<p.y;
}

P findCross_Line_Line(P a0,P a1,P b0,P b1){
	VagueDouble dx0=a1.x-a0.x;
	VagueDouble dy0=a1.y-a0.y;
	VagueDouble dx1=b1.x-b0.x;
	VagueDouble dy1=b1.y-b0.y;
	VagueDouble den=dx1*dy0-dy1*dx0;
	if(den==0) return P();
	VagueDouble da=((a0.x-b0.x)*dy0-(a0.y-b0.y)*dx0)/den;
	return P(b0.x+dx1*da,b0.y+dy1*da);
}

		
P findCross_Seg_Seg(P a0,P a1,P b0,P b1){
	VagueDouble dx0=a1.x-a0.x;
	VagueDouble dy0=a1.y-a0.y;
	VagueDouble dx1=b1.x-b0.x;
	VagueDouble dy1=b1.y-b0.y;
	VagueDouble den=dx1*dy0-dy1*dx0;
	if(den==0) return P();
	VagueDouble da=((a0.x-b0.x)*dy0-(a0.y-b0.y)*dx0)/den;
	if(da<0 || 1<da) return P();
	P res=P(b0.x+dx1*da,b0.y+dy1*da);
	if(res.x<min(a0.x,a1.x) || max(a0.x,a1.x)<res.x) return P();
	if(res.y<min(a0.y,a1.y) || max(a0.y,a1.y)<res.y) return P();
	return res;
}


class Polygon{
private:
public:
	vector<P> points;	
	Polygon(){}
	Polygon(vector<P> vec){ points=vec; }
	Polygon(li num){ points.resize(num); }
	friend std::ostream& operator<<(std::ostream& os,Polygon &poly){ rep(i,sz(poly)) os<<poly[i]<<" "; return os; }
	friend std::istream& operator>>(std::istream& is,Polygon &poly){ rep(i,sz(poly)) is>>poly[i]; return is; }
	inline li size(){ return sz(points); }
	P& operator[](li pos){ return points[pos]; }
	inline void add(P point){ points.pb(point); }

	friend bool isPointOnPoly(P p,Polygon &poly){
		li n=sz(poly.points);
		if(n==0) return false;
		if(isPointOnSeg(p,poly.points[n-1],poly.points[0])) return true;
		rep(i,n-1)if(isPointOnSeg(p,poly.points[i],poly.points[i+1])) return true;
		return false;
	}
	
	friend bool isPointInPoly(P p,Polygon &poly){
		li n=sz(poly.points),cnt=0;
		if(n==0) return false;
		P right_point=p;
		VagueDouble margin=5.0;
		rep(i,n) right_point.x=max(right_point.x+margin,poly.points[i].x);
		for(li i=0,next=1;i<n;i++,next++){
			if(next==n) next=0;
			if(0<(poly.points[i].y-p.y)*(poly.points[next].y-p.y)) continue;
			P cross=findCross_Seg_Seg(poly.points[i],poly.points[next],p,right_point);
			if(isnan(cross)) continue;
			cnt++;
			if(cross==p) return true;
			if(cross==poly.points[i]){
				li prev=(i==0)?(n-1):(i-1);
				VagueDouble dy0=poly.points[prev].y-p.y;
				VagueDouble dy1=poly.points[next].y-p.y;
				if(dy0*dy1<0) cnt++;
			}
		}
		return (cnt&1);
	}
	
	friend bool isPointOutPoly(P p,Polygon &poly){ return !isPointInPoly(p,poly); }
	
	friend void convexReform(Polygon &poly){ // clockwise from the most left point.
		li k=0;
		if(sz(poly.points)<=1) return;
		sort(poly.points.begin(),poly.points.end());
		vector<P> ans(2*sz(poly.points));
		for(li i=0;i<sz(poly.points);ans[k++]=poly.points[i++]){
			while(2<=k && 0<=((ans[k-1]-ans[k-2])^(poly.points[i]-ans[k-2]))) k--;
		}
		for(li i=sz(poly.points)-2,t=k+1;0<=i;ans[k++]=poly.points[i--]){
			while(t<=k && 0<=((ans[k-1]-ans[k-2])^(poly.points[i]-ans[k-2]))) k--;
		}
		ans.resize(k-1);
		poly.points=ans;
	}
	
	friend VagueDouble convexArea(Polygon &poly){
		VagueDouble res=0;
		rep(i,sz(poly.points)-1) res+=findArea_Point_Point_Point(poly.points[0],poly.points[i],poly.points[i+1]);
		return res;
	}
	
	
	friend inline bool isPointInConvexSub(Polygon &poly,bool flag){ poly.points.pop_back(); return flag; }
	friend bool isPointInConvex(P p,Polygon &poly){
		poly.points.pb(poly.points[0]);
		li low=0,high=sz(poly.points);
		while(low+1<high){
			li mid0=(low*2+high)/3;
			li mid1=(low+2*high)/3;
			if(poly.points[mid0]<poly.points[mid1]) low=mid0+1;
			else high=mid1;
		}
//cout<<poly.points[0].x<<" "<<p.x<<endl;
		if(p.x<=poly.points[0].x) return isPointInConvexSub(poly,false);
		if(poly.points[low].x<=p.x) return isPointInConvexSub(poly,false);
		li left=0,right=low;
		while(left+1<right){
			li mid=(left+right)/2;
			if(p.x<poly.points[mid].x) right=mid;
			else left=mid;
		}
		if(!isPointBelowLine(p,poly.points[left],poly.points[left+1])) return isPointInConvexSub(poly,false);
		left=sz(poly.points)-1,right=low;
		while(right+1<left){
			li mid=(left+right)/2;
			if(p.x<=poly.points[mid].x) right=mid;
			else left=mid;
		}
		if(!isPointAboveLine(p,poly.points[right],poly.points[right+1])) return isPointInConvexSub(poly,false);
		return isPointInConvexSub(poly,true);
	}
};

class Circle{
public:
	P center;
	VagueDouble r;
	Circle(){ center=P(); r=VagueDouble(); }
	Circle(P _center,VagueDouble _r){ center=_center; r=_r; };
	Circle(P p0,P p1,P p2){ 
		if(p0==p1){ center=(p0+p2)*0.5; r=dist(p0,p2)*0.5; return; }
		if(p0==p2){ center=(p0+p1)*0.5; r=dist(p0,p2)*0.5; return; }
		if(p1==p2){ center=(p0+p2)*0.5; r=dist(p0,p2)*0.5; return; }
		P mid0=(p0+p1)*0.5;
		P mid1=(p0+p2)*0.5;
		P _mid0=mid0+P((p1-p0).y,(p0-p1).x);
		P _mid1=mid1+P((p2-p0).y,(p0-p2).x);
		center=findCross_Line_Line(mid0,_mid0,mid1,_mid1);
		r=dist(center,p0);
	}
	
};

VagueDouble area(Circle C){ return C.r*C.r*PI; }

vector<P> findCross_Circle_Line(Circle C,P l0,P l1){
	if(l0==l1) return vector<P>();
	if(C.r<findDist_Line_Point(l0,l1,C.center)) return vector<P>();
	VagueDouble a=l1.y-l0.y;
	VagueDouble b=l0.x-l1.x;
	VagueDouble c=-(a*l0.x+b*l0.y);
	VagueDouble d=a*C.center.x+b*C.center.y+c;
	VagueDouble ab=a*a+b*b;
	VagueDouble e=sqrt(ab*C.r*C.r-d*d);
	vector<P> res;
	res.pb(P((-a*d+b*e)/ab+C.center.x,(-b*d-a*e)/ab+C.center.y));
	res.pb(P((-a*d-b*e)/ab+C.center.x,(-b*d+a*e)/ab+C.center.y));
	return res;
}

vector<P> findCross_Circle_Circle(Circle c0,Circle c1){
	if(c0.center==c1.center) return vector<P>();
	VagueDouble dx=c0.center.x-c1.center.x;
	VagueDouble dy=c0.center.y-c1.center.y;
	VagueDouble base=dx*dx+dy*dy;
	VagueDouble a=(base+c1.r*c1.r-c0.r*c0.r)/2.0;
	VagueDouble b=base*c1.r*c1.r-a*a;
	if(b<0) return vector<P>();
	b=sqrt(b);
	vector<P> res;
	res.pb(P((a*dx+dy*b)/base+c1.center.x,(a*dy-dx*b)/base+c1.center.y));
	res.pb(P((a*dx-dy*b)/base+c1.center.x,(a*dy+dx*b)/base+c1.center.y));
	return res;
}

vector<P> findTanget_Circle_Point(Circle c,P p){
	VagueDouble dx=p.x-c.center.x;
	VagueDouble dy=p.y-c.center.y;
	VagueDouble d=dx*dx+dy*dy;
	VagueDouble e=d-c.r*c.r;
	if(e<0) return vector<P>();
	e=sqrt(e);
	VagueDouble f=c.r/d;
	VagueDouble dxcr=dx*c.r;
	VagueDouble dycr=dy*c.r;
	VagueDouble dye=dy*e;
	VagueDouble dxe=dx*e;
	vector<P> res;
	res.pb(f*P(dxcr+dye,dycr-dxe)+c.center);
	res.pb(f*P(dxcr-dye,dycr+dxe)+c.center);
	return res;
}


pair<P,P> findCircleCircleTangentSub(Circle c0,Circle c1,P p){
	vector<P> v=findTanget_Circle_Point(c1,p);
	rep(i,sz(v)){
		if(findDist_Line_Point(p,v[i],c0.center)!=c0.r) continue;
		if(findDist_Line_Point(p,v[i],c1.center)!=c1.r) continue;
		return MP(p,v[i]);
	}
	cout<<"reach here findCircleCircleTangentSub"<<endl;
	return MP(P(),P());
}
			

vector<pair<P,P> > findOutsideTangent_Circle_Circle(Circle c0,Circle c1){
	VagueDouble dx=c1.center.x-c0.center.x;
	VagueDouble dy=c1.center.y-c0.center.y;
	VagueDouble d=dx*dx+dy*dy;
	VagueDouble e=d-sqr(c0.r-c1.r);
	if(e<0) return vector<pair<P,P> >();
	e=sqrt(e);
	vector<P> vec;
	vector<pair<P,P> > res;
	vec.pb(c0.r*P(dx*(c0.r-c1.r)+dy*e,dy*(c0.r-c1.r)-dx*e)/d+c0.center);
	vec.pb(c0.r*P(dx*(c0.r-c1.r)-dy*e,dy*(c0.r-c1.r)+dx*e)/d+c0.center);
	rep(i,sz(vec)) res.pb(findCircleCircleTangentSub(c0,c1,vec[i]));
	return res;
}


vector<pair<P,P> > findInsideTangent_Circle_Circle(Circle c0,Circle c1){
	VagueDouble dx=c1.center.x-c0.center.x;
	VagueDouble dy=c1.center.y-c0.center.y;
	VagueDouble d=dx*dx+dy*dy;
	VagueDouble e=d-sqr(c0.r+c1.r);
	if(e<0) return vector<pair<P,P> >();
	e=sqrt(e);
	vector<P> vec;
	vector<pair<P,P> > res;
	vec.pb(c0.r*P(dx*(c0.r+c1.r)+dy*e,dy*(c0.r+c1.r)-dx*e)/d+c0.center);
	vec.pb(c0.r*P(dx*(c0.r+c1.r)-dy*e,dy*(c0.r+c1.r)+dx*e)/d+c0.center);
	rep(i,sz(vec)) res.pb(findCircleCircleTangentSub(c0,c1,vec[i]));
	return res;
}


class Sector : public Circle{
public:
	VagueDouble start_angle,end_angle;
	Sector(){}
	Sector(VagueDouble _r,P from,P to){
		r=_r;
		P line=to-from;
		line=P(-line.y,line.x);
		VagueDouble d=dist(from,to);
		VagueDouble h=sqrt(sqr(r)-sqr(d/2));
		center=(from+to)/2.0;
		center+=line*h/dist(from,to);
		line=from-center;
		start_angle=atan2(line.y.to_double(),line.x.to_double());
		while(start_angle<0) start_angle+=PI*2.0;
		line=to-center;
		end_angle=atan2(line.y.to_double(),line.x.to_double());
		while(end_angle<0) end_angle+=PI*2.0;
		VagueDouble dif0=end_angle-start_angle;
		VagueDouble dif1=start_angle-end_angle;
		while(dif0<0) dif0+=2.0*PI;
		while(dif1<0) dif1+=2.0*PI;
		if(dif1<dif0) swap(start_angle,end_angle);
	}
	inline VagueDouble area(){ 
		VagueDouble angle=end_angle-start_angle;
		while(angle<0) angle+=PI*2.0;
		return r*r*angle/2.0; 
	}
};
