//By myf
//#pragma comment(linker, "/STACK:16777216")  //C++
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <complex>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <list>

#define rep(i,n) for(int i=0;i<(n);i++)
#define REP(i,l,r) for(int i=(l);i<(r);i++)
#define fab(i,a,b) for(int i=(a);i<=(b);i++)
#define fba(i,b,a) for(int i=(b);i>=(a);i--)
//#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;
#define Sqr(x) (x)*(x)

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef LL T;
typedef pair<T,T> Point;

T cross(Point a,Point b,Point c){return (b.X-a.X)*(c.Y-a.Y)-(c.X-a.X)*(b.Y-a.Y);}

T dot(Point a,Point b,Point c){return (b.X-a.X)*(c.X-a.X)+(b.Y-a.Y)*(c.Y-a.Y);}

bool inpoly(Point a, Point *p, int n){
	int wn = 0;
	rep(i,n){
		Point p1 = p[i], p2 = p[(i + 1) % n];
		T s = cross(a, p1, p2);
		if (!s && dot(a, p1, p2) <= 0) return true;
		T d1 = p1.Y - a.Y, d2 = p2.Y - a.Y;
		if (s > 0 && d1 <= 0 && d2 > 0) ++wn;
		if (s < 0 && d2 <= 0 && d1 > 0) --wn;
	}
	return wn != 0;
}

const int N = 20000, M = 20;

int n, m, r;
Point p[N], poly[M];

const int K = 2;
struct kd {
	LL x[K];
	int id;
}t[N];

T dis2(kd a, kd b){
	T s = 0;
	rep(i,K) s += Sqr(a.x[i] - b.x[i]);
	return s;
}

struct cmpk {
	int k;
	cmpk(int _k): k(_k) {}
	bool operator()(kd a, kd b){ return a.x[k] < b.x[k]; }
};

void build(int l, int r, int d){
	if (r - l <= 1) return;
	int mid = (l + r) >> 1;
	nth_element(t + l, t + mid, t + r, cmpk(d));
	if (++d == K) d = 0;
	build(l, mid, d); build(mid + 1, r, d);
}

typedef priority_queue<pair<T, int> > heap;
void knn(int l, int r, int d, kd p, size_t k, heap &h){
	if (r - l < 1) return;
	int mid = (l + r) >> 1;
	h.push(make_pair(dis2(p, t[mid]), t[mid].id));
	if (h.size() > k) h.pop();
	T dx = p.x[d] - t[mid].x[d];
	if (++d == K) d = 0;
	if (dx < 0) {
		knn(l, mid, d, p, k, h);
		if (h.top().first > Sqr(dx)) knn(mid + 1, r, d, p, k, h);
	} else {
		knn(mid + 1, r, d, p, k, h);
		if (h.top().first > Sqr(dx)) knn(l, mid, d, p, k, h);
	}
}

void solve(){
	scanf("%d", &m);
	rep(i,m) {
	    int x,y;
	    scanf("%d%d",&x,&y);
	    poly[i]=MP(x,y);
	}
	int cnt = 0;
	rep(i,n){
		if (inpoly(p[i], poly, m)) {
			t[cnt].x[0] = p[i].X; t[cnt].x[1] = p[i].Y;
			t[cnt++].id = i + 1;
		}
	}
	build(0, cnt, 0);
	int q;
	scanf("%d", &q);
	while (q--) {
		kd p;
		scanf("%lld%lld", &p.x[0], &p.x[1]);
		heap h;
		knn(0, cnt, 0, p, 2, h);
		int a, b;
		b = h.top().second; h.pop();
		a = h.top().second;
		printf("%d %d\n", a, b);
	}
}

int main(){
	int dat;
	scanf("%d", &dat);
	rep(cas,dat){
		printf("Case #%d:\n", cas+1);
		scanf("%d",&n);
		rep(i,n){
            int x,y;
            scanf("%d%d",&x,&y);
            p[i]=MP(x,y);
		}
		scanf("%d", &r);
		rep(id,r){
			printf("Region %d\n", id+1);
			solve();
		}
	}
}
