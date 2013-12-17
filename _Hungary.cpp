const int MAXN = 1000; //number of vertices

bool a[MAXN][MAXN];
bool b[MAXN];
int c[MAXN];
int N, M;

bool match(int i) {
	b[i] = 1;
	for (int j = 0; j < M; ++j)
		if (a[i][j])
			if (c[j] == -1 || !b[c[j]] && match(c[j])) {
				c[j] = i;
				return 1;
			}
	return 0;
}

void hungary() {
	int pairs = 0;
	memset(c, -1, sizeof(c));
	for (int i = 0; i < N; ++i) {
		memset(b, 0, sizeof(b));
		if (match(i)) ++pairs;
	}
}
