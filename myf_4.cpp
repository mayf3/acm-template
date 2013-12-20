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
#define inf 0xc0c0c0c0
#define lch ch[x][0]
#define rch ch[x][1]

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef int T;

const int N=333333;

int n;
int pre[N],fa[N],fat[N],val[N],ma[N],ch[N][2];
bool black[N];
multiset<int> Q[N];
VI E[N];

inline void up(int x){ma[x]=max(max(val[x],*Q[x].rbegin()),max(ma[lch],ma[rch]));}

inline void rot(int id,int tp){
    static int k;
    k=pre[id];
    ch[k][tp^1]=ch[id][tp];
    if(ch[id][tp]) pre[ch[id][tp]]=k;
    if(pre[k]) ch[pre[k]][k==ch[pre[k]][1]]=id;
    pre[id]=pre[k];
    ch[id][tp]=k;
    pre[k]=id;
    up(k);
}

inline void splay(int x){
    if (!pre[x]) return;
    int tmp;
    for(tmp=x;pre[tmp];tmp=pre[tmp]);
    for(swap(fa[x],fa[tmp]);pre[x];rot(x,x==ch[pre[x]][0]));
    up(x);
}

inline int access(int x){
    int nt;
    for(nt=0;x;x=fa[x]){
        splay(x);
        if (rch){
            fa[rch]=x;
            pre[rch]=0;
            Q[x].insert(ma[rch]);
        }
        rch=nt;
        if (nt){
            fa[nt]=0;
            pre[nt]=x;
            Q[x].erase(Q[x].find(ma[nt]));
        }
        up(nt=x);
    }
    return nt;
}

void make(int x,int f){
    fat[x]=f;
    rep(i,E[x].size()) if (E[x][i]!=f) make(E[x][i],x);
    int t;
    up(x+n);up(x+2*n);
    fa[t=x+(1+black[x])*n]=x;
    Q[x].insert(*Q[t].rbegin());
    up(x);
    fa[x]=t=f+(1+black[x])*n;
    Q[t].insert(ma[x]);
}

void cut(int x,int f){
    access(f);
    splay(f);
    splay(x);
    Q[f].erase(Q[f].find(ma[x]));
    fa[x]=0;
    up(f);
}

void link(int x,int f){
    access(f);
    splay(f);
    splay(x);
    fa[x]=f;
    Q[f].insert(ma[x]);
    up(f);
}

int main(){
    while(~scanf("%d",&n)){
        Cls(pre);
        Cls(ch);
        Cls(fa);
        rep(i,n+1) E[i].clear();
        rep(i,n-1){
            int x,y;
            scanf("%d%d",&x,&y);
            E[x].PB(y);
            E[y].PB(x);
        }
        n++;
        rep(i,3*n+1) Q[i].clear();
        rep(i,3*n+1) ma[i]=val[i]=inf,Q[i].insert(inf);
        REP(i,1,n) scanf("%d",black+i);
        REP(i,1,n) scanf("%d",val+i);
        make(1,n);
        int q,k,x;
        scanf("%d",&q);
        rep(i,q){
            scanf("%d%d",&k,&x);
            if (k==0){
                for(x=access(x);lch;x=lch);
                splay(x);
                printf("%d\n",ma[rch]);
            }
            if (k==1){
                cut(x,fat[x]+(1+black[x])*n);
                cut(x+(1+black[x])*n,x);
                black[x]^=1;
                link(x+(1+black[x])*n,x);
                link(x,fat[x]+(1+black[x])*n);
            }
            if (k==2){
                access(x);
                splay(x);
                scanf("%d",val+x);
                up(x);
            }
        }
    }
    return 0;
}
