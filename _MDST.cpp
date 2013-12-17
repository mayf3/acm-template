#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 200 + 1; //number of vertices
const int maxnum = 200000001; //max weight

int map[maxn][maxn], dist[maxn][maxn], list[maxn][maxn];
int Q[maxn], c[maxn], d1[maxn], d2[maxn];
bool used[maxn];
int n, m, ans, x, y, p;

void init() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			map[i][j] = maxnum;
		map[i][i] = 0;
	}
	for (int i = 0; i < m; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		--u; --v;
		if (w > map[u][v]) continue;
		map[u][v] = map[v][u] = w;
	}
}

void APSP() {
	for (int s = 0; s < n; ++s) {
		memset(used, 0, sizeof(used));
		for (int i = 0; i <= n; ++i) dist[s][i] = maxnum, list[s][i] = n;
		dist[s][s] = 0;
		for (int k, u = s, cnt = 0; u < n; u = k, ++cnt) {
			list[s][cnt] = u; used[u] = 1; k = n;
			for (int v = 0; v < n; ++v) {
				if (used[v]) continue;
				dist[s][v] = min(dist[s][v], dist[s][u] + map[u][v]);
				if (dist[s][v] < dist[s][k]) k = v;
			}
		}
	}
}

void work() {
	ans = maxnum;
	for (int u = 0; u < n; ++u)
		if (dist[u][list[u][n - 1]] + dist[u][list[u][n - 2]] < ans) {
			ans = dist[u][list[u][n - 1]] + dist[u][list[u][n - 2]];
			x = y = u;
		}

	for (int u = 0; u < n; ++u)
		for (int v = u + 1; v < n; ++v)
			if (map[u][v] < maxnum) {
				for (int i = 0; i < n; ++i) d1[i] = dist[u][list[u][i]], d2[i] = dist[v][list[u][i]];
				for (int j = n - 1, i = n - 2; i >= 0; --i) {
					if (d1[i] == d1[j]) {
						if (d2[i] > d2[j]) j = i;
						continue;
					}
					if (d2[i] > d2[j]) {
						if (map[u][v] + d1[i] + d2[j] < ans) {
							ans = map[u][v] + d1[i] + d2[j];
							x = u; y = v;
							p = map[u][v] + d2[j] - d1[i];
						}
						j = i;
					}
				}
			}
}

void print() {
	printf("%d\n", ans);
	if (x == y) {
		for (int i = 0; i < n; ++i) c[i] = dist[x][i];
		p = 1;
	}
	else {
		printf("%d %d\n", x + 1, y + 1);
		for (int i = 0; i < n; ++i)
			if (dist[x][i] * 2 + p < (dist[y][i] + map[x][y]) * 2 - p) c[i] = dist[x][i] * 2 + p;
			else c[i] = (dist[y][i] + map[x][y]) * 2 - p;
		p = 2;
	}

	memset(used, 0, sizeof(used));
	int t = 1;
	Q[0] = x; used[x] = 1;
	if (x != y) Q[t++] = y, used[y] = 1;
	for (int h = 0; h < t; ++h) {
		int u = Q[h];
		for (int v = 0; v < n; ++v)
			if (!used[v] && c[v] == c[u] + map[u][v] * p) {
				used[v] = 1;
				if (u < v) printf("%d %d\n", u + 1, v + 1);
				else printf("%d %d\n", v + 1, u + 1);
				Q[t++] = v;
			}
	}
}

int main() {
	init();
	APSP();
	work();
	print();
	return 0;
}
