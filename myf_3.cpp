//By myf
#pragma comment(linker, "/STACK:16777216")  //C++
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
#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;

using namespace std;

typedef long long LL;
const int MD = 99990001;
const int N = 2*2*100000+10;

struct Mark {
	LL m,a; //x*mul+add
	Mark(LL m, LL a):m(m),a(a){}
	Mark(){m=1,a=0;}
	bool isId() {return m==1&&a==0;}
};

Mark operator*(Mark a, Mark b) {return Mark(a.m*b.m%MD,(a.a*b.m%MD+b.a)%MD;}

struct Node{
	Node *ch[2];
	Mark m;
	LL val;
	int id,min_id;
	int size;
	Node *p;
	Node(){
	    size = 0;
	    min_id=N;
	}
	void sc(Node*c, int d) {
		ch[d] = c;
		c->p = this;
	}
	bool d() {
		return this == p->ch[1];
	}
	void upd() {
	    min_id=min(id,min(ch[0]->min_id,ch[1]->min_id));
		size = 1 + ch[0]->size + ch[1]->size;
	}
	void apply(Mark a) {
		m = m * a;
		val = F(F(val*a.m)+a.a);
	}
	void relax();
} Tnull, *null = &Tnull;

void Node::relax() {
	if (m.isId()) return;
	rep(i,2) if (ch[i] != null) ch[i]->apply(m);
    m = Mark();
}

Node mem[N], *C = mem;

Node*make(int c,int id) {
	C->m = Mark();
	C->val=c;
	C->min_id=C->id=id;
	C->ch[0] = C->ch[1] = null;
	C->p = null;
	C->size=1;
	return C++;
}

void rot(Node*t) {
	Node*p = t->p;
	p->relax();
	t->relax();
	bool d = t->d();
	p->p->sc(t, p->d());
	p->sc(t->ch[!d], d);
	t->sc(p, !d);
	p->upd();
}

void pushTo(Node*t) {
	static Node*stk[N];
	int top = 0;
	while (t != null) {
		stk[top++] = t;
		t = t->p;
	}
	for (int i = top - 1; i >= 0; --i) stk[i]->relax();
}

void splay(Node*u, Node*f = null) {
	pushTo(u);
	while (u->p != f) {
		if (u->p->p == f)
			rot(u);
		else
			u->d() == u->p->d() ? (rot(u->p), rot(u)) : (rot(u), rot(u));
	}
	u->upd();
}

Node *v[N];

vector<PII > G[N];
int n;
int tot;
int edge[N][2];
int cost[N],id[N];
int dfn1[N],dfn2[N];

Node *Pre(Node *v){
    if (v->ch[0]!=null){
        v=v->ch[0];
        while(v->ch[1]!=null) v=v->ch[1];
    }
    else{
        Node *f=v->p;
        while(f!=null && v==f->ch[0]) v=f,f=v->p;
        if (f!=null) v=f;
    }
    return v;
}

Node *Suc(Node *v){
    if (v->ch[1]!=null){
        v=v->ch[1];
        while(v->ch[0]!=null) v=v->ch[0];
    }
    else{
        Node *f=v->p;
        while(f!=null && v==f->ch[1]) v=f,f=v->p;
        if (f!=null) v=f;
    }
    return v;
}

int delEdge(int x,int y) {
    if (dfn1[x]<dfn1[y]) swap(x,y);
    splay(v[dfn1[x]]);
    LL ret=v[dfn1[x]]->val;
    Node *pre=Pre(v[dfn1[x]]),*suc=Suc(v[dfn2[x]]);
    splay(pre),splay(suc,pre);
    pre->relax(),suc->relax();
    Node *tmp=suc->ch[0];
    tmp->p=null,suc->ch[0]=null;
    suc->upd(),pre->upd();
    if (pre->size<tmp->size||pre->size==tmp->size&&pre->min_id<tmp->min_id){
        pre->apply(Mark(ret,0));
        tmp->apply(Mark(1,ret));
    }
    else{
        tmp->apply(Mark(ret,0));
        pre->apply(Mark(1,ret));
    }
    int ans=ret;
	return ans;
}

void Dfs(int x,int f){
    id[tot]=x;
    dfn1[x]=tot++;
    rep(i,G[x].size()){
        int y=G[x][i].X,c=G[x][i].Y;
        if (y==f) continue;
        cost[tot]=c;
        Dfs(y,x);
    }
    id[tot]=x;
    dfn2[x]=tot++;
}

Node* Make(int l,int r){
    int mid=l+(r-l)/2;
    v[mid]=make(cost[mid],id[mid]);
    if (l<mid) v[mid]->sc(Make(l,mid-1),0);
    if (mid<r) v[mid]->sc(Make(mid+1,r),1);
    v[mid]->upd();
    return v[mid];
}

int main() {
	scanf("%d", &n);
	rep(i,n-1) {
		int u, v,c;
		scanf("%d%d%d", &u, &v,&c);
		--u, --v;
		edge[i][0]=u,edge[i][1]=v;
		G[u].PB(MP(v,c));
		G[v].PB(MP(u,c));
	}
	tot=0;
	Dfs(0,-1);
	Make(0,2*n-1);
	rep(i,n-1) {
	    int x;
	    scanf("%d",&x);
	    x--;
	    int a=edge[x][0],b=edge[x][1];
	    printf("%d\n",delEdge(a,b));
	    fflush(stdout);
	}
	return 0;
}
