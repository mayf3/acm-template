//By myf
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
#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++)
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef int T;
typedef complex<double> Comp;
typedef Comp cp;

const double PI = acos(-1);
const Comp I(0, 1);

const int N = 1<<18;
Comp tmp[N];
Comp a[N] = { }, b[N] = { };
int n,m,d;
LL ans;
LL c[N];
int v[N];

void fft(Comp *a,int n,int f=1){
    double arg = PI;
    for(int k = n >> 1; k; k >>= 1, arg *= 0.5) {
        cp wm(cos(arg), f * sin(arg)), w(1, 0);
        for (int i = 0; i < n; i += k, w *= wm) {
            int p = i << 1;
            if (p >= n) p -= n;
            for (int j = 0; j < k; ++j) tmp[i + j] = a[p + j] + w * a[p + k + j];
        }
        rep(i,n) a[i] = tmp[i];
    }
}

int calc(int n){
	fft(a,n,1);
	fft(b,n,1);
	rep(i,n) a[i] = a[i]*b[i];
	fft(a,n,-1);
	rep(i,n) a[i] /= n;
}


int main(){
	int T;
	scanf("%d",&T);
	rep(cas,T){
	    scanf("%d",&n);
	    rep(i,n) scanf("%d",v+i);
	    int ma=0;
	    rep(i,n) ma=max(ma,v[i]);
	    rep(i,N) a[i]=b[i]=Comp(0,0);
	    rep(i,n) b[v[i]]=a[v[i]]+=Comp(1,0);
	    int top=1;
	    while(top<=ma*2) top*=2;
	    calc(top);
	    LL ans=(LL)n*(n-1)*(n-2)/6;
	    rep(i,top) c[i]=(LL)(a[i].real()+0.4);
	    rep(i,n) c[v[i]*2]--;
	    rep(i,top) c[i]/=2;
	    //rep(i,top) cout<<i <<' '<<c[i]<<endl;
	    REP(i,1,top) c[i]+=c[i-1];
	    rep(i,n) ans-=c[v[i]];
		printf("%.7lf\n",ans/((double)n*(n-1)*(n-2)/6));
	}
	return 0;
}
