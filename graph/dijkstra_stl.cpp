#include "template.cpp"

/*
 * name 	: 	dijkstra(STL)
 * usage 	:	single-source shortest path(only non-negative weight)
 * develop	:	small label first optimization, negative circle
 * space complexity	:	O(M)
 * time complexity	:	O(NlogN)
 * checked	:	no
 */

const int N = 111111; //number of the vertices

int n, m;
int dist[N];
vector<PII> E[N];

int calc(int s, int e) {
	priority_queue<PII, vector<PII>, greater<PII> > Q;
	rep(i, n) dist[i] = -1;
	dist[s] = 0;
	Q.push(MP(0, s));
	int x, y, cost;
	while (!Q.empty()) {
		x = Q.top().Y, cost = Q.top().X;
		Q.pop();
		if (cost > dist[x]) continue;
		rep(i, E[x].size()){
			y = E[x][i].X, cost = E[x][i].Y;
			if (dist[y] == -1 || dist[y] != -1 && dist[x] + cost > dist[y]){
				dist[y] = dist[x] + cost;
				Q.push( make_pair(dist[y], y) );
			}
		}
	}
	return dist[e];
}

int main(){
	while(~scanf("%d%d", &n, &m)){
		rep(i, n) E[i].clear();
		int x, y, c;
		rep(i, m){
			scanf("%d%d%d", &x, &y, &c);
			x--, y--;
			E[x].PB(MP(y, c));
			E[y].PB(MP(x, c));
		}
		printf("%d\n", calc(0, n - 1));
	}
	return 0;
}
