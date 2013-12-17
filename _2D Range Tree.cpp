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

const int N=10000+10,M=N*2;

struct node {
	int item, cnt, l, r;
	int *elm, *lnkl, *lnkr;

	void init() {
	    delete [] elm;
	    delete [] lnkl;
	    delete [] lnkr;
	    cnt=0,l=r=-1;
    };
};

struct point {
	T x, y;
};

point a[N];
node tree[M];
int key[N];
int n, m, cur, root;

bool cmp(point a, point b) {
    return a.x<b.x||a.x==b.x&&a.y<b.y;
}

void build(int l, int r, int &k) {
	if (k == -1) tree[k = m++].init();
	int mid = (l + r) >> 1;
	tree[k].item = key[mid];
	if (l == r - 1) {
		int next = cur;
		while (next < n && a[next].x == key[mid]) ++next;
		tree[k].elm = new int[next - cur]; tree[k].lnkl = tree[k].lnkr = NULL;
		REP(i,cur,next) tree[k].elm[tree[k].cnt++] = a[i].y;
		cur = next;
		return;
	}

	int lc, rc;
	build(l, mid, tree[k].l); build(mid, r, tree[k].r);
	lc = tree[k].l; rc = tree[k].r;

	tree[k].cnt = tree[lc].cnt + tree[rc].cnt;
	tree[k].elm = new int[tree[k].cnt]; tree[k].lnkl = new int[tree[k].cnt + 1]; tree[k].lnkr = new int[tree[k].cnt + 1];
	for (int i = 0, j = 0, p = 0; p < tree[k].cnt; ++p) {
		tree[k].lnkl[p] = i; tree[k].lnkr[p] = j;
		if (j == tree[rc].cnt || i < tree[lc].cnt && tree[lc].elm[i] < tree[rc].elm[j]) tree[k].elm[p] = tree[lc].elm[i++];
		else tree[k].elm[p] = tree[rc].elm[j++];
	}
	tree[k].lnkl[tree[k].cnt] = tree[lc].cnt; tree[k].lnkr[tree[k].cnt] = tree[rc].cnt;
}

int find(int x1, int x2, int y1, int y2, int k) {
	while (tree[k].l != -1 && (x2 < tree[k].item || x1 >= tree[k].item))
		if (x2 < tree[k].item) k = tree[k].l;
		else k = tree[k].r;

	int left = lower_bound(tree[k].elm, tree[k].elm + tree[k].cnt, y1) - tree[k].elm;
	int right = upper_bound(tree[k].elm, tree[k].elm + tree[k].cnt, y2) - tree[k].elm;
	if (right - left < 2) return 0;
	if (tree[k].l == -1)
		if (x1 <= tree[k].item && tree[k].item <= x2) return right - left;
		else return 0;

	int ret = 0, v = tree[k].l, l = tree[k].lnkl[left], r = tree[k].lnkl[right];
	while (tree[v].l != -1)
		if (x1 < tree[v].item) {
			ret += tree[v].lnkr[r] - tree[v].lnkr[l];
			if (ret > 1) return ret;
			l = tree[v].lnkl[l]; r = tree[v].lnkl[r]; v = tree[v].l;
		}
		else l = tree[v].lnkr[l], r = tree[v].lnkr[r], v = tree[v].r;
	if (x1 <= tree[v].item) ret += r - l;

	v = tree[k].r; l = tree[k].lnkr[left]; r = tree[k].lnkr[right];
	while (tree[v].l != -1)
		if (x2 >= tree[v].item) {
			ret += tree[v].lnkl[r] - tree[v].lnkl[l];
			if (ret > 1) return ret;
			l = tree[v].lnkr[l]; r = tree[v].lnkr[r]; v = tree[v].r;
		}
		else l = tree[v].lnkl[l], r = tree[v].lnkl[r], v = tree[v].l;
	if (tree[v].item <= x2) ret += r - l;

	return ret;
}

void init() {
    rep(i,n) key[i] = a[i].x;
	sort(a, a + n, cmp);
	sort(key, key + n);
	int l = unique(key, key + n) - key;

	root = 0;
	m = 1; tree[0].init(); cur = 0;
	build(0, l, root);
}

int main(){
    cin>>n;
    rep(i,n) scanf("%d%d",&a[i].x,&a[i].y);
    init();
    return 0;
}
