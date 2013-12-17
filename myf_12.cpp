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

const int M = 222;
const int MD=1000000007;

LL n;
int u,d;
int p[M],q[M];
bool use[M];
LL a[M],b[M];

int calc(LL n,int m,LL a[],LL c[],int p=MD){
    LL v[M]={1%p},u[M<<1],msk=!!n;
    for(LL i=n;i>1;i>>=1) msk<<=1;
    for(LL x=0;msk;msk>>=1,x<<=1){

        fill_n(u,m<<1,0);
        int b=!!(n&msk);
        x|=b;
        if (x<m) u[x]=1%p;
        else{
            rep(i,m) for(int j=0,t=i+b;j<m;++j,++t) u[t]+=v[i]*v[j],u[t]%=p;
            fba(i,(m<<1)-1,m) for(int j=0,t=i-m;j<m;++j,++t) u[t]+=c[j]*u[i],u[t]%=p;
        }
        copy(u,u+m,v);
    }
    LL ret=0;
    rep(i,m) ret+=v[i]*a[i],ret%=p;
    return ret;
}

int main(){
    while(~scanf("%I64d",&n)){
        Cls(a);
        Cls(b);
        Cls(use);
        scanf("%d",&u);
        rep(i,u) scanf("%d",p+i);
        scanf("%d",&d);
        rep(i,d) scanf("%d",q+i);
        int top=0;
        rep(i,d) top=max(top,q[i]+1),use[q[i]]=true;
        b[0]=1;
        REP(i,1,top){
            rep(j,u) if (i>=p[j]) b[i]+=b[i-p[j]],b[i]%=MD;
        }
        rep(i,top) if (!use[i]) b[i]=0;
        a[0]=1;
        REP(i,1,top){
            fab(j,1,i) a[i]+=a[i-j]*b[j],a[i]%=MD;
        }
        reverse(b,b+top);
        printf("%d\n",calc(n,top-1,a,b));
    }
    return 0;
}
