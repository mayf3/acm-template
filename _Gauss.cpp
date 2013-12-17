const int MAXN = 200; //number of variables
const int MAXM = 200; //number of equations

bool e[MAXM][MAXN];
bool f[MAXN], ans[MAXN];
int N, M;

void gauss() {
	int R = 0; //rank
	memset(f, 1, sizeof(f)); //free variables
	for (int i = 0; i < N; ++i) {
		int curr = R;
		while (curr < M && !e[curr][i]) ++curr;
		if (curr >= M) continue;

		f[i] = 0;

		for (int j = 0; j <= N; ++j) swap(e[curr][j], e[R][j]);

		for (int j = R + 1; j < M; ++j)
			if (e[j][i]) for (int k = i; k <= N; ++k) e[j][k] ^= e[R][k];
		++R;
	}

	for (int i = R; i < M; ++i)
		if (e[i][N]) return; //no answer

	memset(ans, 0, sizeof(ans));
	for (int k = R - 1, i = N - 1; i >= 0; --i)
		if (!f[i]) {
			ans[i] = e[k][N];
			for (int j = i + 1; j < N; ++j) if (e[k][j]) ans[i] ^= ans[j];
			--k;
		}
}
