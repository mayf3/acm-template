/*
Given two prime numbers P and K (2 <= P <= 10^9, 2 <= K <= 100000) and integer number A (0 <= A < P).
you are to find all the roots of the equation x^K = A mod P.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

const int maxn = 40000; //maxn * maxn >= maxp
const double EPS = 1E-9;

int mexp[maxn], id[maxn];
bool isPrime[maxn + 1];
int prime[maxn];
int factor[100];
int ans[1000000];
int p, k, a, cnt, factorn, ansn;

int PrimeFilter() {
	cnt = 0;
	memset(isPrime, 1, sizeof(isPrime));
	for (int i = 2; i <= maxn; ++i) {
		if (isPrime[i])	prime[cnt++] = i;
		for (int j = 0; j < cnt && i * prime[j] <= maxn; ++j) {
			isPrime[i * prime[j]] = 0;
			if (i % prime[j] == 0) break;
		}
	}
}

void FindFactor(int x) {
	factorn = 0;
	for (int i = 0; i < cnt && prime[i] * prime[i] <= x; ++i)
		if (x % prime[i] == 0) {
			factor[factorn++] = prime[i];
			while (x % prime[i] == 0) x /= prime[i];
		}
	if (x > 1) factor[factorn++] = x;
}

// ax + by = gcd(a, b)
int Ext_GCD(int a, int b, long long& x, long long& y) {
	if (b == 0) {
		x = 1; y = 0;
		return a;
	}
	int g = Ext_GCD(b, a % b, x, y);
	long long t = x; x = y; y = t - (long long)(a / b) * y;
	return g;
}

// ax = 1 (mod n) 
int Inv(int a, int n) {
	long long x, y;
	if (Ext_GCD(a, n, x, y) == 1) return (x % n + n) % n;
	else return -1;
}

// x = a ^ b (mod n), a, b >= 0 
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

// a ^ x = b (mod n), n is prime 
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

bool JudgeGenerator(int x) {
	for (int i = 0; i < factorn; ++i)
		if (mod_exp(x, (p - 1) / factor[i], p) == 1) return 0;
	return 1;
}

int FindGenerator() {
	for (int i = 2; i < p; ++i)
		if (JudgeGenerator(i)) return i;
	return -1;
}

void solve(int g, int r) {
	long long x, y, d = Ext_GCD(k, p - 1, x, y);
	if (r % d != 0) {
		printf("0\n");
		return;
	}
	x *= r / d; y *= r / d;
	int u = (p - 1) / d; x = (x % u + u) % u;
	printf("%d\n", d);
	ansn = 0;
	while (x < p - 1) {
		ans[ansn++] = mod_exp(g, x, p);
		x += u;
	}
	sort(ans, ans + ansn);
	for (int i = 0; i < ansn; ++i) printf("%d\n", ans[i]);
}

int main() {
	scanf("%d%d%d", &p, &k, &a);
	if (a == 0) printf("1\n0\n");
	else {
		PrimeFilter();
		FindFactor(p - 1);
		int g = FindGenerator(), r = mod_log(g, a, p);
		solve(g, r);
	}
	return 0;
}
