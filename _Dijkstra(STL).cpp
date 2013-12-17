const int MAXN = 100000; //number of the vertices
const int INF = 2000000000; //max dist

vector < pair<int, int> > edge[MAXN];
int dist[MAXN];
int N;

void Dijkstra(int S) {
	priority_queue < pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > Q;

	for (int i = 0; i < N; ++i) dist[i] = INF;
	dist[S] = 0; Q.push( make_pair(0, S) );
	while (!Q.empty()) {
		int u = Q.top().second, d = Q.top().first;
		Q.pop();
		if (d > dist[u]) continue;
		for (int i = 0; i < edge[u].size(); ++i) {
			int v = edge[u][i].first, w = edge[u][i].second;
			if (dist[u] + w < dist[v]) {
				dist[v] = dist[u] + w;
				Q.push( make_pair(dist[v], v) );
			}
		}
	}
}
