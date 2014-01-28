#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 1000 + 10; //number of vertices
const int MAXM = 100000 + 10; //number of edges
const int MAXK = 1000 + 10;
const int MAXH = 200000; //M + N log N
const int INF = 1000000000; //max dist

struct Theap {
	int idx, dep;
	int chd[3];
};

struct Tedge {
	int u, v, w, delta;
	bool inT;
};

Theap heap[MAXH];
Tedge edge[MAXM];
int first[MAXN], rfirst[MAXN], outdeg[MAXN], dist[MAXN], nextT[MAXN], list[MAXN], H1[MAXN], H2[MAXN];
int next[MAXM], rnext[MAXM];
int ans[MAXK];
int N, M, K, S, T, nlist, H, curedge;

void Dijkstra() {
	priority_queue < pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > Q;

	for (int i = 0; i < N; ++i)	dist[i] = INF;
	dist[T] = 0; Q.push( make_pair(0, T) );
	while (!Q.empty()) {
		int u = Q.top().second, d = Q.top().first;
		Q.pop();
		if (d > dist[u]) continue;
		for (int i = rfirst[u]; i != -1; i = rnext[i]) {
			int v = edge[i].u, w = edge[i].w;
			if (dist[u] + w < dist[v]) {
				dist[v] = dist[u] + w;
				Q.push( make_pair(dist[v], v) );
			}
		}
	}
}

void DFS(int u) {
	list[nlist++] = u;
	for (int i = rfirst[u]; i != -1; i = rnext[i]) {
		int v = edge[i].u, w = edge[i].w;
		if (!edge[i].delta && nextT[v] == -1) {
			nextT[v] = u; edge[i].inT = 1;
			DFS(v);
		}
	}
}

int buildH1(int Size, int dep) {
	if (!Size) return 0;

	if (edge[curedge].inT) curedge = next[curedge];

	int cur = H++;
	heap[cur].idx = curedge; curedge = next[curedge];

	heap[cur].chd[2] = 0;
	if (!dep) heap[cur].chd[0] = buildH1(Size - 1, dep + 1), heap[cur].chd[1] = 0;
	else {
		int half = (Size - 1) / 2;
		heap[cur].chd[0] = buildH1(half, dep + 1); heap[cur].chd[1] = buildH1(Size - 1 - half, dep + 1);
	}

	int i = cur;
	while (1) {
		int k = i;
		for (int j = 0; j < 2; ++j)
			if (heap[i].chd[j] && edge[heap[heap[i].chd[j]].idx].delta < edge[heap[k].idx].delta) k = heap[i].chd[j];
		if (k == i) break;
		swap(heap[k].idx, heap[i].idx); i = k;
	}

	return cur;
}

int buildH2(int a, int b) {
	if (!a) {
		heap[b].chd[0] = heap[b].chd[1] = 0; heap[b].dep = 1;
		return b;
	}

	int Next = heap[heap[a].chd[0]].dep >= heap[heap[a].chd[1]].dep;

	int cur = H++;
	heap[cur] = heap[a];
	if (edge[heap[b].idx].delta < edge[heap[a].idx].delta) {
		heap[b].chd[0] = heap[a].chd[0]; heap[b].chd[1] = heap[a].chd[1];
		heap[b].chd[Next] = buildH2(heap[b].chd[Next], cur);
		heap[b].dep = min(heap[heap[b].chd[0]].dep, heap[heap[b].chd[1]].dep) + 1;
		return b;
	}
	else {
		heap[cur].chd[Next] = buildH2(heap[cur].chd[Next], b);
		heap[cur].dep = min(heap[heap[cur].chd[0]].dep, heap[heap[cur].chd[1]].dep) + 1;
		return cur;
	}
}

void solve() {
	Dijkstra();

	memset(ans, -1, sizeof(ans));
	if (dist[S] == INF) return;

	for (int i = 0; i < M; ++i) edge[i].delta = edge[i].w - dist[edge[i].u] + dist[edge[i].v];

	memset(nextT, -1, sizeof(nextT));
	nextT[T] = -2; nlist = 0;
	DFS(T);

	H = 1; heap[0].dep = 0;
	memset(H1, 0, sizeof(H1));
	for (int i = 0; i < N; ++i)
		if (dist[i] < INF) {
			int Size = outdeg[i];
			if (i != T) --Size;
			curedge = first[i];
			H1[i] = buildH1(Size, 0);
			if (H1[i]) {
				heap[H1[i]].chd[2] = heap[H1[i]].chd[0];
				heap[H1[i]].chd[0] = 0;
				heap[H1[i]].dep = 1;
			}
		}

	memset(H2, 0, sizeof(H2));
	H2[T] = H1[T];
	for (int i = 1; i < nlist; ++i) {
		int j = list[i];
		if (!H1[j]) H2[j] = H2[nextT[j]];
		else H2[j] = buildH2(H2[nextT[j]], H1[j]);
	}

	ans[0] = dist[S];

	priority_queue < pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > Q;
	if (H2[S]) Q.push( make_pair(edge[heap[H2[S]].idx].delta, H2[S]) );
	for (int i = 1; i < K; ++i) {
		if (Q.empty()) break;

		int u = Q.top().second, d = Q.top().first;
		ans[i] = dist[S] + d;
		Q.pop();

		for (int j = 0; j < 3; ++j) {
			int v = heap[u].chd[j];
			if (v) Q.push( make_pair(d + edge[heap[v].idx].delta - edge[heap[u].idx].delta, v) );
		}
		int v = H2[edge[heap[u].idx].v];
		if (v) Q.push( make_pair(d + edge[heap[v].idx].delta, v) );
	}
}

int main() {
	memset(first, -1, sizeof(first));
	memset(rfirst, -1, sizeof(rfirst));
	memset(outdeg, 0, sizeof(outdeg));

	scanf("%d%d", &N, &M);
	for (int i = 0; i < M; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		--u; --v;

		edge[i].u = u; edge[i].v = v; edge[i].w = w; edge[i].inT = 0;
		next[i] = first[u]; first[u] = i; rnext[i] = rfirst[v]; rfirst[v] = i;
		++outdeg[u];
	}

	scanf("%d%d%d", &S, &T, &K);
	--S; --T;
	if (S == T) ++K;

	solve();

	printf("%d\n", ans[K - 1]);

	return 0;
}
