#include "template.cpp"

const int N = 1111;
const int inf = 2000000000;

struct Edge{
	int u, v, f, c, w;
	Edge(){
	}
	Edge(int _u, int _v, int _f, int _c, int _w) : u(_u), v(_v), f(_f), c(_c), w(_w){
	}
};

vector<Edge> E[N];
Edge* pre[N];
int dist[N];
deque<int> Q;
bool inq[N];
int n, m, s, t;

void addEdge(int u, int v, int c, int w){
	E[u].PB(Edge(u, v, 0, c, w));
	E[v].PB(Edge(v, u, 0, 0, -w));
}

bool findPath(){
	rep(i, N) dist[i] = inf, inq[i] = false;
	dist[s] = 0, inq[s] = true, Q.PB(s);
	int x, y;
	while(!Q.empty()){
		x = Q.front();Q.pop_front();
		rep(i, E[x].size()){
			Edge &e = E[x][i];
			y = e.v;
			if (e.f < e.c && dist[x] + e.w < dist[y]){
				dist[y] = dist[x] + e.w;
				pre[y] = &e;
				if (!inq[y]){
					inq[y] = true;
					Q.PB(y);
				}
			}
		}
		inq[x] = false;
	}
	return dist[t] < inf;
}

int minCost(){
	int ret = 0, now, delta;
	while(findPath()){
		delta = inf;
		now = t;
		while(now != s){
			Edge &e = *pre[now];
			delta = min(delta, e.c - e.f);
			now = e.u;
		}
	}
}

int main(){
	return 0;
}

