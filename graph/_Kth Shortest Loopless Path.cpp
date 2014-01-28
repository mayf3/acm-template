#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>

using namespace std;

const int MAXN = 50 + 10; //number of vertices
const int MAXK = 200 + 10;
const int INF = 1000000000; //max dist

struct Tpath {
	int cnt, len, pos;
	int v[MAXN];
};

Tpath path[MAXK];
int g[MAXN][MAXN];
int len[MAXK], pos[MAXK], ans[MAXK];
bool used[MAXN];
int dist[MAXN], prev[MAXN], List[MAXN];
int N, M, K, S, T, cnt;

void Dijkstra() {
	int visited[MAXN];
	for (int i = 0; i <= N; ++i) dist[i] = INF, visited[i] = 0;
	dist[T] = 0;
	for (int k, i = T; i != N; i = k) {
		visited[i] = 1; k = N;
		for (int j = 0; j < N; ++j) {
			if (visited[j] || used[j]) continue;
			if (g[j][i] > -1 && dist[i] + g[j][i] < dist[j]) {
				dist[j] = dist[i] + g[j][i];
				prev[j] = i;
			}
			if (dist[j] < dist[k]) k = j;
		}
	}
}

void setPath(int v, Tpath &p) {
	p.len = 0;
	while (1) {
		p.v[p.cnt++] = v;
		if (v == T) return;
		p.len += g[v][prev[v]]; v = prev[v];
	}
}

void solve() {
	memset(used, 0, sizeof(used));
	Dijkstra();

	memset(ans, -1, sizeof(ans));
	if (dist[S] == INF)	return;

	multimap<int, int> Q; Q.clear();
	path[0].cnt = 0; path[0].pos = 0; setPath(S, path[0]); Q.insert( make_pair(path[0].len, 0) );
	int tot = 1;

	for (int i = 0; i < K; ++i) {
		if (Q.empty()) return;

		multimap<int, int> :: iterator p = Q.begin();
		int x = (*p).second;
		ans[i] = path[x].len;

		if (i == K - 1) break;

		memset(used, 0, sizeof(used));
		Tpath cur; cur.cnt = 0; cur.len = 0;
		for (int sum = 0, j = 0; j + 1 < path[x].cnt; ++j) {
			cur.v[cur.cnt++] = path[x].v[j]; used[path[x].v[j]] = 1;
			if (j) sum += g[path[x].v[j - 1]][path[x].v[j]];
			if (j >= path[x].pos) {
				Dijkstra();
				int u = path[x].v[j];
				for (int v = 0; v < N; ++v)
					if (g[u][v] > -1 && !used[v] && dist[v] < INF && v != path[x].v[j + 1]) {
						Tpath tp = cur; tp.pos = j + 1; setPath(v, tp); tp.len += sum + g[u][v];

						if (tot < K) path[tot] = tp, Q.insert( make_pair(tp.len, tot++) );
						else {
							multimap<int, int> :: iterator p = Q.end(); --p;
							if (tp.len >= (*p).first) continue;
							path[(*p).second] = tp; Q.insert( make_pair(tp.len, (*p).second) );
							Q.erase(p);
						}
					}
			}
		}
		Q.erase(p);
	}
}

void DFS(int step, int u, int len) {
	if (!cnt) return;
	if (u == T) {
		if (len == ans[K - 1]) {
			if (!(--cnt)) {
				for (int j = 0; j < step; ++j) {
					if (j) printf("-");
					printf("%d", List[j] + 1);
				}
				printf("\n");
			}
		}
		return;
	}

	Dijkstra();

	int tmp[MAXN];
	for (int i = 0; i < N; ++i) tmp[i] = dist[i];
	for (int i = 0; i < N; ++i)
		if (g[u][i] > -1 && !used[i] && tmp[i] < INF && len + g[u][i] + tmp[i] <= ans[K - 1]) {
			used[i] = 1; List[step] = i;
			DFS(step + 1, i, len + g[u][i]);
			if (!cnt) return;
			used[i] = 0;
		}
}

int main() {
	scanf("%d%d%d%d%d", &N, &M, &K, &S, &T);
	--S; --T;
	if (S == T) ++K;

	memset(g, -1, sizeof(g));
	for (int i = 0; i < M; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		--u; --v;
		g[u][v] = w;
	}

	solve();
		
	if (ans[K - 1] == -1) printf("None\n");
	else {
		cnt = 0;
		for (int i = 0; i < K; ++i)
			if (ans[i] == ans[K - 1]) ++cnt;

		memset(used, 0, sizeof(used));
		used[S] = 1; List[0] = S;
		DFS(1, S, 0);
	}

	return 0;
}
