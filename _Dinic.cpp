const int MAXN = 1000; //number of vertices
const int MAXM = 100000; //number of edges
const int INF = 2000000000; //max capacity

struct Tedge {
	int v, f, c, next;
};

Tedge edge[MAXM];
int first[MAXN], level[MAXN], nedge[MAXN], pedge[MAXN], Q[MAXN];
int N, M, S, T;

void init() {
	memset(first, -1, sizeof(first));
	M = 0;
}

inline void add_edge(int u, int v, int c1, int c2 = 0) {
	edge[M].v = v; edge[M].f = 0; edge[M].c = c1; edge[M].next = first[u]; first[u] = M++;
	edge[M].v = u; edge[M].f = 0; edge[M].c = c2; edge[M].next = first[v]; first[v] = M++;
}

bool newphase() {
	for (int i = 0; i < N; ++i) level[i] = N, nedge[i] = first[i];
	Q[0] = S; level[S] = 0;
	for (int h = 0, t = 1; h < t; ++h) {
		int u = Q[h];
		for (int i = first[u]; i != -1; i = edge[i].next)
			if (edge[i].f < edge[i].c && level[edge[i].v] == N) {
				level[edge[i].v] = level[u] + 1;
				if (edge[i].v == T) return 1;
				Q[t++] = edge[i].v;
			}
	}
	return 0;
}

bool find_path(int u) {
	for (int i = nedge[u]; i != -1; i = edge[i].next)
		if (edge[i].f < edge[i].c && level[edge[i].v] == level[u] + 1)
			if (edge[i].v == T || find_path(edge[i].v)) {
				pedge[edge[i].v] = nedge[u] = i;
				return 1;
			}
	nedge[u] = -1;
	return 0;
}

int Dinic() {
	int ret = 0;
	while (newphase())
		while (find_path(S)) {
			int delta = INF;
			for (int u = T, i = pedge[u]; u != S; u = edge[i ^ 1].v, i = pedge[u])
				delta = min(delta, edge[i].c - edge[i].f);
			for (int u = T, i = pedge[u]; u != S; u = edge[i ^ 1].v, i = pedge[u])
				edge[i].f += delta, edge[i ^ 1].f -= delta;
			ret += delta;
		}
	return ret;
}