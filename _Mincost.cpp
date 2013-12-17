const int MAXN = 1000; //number of vertices
const int MAXM = 10000; //number of edges
const int MAXQ = 10000; //length of queue
const int INF = 2000000000; //max capacity

struct Tedge {
	int v, f, c, w, next;
};

Tedge edge[MAXM];
int first[MAXN], best[MAXN], path[MAXN];
int Q[MAXQ];
bool inq[MAXN];
int N, M, S, T;

void init() {
	memset(first, -1, sizeof(first));
	M = 0;
}

inline void add_edge(int u, int v, int c, int w) {
	edge[M].v = v; edge[M].f = 0; edge[M].c = c; edge[M].w = w; edge[M].next = first[u]; first[u] = M++;
	edge[M].v = u; edge[M].f = 0; edge[M].c = 0; edge[M].w = -w; edge[M].next = first[v]; first[v] = M++;
}

bool find_path() {
	for (int i = 0; i < N; ++i) best[i] = INF, inq[i] = 0;
	best[S] = 0; inq[S] = 1; Q[0] = S;
	for (int h = 0, t = 1; h != t; h = (h + 1) % MAXQ) {
		int u = Q[h];
		for (int i = first[u]; i != -1; i = edge[i].next) {
			int v = edge[i].v;
			if (edge[i].f < edge[i].c && best[u] + edge[i].w < best[v]) {
				best[v] = best[u] + edge[i].w; path[v] = i;
				if (!inq[v]) {
					inq[v] = 1;
					Q[t] = v; t = (t + 1) % MAXQ;
				}
			}
		}
		inq[u] = 0;
	}
	return best[T] < INF;
}

int Mincost() {
	int ret = 0;
	while (find_path()) {
		int delta = INF;
		for (int u = T, i = path[u]; u != S; u = edge[i ^ 1].v, i = path[u])
			delta = min(delta, edge[i].c - edge[i].f);
		for (int u = T, i = path[u]; u != S; u = edge[i ^ 1].v, i = path[u])
			edge[i].f += delta, edge[i ^ 1].f -= delta;
		ret += delta * best[T];
	}
	return ret;
}
