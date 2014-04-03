/*
	n*m room, '.' is bedroom, '*' is pantry, add '|', '-' between bedrooms to
	make that there will be exactly on way between any pair of them. Count the number.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 9 + 5;
const int maxhash = 10131;
const int maxcnt = 10000;
const int modo = 1000000000;

struct node {
	int state, num, next;
};

char s[maxn][maxn];
node f[2][maxcnt];
int hash[maxhash];
int con[maxn], num[maxn];
int cnt[2];
int n, m, curr, left;

void unpack(int state) {
	memset(num, 0, sizeof(num));
	for (int i = m - 1; i >= 0; --i) ++num[con[i] = state % (m + 1)], state /= m + 1;
}

int pack() {
	int ret = 0;
	for (int i = 0; i < m; ++i) ret = ret * (m + 1) + con[i];
	return ret;
}

void relabel() {
	int label[maxn], tot = 0;
	memset(label, 0, sizeof(label));
	for (int i = 0; i < m; ++i)
		if (con[i]) {
			if (!label[con[i]]) label[con[i]] = ++tot;
			con[i] = label[con[i]];
		}
}

void insert(int state, int num) {
	int key = state % maxhash;
	for (int i = hash[key]; i != -1; i = f[curr][i].next)
		if (f[curr][i].state == state) {
			f[curr][i].num = (f[curr][i].num + num) % modo;
			return;
		}
	f[curr][cnt[curr]].state = state; f[curr][cnt[curr]].num = num; f[curr][cnt[curr]].next = hash[key]; hash[key] = cnt[curr]++;
}

int main() {
	left = 0;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%s", s[i]);
		for (int j = 0; j < m; ++j)
			if (s[i][j] == '.') ++left;
	}

	cnt[0] = 1; curr = 0;
	f[0][0].state = 0; f[0][0].num = 1;
	for (int i = 0; left && i < n; ++i)
		for (int j = 0; left && j < m; ++j) {
			curr = 1 - curr;
			memset(hash, -1, sizeof(hash));
			cnt[curr] = 0;
			for (int k = 0; k < cnt[1 - curr]; ++k)
				if (s[i][j] == '*') {
					unpack(f[1 - curr][k].state);
					if (!con[j]) insert(pack(), f[1 - curr][k].num);
					else if (num[con[j]] > 1) {
						con[j] = 0;
						relabel();
						insert(pack(), f[1 - curr][k].num);
					}
				}
				else {
					unpack(f[1 - curr][k].state);
					bool left = j && con[j - 1], up = con[j];

					if (left && up && con[j - 1] != con[j]) {
						int x = con[j - 1], y = con[j];
						for (int z = 0; z < m; ++z)
							if (con[z] == y) con[z] = x;
						relabel();
						insert(pack(), f[1 - curr][k].num);
					}

					unpack(f[1 - curr][k].state);
					if (left && (!up || num[con[j]] > 1)) {
						con[j] = con[j - 1];
						relabel();
						insert(pack(), f[1 - curr][k].num);
					}

					if (up) insert(f[1 - curr][k].state, f[1 - curr][k].num);

					unpack(f[1 - curr][k].state);
					if (!up || num[con[j]] > 1) {
						con[j] = m + 1;
						relabel();
						insert(pack(), f[1 - curr][k].num);
					}
				}

			if (s[i][j] == '.') --left;
		}

	int ans = 0;
	for (int i = 0; i < cnt[curr]; ++i) {
		unpack(f[curr][i].state);

		bool b = 0;
		for (int j = 0; j < m; ++j)
			if (con[j] == 1) b = 1;
		for (int j = 0; j < m; ++j)
			if (con[j] > 1) b = 0;

		if (b) ans = (ans + f[curr][i].num) % modo;
	}

	printf("%d\n", ans);

	return 0;
}
