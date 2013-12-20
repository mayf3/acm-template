#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 10000 + 10;
const int maxl = 15; //2 ^ maxl >= maxn
const int INF = 2000000000;

struct Tedge {
	int v, next;
};

struct node {
	int Max, Min, mid, left, right;
	bool opp;
};

Tedge edge[maxn * 2];
int first[maxn], depth[maxn], dist[maxn], w[maxn], path[maxn], father[maxn],
	chiedge[maxn], paredge[maxn], pos[maxn], high[maxn], order[maxn];
int tour[maxn * 2], lg[maxn * 2];
int st[maxl][maxn * 2];
node segtree[maxn * 4];
int n, npath, m, cnt, tot;

void DFS(int u) {
	tour[m++] = u;
	for (int i = first[u]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if (v != father[u]) {
			paredge[v] = i >> 1; father[v] = u; depth[v] = depth[u] + 1;
			DFS(v);
			tour[m++] = u;
			if (dist[v] > dist[u]) dist[u] = dist[v], chiedge[u] = i;
		}
	}
	++dist[u];
}

inline int cal_max(int k) {
	if (k == -1) return -INF;
	if (segtree[k].opp) return -segtree[k].Min;
	else return segtree[k].Max;
}

inline int cal_min(int k) {
	if (k == -1) return INF;
	if (segtree[k].opp) return -segtree[k].Max;
	else return segtree[k].Min;
}

void build(int l, int r, int k) {
	segtree[k].opp = 0;
	if (l == r) {
		segtree[k].Max = segtree[k].Min = w[order[l]];
		segtree[k].left = segtree[k].right = -1;
		return;
	}

	segtree[k].mid = (l + r) >> 1;
	segtree[k].left = tot++; segtree[k].right = tot++;
	build(l, segtree[k].mid, segtree[k].left); build(segtree[k].mid + 1, r, segtree[k].right);

	segtree[k].Max = max(cal_max(segtree[k].left), cal_max(segtree[k].right));
	segtree[k].Min = min(cal_min(segtree[k].left), cal_min(segtree[k].right));
}

void decom() {
	memset(depth, 0, sizeof(depth));
	memset(dist, 0, sizeof(dist));
	memset(chiedge, -1, sizeof(chiedge));
	memset(paredge, -1, sizeof(paredge));
	memset(father, -1, sizeof(father));
	m = 0;

	DFS(0);

	cnt = npath = 0;
	memset(pos, -1, sizeof(pos));
	for (int i = 0; i < n; ++i)
		if (chiedge[i] == -1) {
			int p = i;
			while (p) {
				int j = paredge[p], q = father[p];
				if ((chiedge[q] >> 1) != j) break;
				path[p] = npath; order[cnt] = j; pos[p] = cnt++; p = q;
			}
			path[p] = npath++; high[i] = p;
			p = i;
			while (1) {
				high[p] = high[i];
				if (p == high[i]) break;
				p = father[p];
			}
		}

	tot = 1;
	build(0, cnt - 1, 0);
}

void RMQ() {
	for (int i = 0; i < m; ++i) first[tour[i]] = i, st[0][i] = tour[i];

	for (int i = 1, k = 2; k <= m; ++i, k <<= 1)
		for (int j = 0; j <= m - k; ++j)
			if (depth[st[i - 1][j]] < depth[st[i - 1][j + (k >> 1)]]) st[i][j] = st[i - 1][j];
			else st[i][j] = st[i - 1][j + (k >> 1)];
}

inline int get_LCA(int i, int j) {
	i = first[i]; j = first[j];
	if (i > j) swap(i, j);
	int k = lg[j - i + 1];
	if (depth[st[k][i]] < depth[st[k][j - (1 << k) + 1]]) return st[k][i];
	else return st[k][j - (1 << k) + 1];
}

inline void update(int k) {
	segtree[k].opp = 0;
	swap(segtree[k].Max, segtree[k].Min);
	segtree[k].Max = -segtree[k].Max; segtree[k].Min = -segtree[k].Min;
	if (segtree[k].left != -1) segtree[segtree[k].left].opp = !segtree[segtree[k].left].opp;
	if (segtree[k].right != -1) segtree[segtree[k].right].opp = !segtree[segtree[k].right].opp;
}

