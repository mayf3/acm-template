/*
	You are given a tree with N nodes.
	The tree’s nodes are numbered 1 through N and its edges are numbered 1 through N - 1.
	Each edge is associated with a weight. Then you are to execute a series of instructions on the tree.
	The instructions can be one of the following forms:
		CHANGE i v Change the weight of the ith edge to v 
		NEGATE a b Negate the weight of every edge on the path from a to b 
		QUERY a b Find the maximum weight of edges on the path from a to b 
*/
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 10010;
const int INF = 2000000000;

struct Node {
	int child[2]; 
	int parent, typ, value, maxvalue, minvalue;
	bool neg;
}; 

struct Tedge {
	int v, w, next;
};

Tedge edge[MAXN * 2];
Node node[MAXN];
int first[MAXN], bottom[MAXN];
bool used[MAXN];
int N;

inline int cal_max(int x) {
	if (!x) return -INF;
	if (node[x].neg) return -node[x].minvalue;
	else return node[x].maxvalue;
}

inline int cal_min(int x) {
	if (!x) return INF;
	if (node[x].neg) return -node[x].maxvalue;
	else return node[x].minvalue;
}

inline void update(int x) {
	if (node[x].neg) {
		node[x].value = -node[x].value;
		swap(node[x].maxvalue, node[x].minvalue);
		node[x].maxvalue = -node[x].maxvalue; node[x].minvalue = -node[x].minvalue;
		node[node[x].child[0]].neg = !node[node[x].child[0]].neg;
		node[node[x].child[1]].neg = !node[node[x].child[1]].neg;
		node[x].neg = 0;
	}
	if (x > 1) node[x].maxvalue = node[x].value;
	else node[x].maxvalue = -INF;
	node[x].maxvalue = max(node[x].maxvalue, cal_max(node[x].child[0]));
	node[x].maxvalue = max(node[x].maxvalue, cal_max(node[x].child[1]));
	if (x > 1) node[x].minvalue = node[x].value;
	else node[x].minvalue = INF;
	node[x].minvalue = min(node[x].minvalue, cal_min(node[x].child[0]));
	node[x].minvalue = min(node[x].minvalue, cal_min(node[x].child[1]));
}

void cal(int x) {
	if (node[x].parent == 0) return;
	cal(node[x].parent);
	update(x);
}

void rotate(int x, int a) {
	int y = node[x].parent, b = node[y].typ;
 
	node[x].parent = node[y].parent; node[x].typ = b;

	if (b != 2) node[node[y].parent].child[b] = x;
	
	b = 1 - a;
	node[node[x].child[b]].parent = y; node[node[x].child[b]].typ = a;
	node[y].child[a] = node[x].child[b]; node[y].parent = x; node[y].typ = b;
	node[x].child[b] = y;

	update(y);
} 

void splay(int x) {
	cal(x);
	while (1) {
		int a = node[x].typ;
		if (a == 2) break;

		int y = node[x].parent, b = node[y].typ;
		if (a == b) rotate(y, a);
		else rotate(x, a);

		if (b == 2) break;
		rotate(x, b);
	}
	update(x);
} 

void expose(int v) {
	int u = v, w = 0;
	while (u) {
		splay(u);
		node[node[u].child[0]].typ = 2;
		node[u].child[0] = w;
		node[w].typ = 0;
		update(u);
		w = u; u = node[u].parent;
	}
	splay(v);
} 

inline void link(int v, int w) {
	expose(v);
	expose(w);
	node[v].parent = w;
}

void DFS(int u) {
	used[u] = 1;
	for (int i = first[u]; i != -1; i = edge[i].next) {
		int v = edge[i].v, w = edge[i].w;
		if (!used[v]) {
			node[v].value = node[v].maxvalue = node[v].minvalue = w;
			bottom[i >> 1] = v;
			link(v, u);
			DFS(v);
		}
	}
}

void readTrees() {
	memset(first, -1, sizeof(first));
	scanf("%d", &N);
	for (int i = 0; i < N - 1; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		edge[i * 2].v = v; edge[i * 2].w = w; edge[i * 2].next = first[u]; first[u] = i * 2;
		edge[i * 2 + 1].v = u; edge[i * 2 + 1].w = w; edge[i * 2 + 1].next = first[v]; first[v] = i * 2 + 1;
	}

	for (int i = 0; i <= N; ++i) {
		node[i].child[0] = node[i].child[1] = 0; node[i].typ = 2;
		node[i].parent = 0; node[i].neg = 0;
	}
	node[0].maxvalue = -INF; node[0].minvalue = INF;
	node[1].maxvalue = -INF; node[1].minvalue = INF;

	memset(used, 0, sizeof(used));
	DFS(1);
}

int query(int u, int v) {
	if (u == v) return 0;

	expose(u);

	int x = v, w = 0, ret = -INF;
	while (v) {
		splay(v);
		if (node[v].parent == 0) {
			ret = max(ret, cal_max(node[v].child[0]));
			ret = max(ret, cal_max(w));
		}
		node[node[v].child[0]].typ = 2;
		node[v].child[0] = w;
		node[w].typ = 0;
		update(v);
		w = v; v = node[v].parent;
	}
	splay(x);
	return ret;
}

void negate(int u, int v) {
	expose(u);

	int x = v, w = 0;
	while (v) {
		splay(v);
		if (node[v].parent == 0) {
			node[node[v].child[0]].neg = !node[node[v].child[0]].neg;
			node[w].neg = !node[w].neg;
		}
		node[node[v].child[0]].typ = 2;
		node[v].child[0] = w;
		node[w].typ = 0;
		update(v);
		w = v; v = node[v].parent;
	}
	splay(x);
}

void solve() {
	char cmd[10];
	while (scanf("%s", cmd), cmd[0] != 'D') {
		int x, y;
		scanf("%d%d", &x, &y);
		
		if (cmd[0] == 'Q') printf("%d\n", query(x, y));
		else if (cmd[0] == 'C') {
			x = bottom[x - 1];
			splay(x);
			node[x].value = y; node[x].neg = 0;
			update(x);
		}
		else negate(x, y);
	}
}

int main() {
	int t;
	for (scanf("%d", &t); t > 0; --t) {
		readTrees();
		solve();
	}

	return 0;
}
