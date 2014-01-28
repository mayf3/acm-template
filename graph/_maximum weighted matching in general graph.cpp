//notice that the N may be changed by the algorithm
//adjust the MAXTRY to be more accurate
const int MAXN = 500 + 10; //number of vertices
const int MAXTRY = 10;

int graph[MAXN][MAXN];
int M[MAXN]; //the match vertex of each vertex
int dist[MAXN], idx[MAXN], prev[MAXN];
bool visited[MAXN];
int N;

int SPFA(int u) {
	visited[u] = 1;
	for (int i = 0; i < N; ++i) {
		int v = idx[i];
		if (dist[u] + graph[v][M[v]] - graph[u][v] < dist[M[v]]) {
			if (visited[v]) continue;
			dist[M[v]] = dist[u] + graph[v][M[v]] - graph[u][v];
			prev[M[v]] = u;
			if (visited[M[v]]) return M[v];
			int res = SPFA(M[v]);
			if (res != -1) return res;
		}
	}
	visited[u] = 0;
	return -1;
}

int Negcir() {
	for (int i = 0; i < N; ++i) {
		memset(visited, 0, sizeof(visited));
		memset(dist, 0, sizeof(dist));
		int ret = SPFA(idx[i]);
		if (ret != -1) return ret;
	}
	return -1;
}

int Augment(int x) {
	if(M[M[x]] != x) return 0;
	int m = M[x], ret = graph[prev[x]][m] - graph[m][x];
	M[m] = prev[x];
	ret += Augment(prev[x]);
	M[prev[x]] = m;
	return ret;
}

int WeightedMatch() {
	int ret = 0;
	if (N & 1) {
		for (int i = 0; i < N; ++i) graph[i][N] = graph[N][i] = 0;
		++N;
	}
	memset(visited, 0, sizeof(visited));
	for (int i = 0; i < N; ++i {
		M[i] = i ^ 1;
		if (i & 1) ret += graph[i][i ^ 1];
		idx[i] = i;
	}
	while (1) {
		int flag = MAXTRY;
		while (flag--) {
			random_shuffle(idx, idx + N);
			int res = Negcir();
			if (res != -1) {
				ret += Augment(res);
				break;
			}
		}
		if (flag < 0) break;
	}
	return ret;
}
