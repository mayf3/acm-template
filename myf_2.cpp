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

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef int T;

const int N=50000+10,M=30;

int n,k;
VI E[N];
int tot,top,mi,root;
int size[N];
LL f[N];
map<LL,int> Q;
LL prime[N];
int sta[N][M];
LL q[N];
bool use[N];
LL base[M+1];

LL ans;

inline LL add(LL x,int sta[M]){
    LL y=0;
    rep(i,k){
        int tmp=(x%base[i+1])/base[i];
        tmp+=sta[i];
        tmp%=3;
        y+=(tmp*base[i]);
    }
    return y;
}

inline LL dec(LL a,LL b){
    LL y=0;
    rep(i,k){
        int tmp1=(a%base[i+1])/base[i];
        int tmp2=(b%base[i+1])/base[i];
        int tmp=(tmp1-tmp2+3)%3;
        y+=(tmp*base[i]);
    }
    return y;
}

inline LL add(LL a,LL b){
    LL y=0;
    rep(i,k){
        int tmp1=(a%base[i+1])/base[i];
        int tmp2=(b%base[i+1])/base[i];
        int tmp=(tmp1+tmp2)%3;
        y+=(tmp*base[i]);
    }
    return y;
}


void getVal(int x,LL val,int fa){
    q[top++]=val;
    rep(i,E[x].size()){
        int y=E[x][i];
        if (use[y]||y==fa)continue;
        getVal(y,add(val,sta[y]),x);
    }
}

void getRoot(int x,int fa){
    int big=-1;
    size[x]=1;
    rep(i,E[x].size()){
        int y=E[x][i];
        if (use[y]||y==fa) continue;
        getRoot(y,x);
        size[x]+=size[y];
        big=max(big,size[y]);
    }
    big=max(big,tot-size[x]);
    if (big<mi) mi=big,root=x;
}

void dfs(int x){
     tot=mi=size[x];
     getRoot(x,-1);
     x=root;
     use[x]=true;
     Q.clear();
     LL now=add(0,sta[x]);
     Q[now]=1;
     if (now==0) ans++;
     rep(i,E[x].size()){
         int y=E[x][i];
         if (use[y]) continue;
         top=0;
         getVal(y,add(0,sta[y]),x);
         rep(j,top){
             LL tmp=dec(0,q[j]);
             if (Q.count(tmp)) ans+=Q[tmp];
         }
         rep(j,top) Q[add(now,q[j])]++;
     }
     rep(i,E[x].size()) if (!use[E[x][i]]) dfs(E[x][i]);
}

int main(){
    base[0]=1;
    fab(i,1,M) base[i]=base[i-1]*3;
    while(~scanf("%d",&n)){
        Cls(use);
        rep(i,n) E[i].clear();
        scanf("%d",&k);
        rep(i,k) scanf("%I64d",prime+i);
        rep(i,n){
            LL x;
            scanf("%I64d",&x);
            Cls(sta[i]);
            rep(j,k) while (x%prime[j]==0) x/=prime[j],sta[i][j]++,sta[i][j]%=3;
        }
        rep(i,n-1){
            int x,y;
            scanf("%d%d",&x,&y);
            x--,y--;
            E[x].PB(y);
            E[y].PB(x);
        }
        size[0]=n;
        ans=0;
        dfs(0);
        printf("%I64d\n",ans);
    }
    return 0;
}
