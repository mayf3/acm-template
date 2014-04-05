const int limit = 1000000; //limit of brute-force
const int maxfn = 100;
const int maxL = 10;

bool b[limit];
long long p[limit];
pair <long long, int> f[maxfn];
long long n;
int pn, fn;

void init() {
	pn = 0;
	memset(b, 1, sizeof(b));
	for (int i = 2; i < limit; ++i)
		if (b[i]) {
			p[pn++] = i;
			for (int j = i + i; j < limit; j += i) b[j] = 0;
		}
}

long long mod_mul(long long a, long long b, long long n) {
	if (a <= 0x7fffffff && b <= 0x7fffffff) return a * b % n;
	long long len = 61, ret = 0;
	for (long long p = 8; p < n; len -=4, p <<= 4);
	for (long long dig = (1LL << len) - 1; b > 0; b >>= len) {
		if (b & dig) ret = (ret + a * (b & dig)) % n;
		a = (a << len) % n;
	}
	return ret;
}

long long mod_exp(long long a, long long b, long long n) {
	long long ret = 1;
	while (b) {
		if (b & 1) ret = mod_mul(ret, a, n);
		a = mod_mul(a, a, n); b >>= 1;
	}
	return ret;
}

long long gcd(long long a, long long b) {
	if (!b) return a;
	else return gcd(b, a % b);
}

bool Miller_Rabin(long long n) {
	if (n < 2) return 0;
	if (n == 2) return 1;
	if (!(n & 1)) return 0;

	for (int i = 0; i < 20 && p[i] < n; ++i)
		if (mod_exp(p[i], n - 1, n) != 1) return 0;
	return 1;
}

void factor(long long n) {
	fn = 0;
	for (int i = 0; i < pn && p[i] * p[i] <= n; ++i)
		if (n % p[i] == 0) {
			int cnt = 0;
			while (n % p[i] == 0) n /= p[i], ++cnt;
			f[fn++] = make_pair(p[i], cnt);
		}
	if (n == 1) return;

	long long x = 5, y = 2, k = 1, l = 1;
	while (!Miller_Rabin(n)) {
		while (1) {
			long long g = gcd((y - x + n) % n, n);
			if (g == 1) {
				if ((--k) == 0) y = x, l <<= 1, k = l;
				x = (mod_mul(x, x, n) + 1) % n;
				continue;
			}
			int cnt = 0;
			while (n % g == 0) n /= g, ++cnt;
			f[fn++] = make_pair(g, cnt);
			if (n == g) return;
			n /= g; x %= n; y %= n;
			break;
		}
	}
	f[fn++] = make_pair(n, 1);
}
