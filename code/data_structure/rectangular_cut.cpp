//label : unfinished
//By myf
//#pragma comment(linker, "/STACK:16777216")  //C++
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <map>
#include <set>
#include <bitset>
#include <stack>
#include <complex>
#include <list>
#include <iomanip>

#define rep(i, n) for(int i = 0; i < (n); i++)
#define REP(i, l, r) for(int i = (l) ; i < (r); i++)
#define MP make_pair
#define PB push_back
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;
#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define F first
#define S second
#define X real()
#define Y imag()
#define Sqr(x) (x)*(x)
#define sign(x) ((x < -EPS) ? -1 : x > EPS)

using namespace std;

typedef long long LL;
typedef complex<double> Point;
typedef Point Vec;
typedef pair<Point, Point> Line;
typedef pair<int, int> pii;
//typedef complex<double> Comp;

const int N = 1000000;
const int MD = 14121413; 
const int INF = 0x3f3f3f3f;
const double PI = acos(-1.0);
const double EPS = 1E-6;

typedef vector<int> vi;

int n, m;
LL ans;
vi v[N][2];

void work(int lev, vi &a, vi &b);

bool is_in(vi &a, vi &b, vi &a2, vi &b2){
	rep(i, n){
		if (!(a2[i] <= a[i] && b[i] <= b2[i])) return false;
	}
	return true;
}

void dfs(vi &a, vi &b, vi &a2, vi &b2, int now_d, int lev){
	rep(i, n){
		if (a[i] == b[i]) return;
	}
	if (is_in(a, b, a2, b2)) return;
	if (now_d == n) return;
	int l = max(a[now_d], a2[now_d]);
	int r = min(b[now_d], b2[now_d]);
	int tmp_l = a[now_d], tmp_r = b[now_d];

	a[now_d] = l, b[now_d] = r;
	dfs(a, b, a2, b2, now_d + 1, lev);
	a[now_d] = tmp_l, b[now_d] = tmp_r;

	b[now_d] = l;
	work(lev + 1, a, b);
	b[now_d] = tmp_r;

	a[now_d] = r;
	work(lev + 1, a, b);
	a[now_d] = tmp_l;
}

void work(int lev, vi &a, vi &b){
	rep(i, n){
		if (a[i] == b[i]) return;
	}
	if (lev == m){
		LL ret = 1;
		rep(i, n) ret = ret * (b[i] - a[i]) % MD;
		ans = (ans + ret) % MD;
		return;
	}
	vi &a2 = v[lev][0], &b2 = v[lev][1];
	bool no_cover = false;
	rep(i, n){
		int l = max(a[i], a2[i]);
		int r = min(b[i], b2[i]);
		if (l >= r) no_cover = true;
	}
	if (no_cover) work(lev + 1, a, b);
	else dfs(a, b, a2, b2, 0, lev);
}

int main(){
	while(~scanf("%d%d", &m, &n)){
		rep(i, m){
			v[i][0].clear();
			rep(j, n){
				int x;
				scanf("%d", &x);
				v[i][0].PB(x);
			}
			v[i][1].clear();
			rep(j, n){
				int x;
				scanf("%d", &x);
				v[i][1].PB(x);
			}
			rep(j, n) if (v[i][0][j] > v[i][1][j]) swap(v[i][0][j], v[i][1][j]);
		}
		ans = 0;
		rep(i, m){
			work(i + 1, v[i][0], v[i][1]);
		}
		ans = (ans % MD + MD) % MD;
		printf("%d\n", (int)ans);
	}
	return 0;
}
