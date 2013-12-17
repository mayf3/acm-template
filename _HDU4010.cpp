/*
	1. Given two integer x, y, you should make a new edge between these two node x and y. So after this operation, two trees will be connected to a new one. 
	2. Given two integer x, y, you should find the tree in the tree set who contain node x, and you should make the node x be the root of this tree, and then you should cut the edge between node y and its parent. So after this operation, a tree will be separate into two parts. 
	3. Given three integer w, x, y, for the x, y and all nodes between the path from x to y, you should increase their weight by w. 
	4. Given two integer x, y, you should check the node weights on the path between x and y, and you should output the maximum weight on it. 
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 300000 + 10;

struct Node {
	int p, d, r, w, maxw, delta;
	int ch[2];
};

Node node[MAXN];

#define ROOT(x) (!node[x].p || node[node[x].p].ch[0] != x && node[node[x].p].ch[1] != x)

inline void merge(int x) {
	node[x].maxw = node[x].w;
	node[x].maxw = max(node[x].maxw, max(node[node[x].ch[0]].maxw, node[node[x].ch[1]].maxw));
}

inline void setChild(int x, int d, int y) {
	node[x].ch[d] = y;
	node[y].p = x;
	node[y].d = d;
}

inline void update(int x, int d) {
	if (!x) return;
	node[x].w += d;
	node[x].maxw += d;
	node[x].delta += d;
}

inline void unmark(int x) {
	if (node[x].r) {
		swap(node[x].ch[0], node[x].ch[1]);
		int l = node[x].ch[0], r = node[x].ch[1];
		if (l) node[l].r ^= 1, node[l].d = 0;
		if (r) node[r].r ^= 1, node[r].d = 1;
		node[x].r = 0;
	}
	if (node[x].delta) {
		update(node[x].ch[0], node[x].delta);
		update(node[x].ch[1], node[x].delta);
		node[x].delta = 0;
	}
}

inline void zag(int x) {
	int y = node[x].p;
	unmark(y); unmark(x);
	int d = node[x].d;
	setChild(y, d, node[x].ch[!d]);
	if (y == node[node[y].p].ch[0]) setChild(node[y].p, 0, x);
	else if (y == node[node[y].p].ch[1]) setChild(node[y].p, 1, x);
	else node[x].p = node[y].p;
	setChild(x, !d, y);
	merge(y);
}

void splay(int x) {
	unmark(x);
	while (!ROOT(x)) {
		if (!ROOT(node[x].p)) zag(node[x].d == node[node[x].p].d ? node[x].p : x);
		zag(x);
	}
	merge(x);
}

void expose(int x) {
	for (int y = x, z = 0; y; y = node[y].p) splay(y), setChild(y, 1, z), merge(z = y);
	splay(x);
}

void evert(int x) {
	expose(x);
	node[x].r ^= 1;
}

void link(int x, int y) {
	expose(x); expose(y);
	setChild(y, 0, x);
	merge(y);
}

void cut(int x) {
	expose(x);
	int y = node[x].ch[0];
	if (y) {
		node[y].p = 0;
		node[x].ch[0] = 0;
		merge(x);
	}
}

int root(int x) {
	expose(x);
	while (node[x].ch[0]) x = node[x].ch[0], unmark(x);
	splay(x);
	return x;
}

int parent(int x) {
	expose(x);
	x = node[x].ch[0];
	if (!x) return 0;
	while (node[x].ch[1]) x = node[x].ch[1], unmark(x);
	return x;
}

void add(int x, int y, int d) {
	evert(x);
	expose(y);
	update(y, d);
}

int query(int x, int y) {
	evert(x);
	expose(y);
	return node[y].maxw;
}

int n, q;
int edge[MAXN][2];

int main() {
	while (scanf("%d", &n) != EOF) {
		memset(node, 0, sizeof(node[0]) * (n + 1));
		for (int i = 0; i < n - 1; ++i) scanf("%d%d", &edge[i][0], &edge[i][1]);

		for (int i = 1; i <= n; ++i) {
			int w;
			scanf("%d", &w);
			node[i].w = node[i].maxw = w;
		}

		for (int i = 0; i < n - 1; ++i) {
			int x = edge[i][0], y = edge[i][1];
			evert(y);
			link(x, y);
		}

		for (scanf("%d", &q); q > 0; --q) {
			int op;
			scanf("%d", &op);

			if (op == 1) {
				int x, y;
				scanf("%d%d", &x, &y);
				if (root(x) == root(y)) puts("-1");
				else {
					evert(y);
					link(x, y);
				}
			}
			else if (op == 2) {
				int x, y;
				scanf("%d%d", &x, &y);
				if (x == y || root(x) != root(y)) puts("-1");
				else {
					evert(x);
					cut(y);
				}
			}
			else if (op == 3) {
				int x, y, w;
				scanf("%d%d%d", &w, &x, &y);
				if (root(x) != root(y)) puts("-1");
				else add(x, y, w);
			}
			else if (op == 4) {
				int x, y;
				scanf("%d%d", &x, &y);
				if (root(x) != root(y)) puts("-1");
				else printf("%d\n", query(x, y));
			}
		}

		printf("\n");
	}

	return 0;
}
