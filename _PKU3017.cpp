/*
	Given an integer sequence { an } of length N,
	cut the sequence into several parts every one of which is a consecutive subsequence of the original sequence.
	Every part must satisfy that the sum of the integers in the part is not greater than a given integer M.
	You are to find a cutting that minimizes the sum of the maximum integer of each part.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 100000 + 10;
const long long INF = 1000000000000000LL;

struct node {
	long long key;
	int p;
};

node heap[maxn];
int a[maxn], q[maxn], pos[maxn];
long long f[maxn], sum[maxn];
bool del[maxn];
int n, cnt;
long long m;

void moveup(int i) {
	node curr = heap[i];
	while (i > 1 && heap[i >> 1].key > curr.key) {
		heap[i] = heap[i >> 1]; pos[heap[i].p] = i; i >>= 1;
	}
	heap[i] = curr; pos[curr.p] = i;
}

void movedown(int i) {
	node curr = heap[i];
	while ((i << 1) <= cnt) {
		int j = i << 1;
		if (j < cnt && heap[j + 1].key < heap[j].key) ++j;
		if (curr.key <= heap[j].key) break;
		heap[i] = heap[j]; pos[heap[i].p] = i; i = j;
	}
	heap[i] = curr; pos[curr.p] = i;
}

int main() {
	scanf("%d%I64d", &n, &m);
	for (int i = 1; i <= n; ++i) {
		scanf("%d", &a[i]);
		sum[i] = sum[i - 1] + a[i];
	}

	cnt = 0; f[0] = 0;
	memset(del, 0, sizeof(del));
	for (int h = 0, t = 0, j = 0, i = 1; i <= n; ++i) {
		while (h < t && sum[i] - sum[q[h] - 1] > m) del[q[h++]] = 1;
		while (h < t && a[i] >= a[q[t - 1]]) del[q[--t]] = 1;
		q[t++] = i;
		if (h + 1 < t) {
			heap[pos[q[t - 2]]].key = f[q[t - 2]] + a[i];
			moveup(pos[q[t - 2]]);
			movedown(pos[q[t - 2]]);
		}
		heap[++cnt].key = INF; heap[cnt].p = i; pos[i] = cnt;
		while (cnt && del[heap[1].p]) {
			heap[1] = heap[cnt--]; pos[heap[1].p] = 1;
			movedown(1);
		}

		f[i] = INF;
		while (sum[i] - sum[j] > m) ++j;
		if (j < i) f[i] = f[j] + a[q[h]];
		if (cnt && heap[1].key < f[i]) f[i] = heap[1].key;
	}

	if (f[n] >= INF) printf("-1\n");
	else printf("%I64d\n", f[n]);

	return 0;
}
