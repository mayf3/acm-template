const int MAXN = 1000; //number of vertices
const int INF = 1000000; //max weight

int a[MAXN][MAXN];
int lx[MAXN], ly[MAXN], Link[MAXN];
bool px[MAXN], py[MAXN];
int N;

bool find(int i) {
	px[i] = 1;
	for (int j = 0; j < N; ++j)
		if (a[i][j] == lx[i] + ly[j] && !py[j]) {
			py[j] = 1;
			if (Link[j] == -1 || find(Link[j])) {
				Link[j] = i;
				return 1;
			}
		}
	return 0;
}

void KM() {
	memset(Link, -1, sizeof(Link));
	memset(lx, 0, sizeof(lx));
	memset(ly, 0, sizeof(ly));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			lx[i] = max(lx[i], a[i][j]);

	for (int k = 0; k < N; ++k)
		while (1) {
			memset(px, 0, sizeof(px));
			memset(py, 0, sizeof(py));
			if (find(k)) break;

			int delta = INF;
			for (int i = 0; i < N; ++i)
				if (px[i])
					for (int j = 0; j < N; ++j)
						if (!py[j]) delta = min(delta, lx[i] + ly[j] - a[i][j]);
			if (delta == INF || delta == 0) break;

			for (int i = 0; i < N; ++i)
				if (px[i]) lx[i] -= delta;
			for (int i = 0; i < N; ++i)
				if (py[i]) ly[i] += delta;
		}
}
