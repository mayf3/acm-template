long long mul_mod(long long x, long long y, long long n) {
	if (!x) return 0;
	return (((x & 1) * y) % n + (mul_mod(x >> 1, y, n) << 1) % n) % n;
}

long long pow_mod(long long a, long long x, long long n) {
	long long ret = 1;
	while (x) {
		if (x & 1) ret = mul_mod(ret, a, n);
		a = mul_mod(a, a, n); x >>= 1;
	}
	return ret;
}

bool millerRabin(long long a, long long n) {
	long long r = 0, s = n - 1;
	while (!(s & 1)) {
		s >>= 1;
		++r;
	}
	long long x = pow_mod(a, s, n);
	if (x == 1 || x == n - 1) return 1;
	for (int j = 1; j < r; ++j) {
		x = mul_mod(x, x, n);
		if (x == 1) return 0;
		if (x == n - 1) return 1;
	}
	return 0;
}

/*
	if n < 1,373,653, it is enough to test a = 2 and 3;
	if n < 9,080,191, it is enough to test a = 31 and 73;
	if n < 4,759,123,141, it is enough to test a = 2, 7, and 61;
	if n < 2,152,302,898,747, it is enough to test a = 2, 3, 5, 7, and 11;
	if n < 3,474,749,660,383, it is enough to test a = 2, 3, 5, 7, 11, and 13;
	if n < 341,550,071,728,321, it is enough to test a = 2, 3, 5, 7, 11, 13, and 17.
*/
bool isPrime(long long n) {
	for (int i = 2; i < 1000 && i < n; ++i)
		if (n % i == 0) return 0;

	if (!millerRabin(2, n)) return 0;
	if (!millerRabin(3, n)) return 0;
	if (!millerRabin(5, n)) return 0;
	if (!millerRabin(7, n)) return 0;

	return 1;
}