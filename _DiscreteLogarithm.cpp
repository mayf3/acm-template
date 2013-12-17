int gcd(int a, int b) {
	if (!b) return a;
	else return gcd(b, a % b);
}

int ext_gcd(int a, int b, int &x, int &y) {
	if (!b) {
		x = 1; y = 0;
		return a;
	}
	else {
		int g = ext_gcd(b, a % b, x, y);
		int t = x; x = y; y = t - (a / b) * y;
		return g;
	}
}

int Inval(int a, int b, int n) {
	int x, y;
	ext_gcd(a, n, x, y);
	int e = (long long)x * b % n;
	return e < 0 ? e + n : e;
}

int mod_exp(int a, int b, int n) {
	a %= n;
	long long ret = 1 % n;
	while (b) {
		if (b & 1) ret = ret * a % n;
		a = (long long)a * a % n; b >>= 1;
	}
	return ret;
}

// A ^ x = B (mod C)
// return x, -1 means no solution
int Dlog(int A, int B, int C) {
	map<int, int> Hash;
	int D = 1 % C;
	for(int j = D, i = 0;i <= 100; j = (long long)j * A % C, ++i)
		if (j == B) return i;
	int d = 0;
	for (int g; (g = gcd(A, C)) != 1; ) {
		if (B % g)return -1;
		++d; C /= g; B /= g; D = (long long)D * A / g % C;
	}
	int M = (int)ceil(sqrt((double)C));
	for (int j = 1 % C, i = 0; i <= M; j = (long long)j * A % C, ++i)
		if (Hash.find(j) == Hash.end()) Hash[j] = i;
	for (int j = mod_exp(A, M, C), i = 0; i <= M; D = (long long)D * j % C, ++i) {
		int tmp = Inval(D, B, C);
		if (tmp >= 0 && Hash.find(tmp) != Hash.end()) return i * M + Hash[tmp] + d;
	}
	return -1;
}
