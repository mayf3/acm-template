/*
Give a tree with n vertices,each edge has a length(positive integer less than 1001). 
Define dist(u,v)=The min distance between node u and v. 
Give an integer k,for every pair (u,v) of vertices is called valid if and only if dist(u,v) not exceed k. 
Write a program that will count how many pairs which are valid for a given tree. 
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 10000 + 10;
const int INF = 1000000000;

struct Tedge {
	int v, w, next;
};

Tedge edge[maxn * 2];
int first[maxn], queue[maxn], sum[maxn], par[maxn], depth[maxn], dist[maxn];
bool used[maxn];
pair<int, int> stack[maxn];
int n, k, cnt;

int findRoot(int u, int n) {
	queue[0] = u; par[u] = -1;
	for (int h = 0, t = 1; h < t; ++h) {
		int u = queue[h];
		sum[u] = 1;
		for (int i = first[u]; i != -1; i = edge[i].next) {
			int v = edge[i].v;
			if (!used[v] && par[u] != v) {
				par[v] = u;
				queue[t++] = v;
			}
		}
	}
	for (int i = n - 1; i > 0; --i) sum[par[queue[i]]] += sum[queue[i]];

	int root = u, diff = INF;
	for (int i = 0; i < n; ++i) {
		int u = queue[i];
		if (sum[u] == 1) continue;
		int cnt = 0, Max = -1, Min = INF;
		if (par[u] != -1) {
			++cnt;
			Max = Min = n - sum[u];
		}
		for (int j = first[u]; j != -1; j = edge[j].next) {
			int v = edge[j].v;
			if (!used[v] && v != par[u]) {
				++cnt;
				if (sum[v] > Max) Max = sum[v];
				if (sum[v] < Min) Min = sum[v];
			}
		}
		if (cnt > 1 && Max - Min < diff) {
			diff = Max - Min;
			root = u;
		}
	}

	cnt = 0;
	int top = 1;
	stack[0] = make_pair(root, first[root]); depth[root] = 0;
	for (par[root] = -1; top > 0; ) {
		int u = stack[top - 1].first, &i = stack[top - 1].second;
		if (i == first[u]) sum[u] = 1;
		else sum[u] += sum[stack[top].first];
		while (i != -1 && (used[edge[i].v] || edge[i].v == par[u])) i = edge[i].next;
		if (i == -1) {
			--top;
			continue;
		}
		par[edge[i].v] = u; dist[cnt++] = depth[edge[i].v] = depth[u] + edge[i].w; stack[top++] = make_pair(edge[i].v, first[edge[i].v]);
		i = edge[i].next;
	}

	return root;
}

int DFS(int u, int n, int k) {
	if (n == 1) return 0;

	int root = findRoot(u, n), ans = 0;
	used[root] = 1;
	for (int *c = dist, i = first[root]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if (used[v]) continue;
		sort(c, c + sum[v]);
		for (int j = sum[v] - 1, l = 0; j >= 0; --j, ans -= l)
			while (l < sum[v] && c[j] + c[l] <= k) ++l;
		c += sum[v];
	}
	sort(dist, dist + cnt);
	for (int i = cnt - 1, j = 0; i >= 0; --i, ans += j)
		while (j < cnt && dist[i] + dist[j] <= k) ++j;
	ans = (lower_bound(dist, dist + cnt, k + 1) - dist) + ans / 2;
	for (int i = first[root]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if (!used[v]) ans += DFS(v, sum[v], k);
	}

	return ans;
}

int main() {
	while (scanf("%d%d", &n, &k), n || k) {
		memset(first, -1, sizeof(first));
		for (int i = 0; i < n - 1; ++i) {
			int u, v, w;
			scanf("%d%d%d", &u, &v, &w);
			--u; --v;
			edge[i * 2].v = v; edge[i * 2].w = w; edge[i * 2].next = first[u]; first[u] = i * 2;
			edge[i * 2 + 1].v = u; edge[i * 2 + 1].w = w; edge[i * 2 + 1].next = first[v]; first[v] = i * 2 + 1;
		}

		memset(used, 0, sizeof(used));
		printf("%d\n", DFS(0, n, k));
	}

	return 0;
}
