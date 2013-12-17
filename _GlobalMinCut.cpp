const int MAXN = 50 + 10; //number of vertices
const int MAXM = 500; //number of MinCut edges
const int INF = 1000000000; //max capacity

int map[MAXN][MAXN], a[MAXN][MAXN], idx[MAXN][MAXN]; //map, tmp map, idx of edge
int root[MAXN], q[MAXN], w[MAXN], pre[MAXN];
int list[MAXM]; //MinCut Edges
bool used[MAXN];
int N, M;

int mincut(int n) {
	memset(used, 0, sizeof(used));
	memset(w, 0, sizeof(w));
	int last, cnt = 0;
	for (int k, i = 0; i != n; i = k) {
		last = i; used[i] = 1; k = n;
		for (int j = 0; j < n; ++j) {
			if (used[j]) continue;
			w[j] += a[q[i]][q[j]]; pre[j] = i;
			if (w[j] > w[k]) k = j;
		}
	}
	return last;
}

int find(int x) {
	if (root[x] == x) return x;
	else return root[x] = find(root[x]);
}

int stoer_wagner() {
	memcpy(a, map, sizeof(map));
	for (int i = 0; i < N; ++i) q[i] = root[i] = i;

	int ret = INF;
	for (int i = 0; i < N - 1; ++i) {
		int t = mincut(N - i);
		ret = min(ret, w[t]);
		int s = pre[t];
		for (int j = 0; j < N - i; ++j)
			if (j != s && j != t) a[q[t]][q[j]] = (a[q[j]][q[t]] += a[q[j]][q[s]]);
		root[find(q[s])] = find(q[t]); q[s] = q[N - i - 1]; 
	}

	return ret;
}

void cal(int ans) {
	memcpy(a, map, sizeof(map));
	for (int i = 0; i < N; ++i) q[i] = root[i] = i;

	int t;
	for (int i = 0; i < N - 2; ++i) {
		t = mincut(N - i - 1);

		if (w[t] == ans) break;

		int s = pre[t];
		for (int j = 0; j < N - i - 1; ++j)
			if (j != s && j != t) a[q[t]][q[j]] = (a[q[j]][q[t]] += a[q[j]][q[s]]);
		root[find(q[s])] = find(q[t]); q[s] = q[N - i - 2]; 
	}

	t = find(q[t]);

	M = 0; //number of MinCut edges
	for (int i = 0; i < N; ++i)
		if (find(root[i]) == t)
			for (int j = 0; j < N; ++j)
				if (find(root[j]) != t && idx[i][j]) list[M++] = idx[i][j];
}