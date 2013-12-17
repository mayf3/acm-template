#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int n = 16;
const int sz = 4;
const int maxtot = 20000;

struct node {
	int L, R, U, D, pos, line;
};

node a[maxtot];
int size[n * n * 4 + 1], last[n * n * 4 + 1];
int board[n + 1][n + 1];
int first[n + 1][n + 1][n + 1];
bool used[n * n * n + 1];
char s[n + 10];
int tot;

inline int get(int x, int y) {
	return ((x - 1) / sz) * sz + ((y - 1) / sz);
}

void dancing_links() {
	memset(size, 0, sizeof(size));
	for (int i = 1; i <= n * n * 4; ++i) {
		a[i].L = i - 1; a[i - 1].R = i;
		a[i].R = 0; a[0].L = i;
		last[i] = i;
	}
	tot = n * n * 4;

	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n; ++j)
			for (int k = 1; k <= n; ++k) {
				first[i][j][k] = tot + 1;
				for (int x, p = 1; p <= 4; ++p) {
					if (p == 1) x = tot + 4;
					else x = tot + p - 1;
					a[tot + p].L = x; a[x].R = tot + p;
				}

				int x = (i - 1) * n + k; ++size[x];
				a[tot + 1].U = last[x]; a[last[x]].D = tot + 1;
				a[tot + 1].D = x; a[x].U = tot + 1; last[x] = tot + 1;
				a[tot + 1].pos = x; a[tot + 1].line = (i - 1) * n * n + (j - 1) * n + k;

				x = n * n + (j - 1) * n + k; ++size[x];
				a[tot + 2].U = last[x]; a[last[x]].D = tot + 2;
				a[tot + 2].D = x; a[x].U = tot + 2; last[x] = tot + 2;
				a[tot + 2].pos = x; a[tot + 2].line = (i - 1) * n * n + (j - 1) * n + k;

				x = n * n * 2 + get(i, j) * n + k; ++size[x];
				a[tot + 3].U = last[x]; a[last[x]].D = tot + 3;
				a[tot + 3].D = x; a[x].U = tot + 3; last[x] = tot + 3;
				a[tot + 3].pos = x; a[tot + 3].line = (i - 1) * n * n + (j - 1) * n + k;
 
				x = n * n * 3 + (i - 1) * n + j; ++size[x];
				a[tot + 4].U = last[x]; a[last[x]].D = tot + 4;
				a[tot + 4].D = x; a[x].U = tot + 4; last[x] = tot + 4;
				a[tot + 4].pos = x; a[tot + 4].line = (i - 1) * n * n + (j - 1) * n + k;

				tot += 4;
			}
}

void cover(int col) {
	if (a[a[col].L].R != col) return;
	a[a[col].L].R = a[col].R; a[a[col].R].L = a[col].L;
	for (int i = a[col].D; i != col; i = a[i].D)
		for (int j = a[i].R; j != i; j = a[j].R) {
			a[a[j].U].D = a[j].D; a[a[j].D].U = a[j].U;
			--size[a[j].pos];
		}
}

void recover(int col) {
	for (int i = a[col].U; i != col; i = a[i].U)
		for (int j = a[i].L; j != i; j = a[j].L) {
			a[a[j].U].D = j; a[a[j].D].U = j;
			++size[a[j].pos];
		}
	a[a[col].L].R = col; a[a[col].R].L = col;
}

bool DFS(int dep) {
	int mini = -1;
	for (int i = a[0].L; i != 0; i = a[i].L)
		if (mini < 0 || size[i] < size[mini]) mini = i;
 
	if (mini < 0) return 1;
	if (!size[mini]) return 0;

	cover(mini);
	for (int i = a[mini].D; i != mini; i = a[i].D) {
		for (int j = a[i].R; j != i; j = a[j].R) cover(a[j].pos);
		used[a[i].line] = 1;
		if (DFS(dep + 1)) return 1;
		used[a[i].line] = 0;
		for (int j = a[i].L; j != i; j = a[j].L) recover(a[j].pos);
	}
	recover(mini);
	return 0;
}

int main() {
	int tst = 0;
	while (scanf("%s", s) != EOF) {
		if (tst++) printf("\n");
		for (int i = 1; i <= n; ++i) {
			if (i > 1) scanf("%s", s);
			for (int j = 0; j < n; ++j)
				if (s[j] != '-') board[i][j + 1] = s[j] - 'A' + 1;
				else board[i][j + 1] = 0;
		}

		dancing_links();
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= n; ++j)
				if (board[i][j]) {
					int k = first[i][j][board[i][j]];
					do {
						cover(a[k].pos); k = a[k].R;
					} while (k != first[i][j][board[i][j]]);
				}
		
		memset(used, 0, sizeof(used));
		DFS(1);

		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				for (int k = 1; k <= n; ++k)
					if (used[(i - 1) * n * n + (j - 1) * n + k]) {
						board[i][j] = k;
						break;
					}
				printf("%c", 'A' + board[i][j] - 1);
			}
			printf("\n");
		}
	}

	return 0;
}
