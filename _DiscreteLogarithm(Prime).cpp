int mexp[50000], id[50000];

//ax + by = gcd(a, b)
int Ext_GCD(int a, int b, int& x, int& y)  {
	if (b == 0) {
		x = 1; y = 0;
		return a;
	}
	int g = Ext_GCD(b, a % b, x, y);
	int t = x; x = y; y = t - (a / b) * y;
	return g;
}

//ax = 1 (mod n)
int Inv(int a, int n) {
	int x, y;
	if (Ext_GCD(a, n, x, y) == 1) return (x % n + n) % n;
	else return -1;
}

//x = a ^ b (mod n), a, b >= 0
int mod_exp(int a, int b, int n) {
	long long ret = 1;
	while (b) {
		if (b & 1) ret = ret * a % n;
		a = (long long)a * a % n; b >>= 1;
	}
	return ret;
}

bool logcmp(int a, int b) {
	return mexp[a] < mexp[b] || mexp[a] == mexp[b] && a < b;
}

//a ^ x = b (mod n), n is prime
int mod_log(int a, int b, int n) {
	int m = (int)ceil(sqrt(n)), inv = Inv(mod_exp(a, m, n), n);
	id[0] = 0; mexp[0] = 1;
	for (int i = 1; i < m; ++i) id[i] = i, mexp[i] = (long long)mexp[i - 1] * a % n;
	sort(id, id + m, logcmp); sort(mexp, mexp + m);
	for (int i = 0; i < m; ++i) {
		int j = lower_bound(mexp, mexp + m, b) - mexp;
		if (j < m && mexp[j] == b) return i * m + id[j];
		b = (long long)b * inv % n;
	}
	return -1;
}
