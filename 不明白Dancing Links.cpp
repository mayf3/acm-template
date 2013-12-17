#define FF(i, A, s) for (int i = A[s]; i != s; i = A[i])
struct DancingLinks {
	static const int maxm = 45 * 65; // number of nodes
	static const int maxn = 65; // number of col

	int R[maxm], L[maxm], U[maxm], D[maxm], col[maxm], row[maxm];
	int S[maxn];
	bool hash[maxn];
	int pre, first, sz, NV;

	void init(int n) {
		for (int i = 0; i <= n; ++i) {
			U[i] = i, D[i] = i;
			L[i] = i - 1, R[i] = i + 1;
		}
		NV = n, sz = n + 1, pre = -1, first = 0;
		memset(S, 0, sizeof(int) * (n + 1));
	}

	// make sure j > 0
	inline void insert(int i, int j) {
		if (i != pre) {
			R[sz - 1] = first, L[first] = sz - 1;
			pre = i, first = sz;
		}
		D[U[j]] = sz, L[sz] = sz - 1, R[sz] = sz + 1;
		D[sz] = j, U[sz] = U[j], U[j] = sz;
		row[sz] = i, col[sz++] = j, ++S[j];
	}

	inline void finish() { R[sz - 1] = first; L[first] = sz - 1; }

	void EXremove(int c) {
		L[R[c]] = L[c]; R[L[c]] = R[c];
		FF(i, D, c) FF(j, R, i) U[D[j]] = U[j], D[U[j]] = D[j], --S[col[j]];
	}

	void EXresume(int c) {
		FF(i, U, c) FF(j, L, i) ++S[col[j]], U[D[j]] = j, D[U[j]] = j;
		L[R[c]] = c; R[L[c]] = c;
	}

	bool EXdfs(int deep) {
		if (R[0] == 0) return true;
		int idx = R[0];
		FF(i, R, 0) if (S[i] < S[idx]) idx = i;
		EXremove(idx);
		FF(i, D, idx) {
			FF (j, R, i) EXremove(col[j]);
			if (EXdfs(deep + 1)) return true;
			FF (j, L, i) EXresume(col[j]);
		}
		EXresume(idx);
	}

	void remove(int &c) { FF(i, D, c) L[R[i]] = L[i], R[L[i]] = R[i];}

	void resume(int &c) { FF(i, U, c) L[R[i]] = i, R[L[i]] = i;}

	int h(int ret = 0) {
		memset(hash , false , sizeof(bool) * ( NV + 1 ));
		FF(c, R, 0) if (!hash[c]) {
			++ret, hash[c] = true;
			FF(i, D, c) FF(j, R, i) hash[col[j]] = true;
		}
		return ret;
	}

	bool dfs(int deep, int lim) {
		if (deep + h() > lim) return false;
		if (R[0] == 0) return true;
		int idx = R[0]; bool f = false;
		FF(i, R, 0) if (S[i] < S[idx]) idx = i;
		FF(i, D, idx) {
			remove(i); FF(j, R, i) remove(j);
			if (dfs(deep + 1, lim)) f = true;
			FF(j, L, i) resume(j); resume(i);
			if (f) return true;
		}
		return false;
	}

	int solve(int lo, int hi) {
		while ( lo <= hi ) {
			int mid = (lo + hi) >> 1;
			if (dfs(0, mid)) hi = mid - 1;
			else lo = mid + 1;
		}
		return lo;
	}
} dlx;
