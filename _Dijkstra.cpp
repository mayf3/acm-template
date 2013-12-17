const int MAXN = 100000 + 1; //number of vertices + 1
const int MAXM = 1000000; //number of edges
const int INF = 2000000000; //max dist

struct Tedge {
	int v, w, next;
};

Tedge edge[MAXM];
int first[MAXN], dist[MAXN], heap[MAXN], pos[MAXN];
int N, M, cnt;

inline void init() {
	memset(first, -1, sizeof(first));
	M = 0;
}

inline void add_edge(int u, int v, int w) {
	edge[M].v = v; edge[M].w = w; edge[M].next = first[u]; first[u] = M++;
}

inline void moveup(int i) {
	int key = heap[i];
	while (i > 1 && dist[heap[i >> 1]] > dist[key]) {
		heap[i] = heap[i >> 1]; pos[heap[i]] = i; i >>= 1;
	}
	heap[i] = key; pos[key] = i;
}

inline void movedown(int i) {
	int key = heap[i];
	while ((i << 1) <= cnt) {
		int j = i << 1;
		if (j < cnt && dist[heap[j + 1]] < dist[heap[j]]) ++j;
		if (dist[key] <= dist[heap[j]]) break;
		heap[i] = heap[j]; pos[heap[i]] = i; i = j;
	}
	heap[i] = key; pos[key] = i;
}

void Dijkstra(int S) {
	for (int i = 0; i < N; ++i) pos[i] = -1, dist[i] = INF;
	cnt = 1; heap[1] = S; dist[S] = 0;
	while (cnt) {
		int u = heap[1];
		heap[1] = heap[cnt--];
		movedown(1);
		for (int i = first[u]; i != -1; i = edge[i].next) {
			int v = edge[i].v, w = edge[i].w;
			if (dist[u] + w < dist[v]) {
				dist[v] = dist[u] + w;
				if (pos[v] == -1) pos[v] = ++cnt, heap[cnt] = v;
				moveup(pos[v]);
			}
		}
	}
}
