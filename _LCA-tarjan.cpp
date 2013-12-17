const int MAXN = 1000; //number of vertices
const int MAXM = 10000; //number of edges
const int MAXQ = 1000; //number of queries

struct Tedge {
	int v, next;
};

Tedge edge[MAXM], query[MAXQ];
int first[MAXN], head[MAXN], root[MAXN], anc[MAXN];
bool used[MAXN];
int N, M, Q;

inline void init() {
	memset(first, -1, sizeof(first));
	memset(head, -1, sizeof(head));
	memset(root, -1, sizeof(root));
	memset(used, 0, sizeof(used));
	M = Q = 0;
}

inline void add_edge(int u, int v) {
	edge[M].v = v; edge[M].next = first[u]; first[u] = M++;
}

inline void add_query(int u, int v) {
	query[Q].v = v; query[Q].next = head[u]; head[u] = Q++;
}

int find(int x) {
	if (root[x] == x) return x;
	else return root[x] = find(root[x]);
}

void LCA(int u) {
	root[u] = anc[u] = u;
	for (int i = first[u]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if (root[v] == -1) {
			LCA(v);
			root[v] = u;
		}
	}

	used[u] = 1;
	for (int i = head[u]; i != -1; i = query[i].next) {
		int v = query[i].v;
		if (used[v]) printf("LCA(%d , %d) = %d\n", u, v, anc[find(v)]);
	}
}
