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
#define fab(i,a,b) for(int i = (a); i <= (b); i++)
#define fba(i,b,a) for(int i = (b); i >= (a); i--)
#define MP make_pair
#define PB push_back
//#define PF push_front
//#define F front
//#define B back
#define X first
#define Y second
#define Sqr(x) (x)*(x)
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;
//#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
//#define X real()
//#define Y imag()

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef int T;
typedef vector<int> VI;
//typedef complex<double, double> Comp;

const int N = 1000000, MD = 1000000007;

int n, m;
vector<PII> E[N];
int dist[N];

int spfa(int s, int e){
	static deque<int> Q;
	static bool inque[N];
	Cls(inque);
	memset(dist, -1, sizeof dist);
	dist[s] = 0;
	Q.PB(s);
	inque[s] = true;
	int x, y, c;
	while(!Q.empty()){
		x = Q.front();
		Q.pop_front();
		inque[x] = false;
		rep(i, E[x].size()){
			y = E[x][i].X;
			c = E[x][i].Y;
			if (dist[y] == -1 || dist[y] != -1 && dist[y] > dist[x] + c){
				dist[y] = dist[x] + c;
				if (!inque[y]){
					Q.PB(y);
					inque[y] = true;
				}
			}
		}
	}
	return dist[e];
}

int main(){
	while(~scanf("%d%d", &n, &m)){
		rep(i, n) E[i].clear();
		int x, y, c;
		rep(i, m){
			scanf("%d%d%d", &x, &y, &c);
			x--, y--;
			E[x].PB(MP(y, c));
			E[y].PB(MP(x, c));
		}
		printf("%d\n", spfa(0, n - 1));
	}
	return 0;
}