void modify(int p, int l, int r, int k) {
	if (l == r) {
		segtree[k].Max = segtree[k].Min = w[order[l]]; segtree[k].opp = 0;
		return;
	}

	if (segtree[k].opp) update(k);

	if (p <= segtree[k].mid) modify(p, l, segtree[k].mid, segtree[k].left);
	else modify(p, segtree[k].mid + 1, r, segtree[k].right);

	segtree[k].Max = max(cal_max(segtree[k].left), cal_max(segtree[k].right));
	segtree[k].Min = min(cal_min(segtree[k].left), cal_min(segtree[k].right));
}

void turnopp(int x, int y, int l, int r, int k) {
	if (x > r || y < l) return;
	if (x <= l && r <= y) {
		segtree[k].opp = !segtree[k].opp;
		return;
	}

	if (segtree[k].opp) update(k);

	turnopp(x, y, l, segtree[k].mid, segtree[k].left); turnopp(x, y, segtree[k].mid + 1, r, segtree[k].right);

	segtree[k].Max = max(cal_max(segtree[k].left), cal_max(segtree[k].right));
	segtree[k].Min = min(cal_min(segtree[k].left), cal_min(segtree[k].right));
}

void negate(int l, int r) {
	while (l != r) {
		if (pos[l] > -1) {
			int j;
			if (path[l] == path[r]) j = r;
			else j = high[l];
			j = edge[chiedge[j]].v;
			turnopp(pos[l], pos[j], 0, cnt - 1, 0);
			l = j;
		}
		else w[paredge[l]] = -w[paredge[l]];
		l = father[l];
	}
}

int getmax(int x, int y, int l, int r, int k) {
	if (x > r || y < l) return -INF;
	if (segtree[k].opp) update(k);

	if (x <= l && r <= y) return segtree[k].Max;

	return max(getmax(x, y, l, segtree[k].mid, segtree[k].left), getmax(x, y, segtree[k].mid + 1, r, segtree[k].right));
}

int query(int l, int r) {
	int ret = -INF;
	while (l != r) {
		if (pos[l] > -1) {
			int j;
			if (path[l] == path[r]) j = r;
			else j = high[l];
			j = edge[chiedge[j]].v;
			ret = max(ret, getmax(pos[l], pos[j], 0, cnt - 1, 0));
			l = j;
		}
		else ret = max(ret, w[paredge[l]]);
		l = father[l];
	}
	return ret;
}

int main() {
	lg[0] = 0;
	for (int j, i = 1; i < maxn * 2; lg[i++] = j - 1)
		for (j = 0; (1 << j) <= i; ++j);
	
	int T;
	for (scanf("%d", &T); T > 0; --T) {
		memset(first, -1, sizeof(first));

		scanf("%d", &n);
		for (int i = 0; i < n - 1; ++i) {
			int u, v;
			scanf("%d%d%d", &u, &v, &w[i]);
			--u; --v;
			edge[i * 2].v = v; edge[i * 2].next = first[u]; first[u] = i * 2;
			edge[i * 2 + 1].v = u; edge[i * 2 + 1].next = first[v]; first[v] = i * 2 + 1;
		}

		decom();
		RMQ();

		char cmd[10];
		while (scanf("%s", cmd), cmd[0] != 'D') {
			int a, b;
			scanf("%d%d", &a, &b);

			if (cmd[0] == 'C') {
				w[--a] = b;
				int u = edge[a << 1].v, v = edge[(a << 1) + 1].v;
				if (depth[u] < depth[v]) u = v;
				if (pos[u] > -1) modify(pos[u], 0, cnt - 1, 0);
			}
			else if (cmd[0] == 'N') {
				int LCA = get_LCA(--a, --b);
				negate(a, LCA); negate(b, LCA);
			}
			else {
				if ((--a) == (--b)) printf("0\n");
				else {
					int LCA = get_LCA(a, b);
					printf("%d\n", max(query(a, LCA), query(b, LCA)));
				}
			}
		}
	}

	return 0;
}
