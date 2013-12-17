const int maxn = 300 + 10;

int board[maxn], diagl[maxn * 2], diagr[maxn * 2];
int n, conflict;

void compute() {
	conflict = 0;
	memset(diagl, 0, sizeof(diagl));
	memset(diagr, 0, sizeof(diagr));
	for (int i = 1; i <= n; ++i) {
		int r = i, c = board[i];
		++diagl[r - c + n]; ++diagr[r + c - 1];
	}
	for (int i = 1; i < (n << 1); ++i) {
		if (diagl[i]) conflict += diagl[i] - 1;
		if (diagr[i]) conflict += diagr[i] - 1;
	}
}

void del(int r, int c, int& sum) {
	if (--diagl[r - c + n]) --sum;
	if (--diagr[r + c - 1]) --sum;
}

void ins(int r, int c, int& sum) {
	if (++diagl[r - c + n] > 1) ++sum;
	if (++diagr[r + c - 1] > 1) ++sum;
}

void NQueens() {
	for (int i = 1; i <= n; ++i) board[i] = i;
	while (1) {
		random_shuffle(board + 1, board + n + 1);
		compute();
		while (conflict) {
			int tot = 0;
			for (int i = 1; i <= n; ++i)
				for (int j = i + 1; j <= n; ++j) {
					int k = conflict;
					del(i, board[i], k); del(j, board[j], k);
					ins(i, board[j], k); ins(j, board[i], k);
					if (k < conflict) {
						swap(board[i], board[j]);
						conflict = k; ++tot;
					}
					else {
						ins(i, board[i], k); ins(j, board[j], k);
						del(i, board[j], k); del(j, board[i], k);
					}
				}
			if (!tot) break;
		}
		if (!conflict) break;
	}
	for (int i = 1; i <= n; ++i) printf("%d\n", board[i]);
}
