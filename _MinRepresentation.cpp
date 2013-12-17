const int maxn = 1000000 + 10;

char s[maxn * 2];
int n;

int MinRepresentation() {
	n = strlen(s);
	for (int i = 0; i < n; ++i) s[n + i] = s[i];

	int i = 0, j = 1;
	while (i < n && j < n) {
		int k = 0;
		while (k <= n && s[i + k] == s[j + k]) ++k;
		if (k > n) break;
		if (s[i + k] < s[j + k]) j += k + 1;
		else i += k + 1;
		if (i == j) ++j;
	}
	return (i < j) ? i : j;
}
