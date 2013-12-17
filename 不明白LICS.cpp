const int MAXN = 1000; //lenth of sequence
const int limit = 300;

int a[MAXN], b[MAXN], u[MAXN], v[MAXN], list[MAXN];
bool used[MAXN];
int n, m, ans;

int cal_LIS(int a[], int f[], int x, int y) {
	int ret = 0;
	for (int i = 0; i < n; ++i) {
		if (!y && a[i] <= x || y && a[i] >= x) {
			if (i) f[i] = f[i - 1];
			else f[i] = 0;
			continue;
		}

		int l = 1, r = ret;
		while (l <= r) {
			int m = (l + r) / 2;
			if (!y)
				if (a[b[m]] < a[i]) l = m + 1;
				else r = m - 1;
			else
				if (a[b[m]] > a[i]) l = m + 1;
				else r = m - 1;
		}
		ret = max(ret, l);
		b[l] = i; f[i] = l;

		if (i && f[i - 1] > f[i]) f[i] = f[i - 1];
	}
	return ret;
}

void prep() {
	m = cal_LIS(a, u, -1, 0);
	for (int i = 1; i <= m; ++i) list[i - 1] = b[i];

	if (m > limit) {
		memset(used, 0, sizeof(used));
		for (int i = 0; i < limit; ++i) {
			int j;
			do j = rand() % m; while (used[j]);
			used[j] = 1;
		}
		for (int i = 0, j = 0; i < m; ++i)
			if (used[i]) list[j++] = list[i];
		m = limit;
	}
}

void solve(int p) {
	int curr = a[p];
	int t[MAXN];

	for (int i = 0; i < n; ++i) t[i] = a[(i + p + 1) % n];

	memset(u, 0, sizeof(u));
	cal_LIS(t, u, curr, 0);

	reverse(t, t + n);
	memset(v, 0, sizeof(v));
	cal_LIS(t, v, curr, 1);

	for (int i = 0; i <= n; ++i) {
		int tmp = 1;
		if (i) tmp += u[i - 1];
		if (n - i - 1 > -1) tmp += v[n - i - 1];
		ans = max(ans, tmp);
	}
}

int LICS() {
	srand(time(NULL));
	ans = 0;
	prep();
	for (int i = 0; i < m; ++i) solve(list[i]);
	return ans;
}
