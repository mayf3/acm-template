const int MAXN = 1000; //number of vertices
const int MAXE = 10000; //number of edges

struct Tedge {
	int v, next;
};

Tedge edge[MAXE];
int first[MAXN], px[MAXN], py[MAXN], dx[MAXN], dy[MAXN], q[MAXN];
bool used[MAXN];
int N, E, len;

void init() {
	memset(first, -1, sizeof(first));
	E = 0;
}

inline void add_edge(int u, int v) {
	edge[E].v = v; edge[E].next = first[u]; first[u] = E++;
}

bool search(int u) {
	if (dx[u] > len) return 0;
	used[u] = 1;
	for (int i = first[u]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if ((py[v] == -1 || !used[py[v]]) && dx[u] + 1 == dy[v]) {
			int tx = px[u], ty = py[v];
			px[u] = v; py[v] = u;
			if (ty == -1 || search(ty)) return 1;
			px[u] = tx; py[v] = ty;
		}
	}
	return 0;
}

void hopcroft() {
	memset(px, -1, sizeof(px));
	memset(py, -1, sizeof(py));

	while (1) {
		memset(dx, 0, sizeof(dx));
		memset(dy, 0, sizeof(dy));
		int t = len = 0;
		for (int i = 0; i < N; ++i)
			if (px[i] == -1) q[t++] = i, dx[i] = 1;
		for (int h = 0; h < t; ++h) {
			int u = q[h];
			for (int i = first[u]; i != -1; i = edge[i].next) {
				int v = edge[i].v;
				if (!dy[v]) {
					dy[v] = dx[u] + 1;
					if (py[v] != -1) q[t++] = py[v], dx[py[v]] = dy[v] + 1;
					else len = max(len, dy[v]);
				}
			}
		}
		if (!len) break;
		memset(used, 0, sizeof(used));
		for (int i = 0; i < N; ++i)
			if (px[i] == -1) search(i);
	}
}
