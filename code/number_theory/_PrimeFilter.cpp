const int maxn = 1000;

bool isPrime[maxn + 1];
int prime[maxn];
int cnt;

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