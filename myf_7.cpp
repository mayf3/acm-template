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

const int N=500010;
const int K = 2;

int n;
T a[2], b[2], c[2];

struct kd {
	T x[K];
	int id;
	bool del;
}t[N];

struct cmpk {
	int k;
	cmpk(int _k) : k(_k) {}
	bool operator()(kd a,kd b){ return a.x[k] < b.x[k];}
};

T ans;
int pos[N];

LL dis2(kd a,kd b){
	LL d = 0;
	rep(i,K) d += Sqr(a.x[i] - b.x[i]);
	return d;
}

void buildkd(int l, int r, int d){
	if (r - l <= 1) return;
	int mid = (l + r) >> 1;
	nth_element(t + l, t + mid, t + r, cmpk(d));
	if (++d == K) d = 0;
	buildkd(l, mid, d); buildkd(mid + 1, r, d);
}

void knn(int l, int r, int d, kd p){
	if (r - l < 1) return;
	int mid = (l + r) >> 1;
	if (!t[mid].del) ans = min(ans, dis2(p, t[mid]));
	LL dx = p.x[d] - t[mid].x[d];
	if (++d == K) d = 0;
	if (dx < 0) {
		knn(l, mid, d, p);
		if (ans > Sqr(dx)) knn(mid + 1, r, d, p);
	} else {
		knn(mid + 1, r, d, p);
		if (ans > Sqr(dx)) knn(l, mid, d, p);
	}
}

void init(){
	scanf("%d", &n);
	rep(i,K) scanf("%I64d%I64d%I64d", &a[i], &b[i], &c[i]);
	t[0].x[0] = b[0] % c[0]; t[0].x[1] = b[1] % c[1]; t[0].id = 0; t[0].del = true;
	REP(i,1,n){
		rep(k,K) t[i].x[k] = (t[i - 1].x[k] * a[k] + b[k]) % c[k];
		t[i].id = i; t[i].del = true;
	}
}

T solve(){
	LL ret = 0;
	ans = ((unsigned long long)1<<63)-1;
	rep(i,n) pos[t[i].id] = i;
	t[pos[0]].del = false;
	REP(i,1,n){
		knn(0, n, 0, t[pos[i]]);
		t[pos[i]].del = false;
		ret += ans;
	}
	return ret;
}

int main(){
	int dat;
	scanf("%d", &dat);
	while (dat--) {
		init();
		buildkd(0, n, 0);
		printf("%I64d\n", solve());
	}
}
