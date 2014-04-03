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
typedef int T;

const double eps = 1e-8;
const double inf = 1e15;
const int N = 20;

#define OPTIMAL -1
#define UNBOUNDED -2
#define FEASIBLE -3
#define INFEASIBLE -4
#define PIVOT_OK 1

int basic[N], row[N], col[N];
double c0[N];

double dcmp(double x){
	if (x > eps) return 1;
	if (x < -eps) return -1;
	return 0;
}

int Pivot(int n, int m, double *c, double a[N][N],double *rhs, int &x, int &y){
	double mi = inf;int k = -1;
	fab(j,0,n){
	    if (basic[j] || dcmp(c[j])<=0 ) continue;
		if (k < 0 || dcmp(c[j] - c[k]) > 0) k = j;
	}
	y = k; if (k < 0) return OPTIMAL;
	k=-1;
	fab(i,1,m){
	    if ( dcmp(a[i][y]) <= 0) continue;
		if (dcmp(rhs[i]/a[i][y]-mi) < 0) mi = rhs[i]/a[i][y],k = i;
	}
	x = k;
	return (k<0)?UNBOUNDED:PIVOT_OK;
}

int PhaseII(int n, int m, double *c, double a[N][N],double *rhs, double &ans, int PivotIndex){
	int i,j,k;
	double tmp;
	while (k = Pivot(n,m,c,a,rhs,i,j),k==PIVOT_OK || PivotIndex) {
		if (PivotIndex) {j=0; i=PivotIndex; PivotIndex=0;}
		basic[row[i]] = 0; col[row[i]]=0; basic[j]=1; col[j] = i; row[i]=j;
		tmp = a[i][j];
		fab(k,0,n) a[i][k]/=tmp; rhs[i] /=tmp;
		fab(k,1,m) if (k!=i && dcmp(a[k][j])) {
			tmp = -a[k][j];
			fab(l,0,m) a[k][l]+=tmp*a[i][l];
			rhs[k] += tmp * rhs[i];
		}
		tmp=-c[j];
		fab(l,0,n) c[l]+=a[i][l]*tmp;ans-=tmp*rhs[i];
	}
	return k;
}

int PhaseI(int n, int m,double *c, double a[N][N], double *rhs,double &ans){
	int k = -1;
	double tmp,mi=0,ans0 = 0;
	fab(i,1,m) if (dcmp(rhs[i]-mi) < 0) mi=rhs[i],k=i;
	if (k<0) return FEASIBLE;
	fab(i,1,m)a[i][0] = -1;
	fab(j,1,n) c0[j] = 0; c0[0] = -1;
	PhaseII(n, m, c0, a, rhs, ans0, k);
	if (dcmp(ans0) < 0) return INFEASIBLE;
	fab(i,1,m) a[i][0] = 0;
	fab(j,1,m) if (dcmp(c[j]) && basic[j]) {
		tmp = c[j]; ans += rhs[col[j]] * tmp;
		fab(i,0,n) c[i] -= tmp*a[col[j]][i];
	}
	return FEASIBLE;
}

int simplex(int n, int m, double *c, double a[N][N],double *rhs, double &ans, double *x){
	int k;
	fab(i,1,m){
	    fab(j,n+1,n+m) a[i][j] = 0;
		a[i][n+i] = 1; a[i][0] = 0;
		row[i] = n + i; col[n + i] = i;
	}
	k = PhaseI(n+m, m, c, a, rhs, ans);
	if (k == INFEASIBLE) return k;
	k = PhaseII(n+m,m,c,a,rhs,ans,0);
	fab(j,0,n+m) x[j] = 0;
	fab(i,1,m) x[row[i]] = rhs[i];
	return k;
}

int n, m;
double c[N], ans, a[N][N], rhs[N], x[N];

int main(){
	n=4,m=10,ans=0;
	fab(i,1,n) c[i]=1;
	fab(i,1,m){
		fab(j,1,n) a[i][j]=0;
		rhs[i]=0;
	}
	a[1][1]=1;a[1][4]=1;
	a[2][1]=1;a[2][3]=1;
	a[3][3]=1;a[3][4]=1;
	a[4][2]=1;a[4][3]=1;
	a[5][2]=1;a[5][4]=1;
	a[6][1]=1;a[6][2]=1;
	a[7][1]=-1;
	a[8][2]=-1;
	a[9][3]=-1;
	a[10][4]=-1;
	fab(i,1,6) scanf("%lf",&rhs[i]);
	simplex(n,m,c,a,rhs,ans,x);
    fab(i,1,n) printf(i==n?"%lf\n":"%lf ",x[i]);
	return 0;
}
