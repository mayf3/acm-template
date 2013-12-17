typedef unsigned char byte;
const byte mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

#define tget(i) ( (t[(i) >> 3] & mask[(i) & 7]) ? 1 : 0 )
#define tset(i, b) t[(i) >> 3] = (b) ? (mask[(i) & 7] | t[(i) >> 3]) : ((~mask[(i) & 7]) & t[(i) >> 3])
#define chr(i) (cs == sizeof(int) ? ((int*)s)[i] : ((byte*)s)[i])
#define isLMS(i) (i > 0 && tget(i) && !tget(i - 1))

void getBuckets(byte *s, int *bkt, int n, int K, int cs, bool end) {
	int sum = 0;
	for (int i = 0; i <= K; ++i) bkt[i] = 0;
	for (int i = 0; i < n; ++i) ++bkt[chr(i)];
	for (int i = 0; i <= K; ++i) {
		sum += bkt[i];
		bkt[i] = end ? sum : sum - bkt[i];
	}
}

void induceSAl(byte *t, int *SA, byte *s, int *bkt, int n, int K, int cs, bool end) {
	getBuckets(s, bkt, n, K, cs, end);
	for (int j, i = 0; i < n; ++i) if ((j = SA[i] - 1) >= 0 && !tget(j)) SA[bkt[chr(j)]++] = j;
}

void induceSAs(byte *t, int *SA, byte *s, int *bkt, int n, int K, int cs, bool end) {
	getBuckets(s, bkt, n, K, cs, end);
	for (int j, i = n - 1; i >= 0; --i) if ((j = SA[i] - 1) >= 0 && tget(j)) SA[--bkt[chr(j)]] = j;
}

// find the suffix array SA of s[0..n-1] in {1..K}^n
// require s[n-1]=0 (the sentinel!), n>=2
// use a working space (excluding s and SA) of at most 2.25n+O(1) for a constant alphabet
void SA_IS(byte *s, int *SA, int n, int K, int cs) {
	byte *t = (byte*)malloc((n >> 3) + 1);
	tset(n - 2, 0); tset(n - 1, 1);
	for (int i = n - 3; i >= 0; --i) tset(i, (chr(i) < chr(i + 1) || (chr(i) == chr(i + 1) && tget(i + 1) == 1)) ? 1 : 0);

	int *bkt = (int*)malloc(sizeof(int) * (K + 1));
	getBuckets(s, bkt, n, K, cs, true);
	for (int i = 0; i < n; ++i) SA[i] = -1;
	for (int i = 1; i < n; ++i) if (isLMS(i)) SA[--bkt[chr(i)]] = i;
	induceSAl(t, SA, s, bkt, n, K, cs, 0); induceSAs(t, SA, s, bkt, n, K, cs, 1);
	free(bkt);

	int n1 = 0;
	for (int i = 0; i < n; ++i) if (isLMS(SA[i])) SA[n1++] = SA[i];
	for (int i = n1; i < n; ++i) SA[i] = -1;
	int name = 0, prev = -1;
	for (int i = 0; i < n1; ++i) {
		int pos = SA[i];
		bool diff = 0;
		for (int d = 0; d < n; ++d)
			if (prev == -1 || chr(pos + d) != chr(prev + d) || tget(pos + d) != tget(prev + d)) {
				diff = 1;
				break;
			}
			else if (d > 0 && (isLMS(pos + d) || isLMS(prev + d))) break;
			if (diff) {
				++name; prev = pos;
			}
			pos = (!(pos & 1)) ? (pos >> 1) : ((pos - 1) >> 1); SA[n1 + pos] = name - 1;
	}
	for (int i = n - 1, j = n - 1; i >= n1; --i) if (SA[i] >= 0) SA[j--] = SA[i];

	int *SA1 = SA, *s1 = SA + n - n1;
	if (name < n1) SA_IS((byte *) s1, SA1, n1, name - 1, sizeof(int));
	else for (int i = 0; i < n1; ++i) SA1[s1[i]] = i;

	bkt = (int*)malloc(sizeof(int) * (K + 1));
	getBuckets(s, bkt, n, K, cs, 1);
	for (int i = 1, j = 0; i < n; ++i) if (isLMS(i)) s1[j++] = i;
	for (int i = 0; i < n1; ++i) SA1[i] = s1[SA1[i]];
	for (int i = n1; i < n; ++i) SA[i] = -1;
	for (int i = n1 - 1; i >= 0; --i) {
		int j = SA[i];
		SA[i] = -1; SA[--bkt[chr(j)]] = j;
	}
	induceSAl(t, SA, s, bkt, n, K, cs, 0); induceSAs(t, SA, s, bkt, n, K, cs, 1);
	free(bkt); free(t);
}
