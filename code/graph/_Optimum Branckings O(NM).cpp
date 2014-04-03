const int MAXN = 1000 + 10; //number of vertices
const int MAXM = 1001000 + 10; //number of edges
const int INF = 1000000000;

struct Tedge {
	int u, v, w;
};

Tedge edge[MAXM];
int idx[MAXN], pre[MAXN], vis[MAXN], in[MAXN];
int N, M;

inline void add_edge(int u, int v, int w) {
	edge[M].u = u; edge[M].v = v; edge[M++].w = w;
}

int DMST(int root) {
	int ret = 0;
	while (1) {
		for (int i = 0; i < N; ++i) {
			in[i] = INF;
			pre[i] = -1;
		}
		for (int i = 0; i < M; ++i) {
			int u = edge[i].u, v = edge[i].v, w = edge[i].w;
			if (u != v && w < in[v]) {
				pre[v] = u;
				in[v] = w;
			}
		}
		for (int i = 0; i < N; ++i) {
			if(i == root) continue;
			if (pre[i] == -1) return -1; //No solution
		}
		in[root] = 0;
		int cnt = 0;
		memset(vis, -1, sizeof(vis));
		memset(idx, -1,sizeof(idx));
		for (int i = 0; i < N; ++i) {
			int v = i;
			ret += in[v];
			while (vis[v] != i && idx[v] == -1 && v != root) {
				vis[v] = i; v = pre[v];
			}
			if (v != root && idx[v] == -1) {
				int u = pre[v];
				while (u != v) {
					idx[u] = cnt;
					u = pre[u];
				}
				idx[v] = cnt++;
			}
		}
		if (cnt == 0) return ret;
		for (int i = 0; i < N; ++i)
			if (idx[i] == -1) idx[i] = cnt++;
		for (int i = 0; i < M; ++i) {
			int v = edge[i].v;
			edge[i].u = idx[edge[i].u]; edge[i].v = idx[edge[i].v];
			if (edge[i].u != edge[i].v) edge[i].w -= in[v];
		}
		N = cnt; root = idx[root];
	}
	return ret;
}
