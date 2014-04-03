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
//typedef vector<int> VI;
//typedef complex<double> Comp;

const int N = 20000000;

bool vis[N];
vector<int> prime;

void prime_table(){
	REP(i, 2, N){
		if (!vis[i]) prime.PB(i);
		for(iint j = 0; i * prime[j] < N; j++){
			vis[i * prime[j]] = true;
			if (i % prime[j] == 0) break;
		}
	}
}
