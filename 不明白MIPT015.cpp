/*
In the square 0 <= x <= 100, 0 <= y <= 100 there are N points with integer coordinates.
You should find out rectangle which has no any of given point inside and has maximum possible area.

Remark: Points are permitted to be on the border of the rectangle.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 100 + 10;

struct Tpoint {
	int x, y;
};

Tpoint p[maxn];
int y[maxn];
int n;

inline bool cmp(Tpoint i, Tpoint j) {
	return i.x < j.x;
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &p[i].x, &p[i].y);
		y[i] = p[i].y;
	}
	y[n] = 0; y[n + 1] = 100;

	int ans = 0;

	sort(y, y + n + 2);
	int m = unique(y, y + n + 2) - y;
	for (int i = 1; i < m; ++i)
		if (y[i] - y[i - 1] > ans) ans = y[i] - y[i - 1];
	ans *= 100;

	sort(p, p + n, cmp);
	for (int i = 0; i < n; ++i)
		if (p[i].x > 0 && p[i].x < 100 && p[i].y > 0 && p[i].y < 100) {
			int up = 100, down = 0;
			for (int j = i + 1; j < n; ++j)
				if (p[j].x > p[i].x && p[j].y < up && p[j].y > down) {
					if ((p[j].x - p[i].x) * (up - down) > ans) ans = (p[j].x - p[i].x) * (up - down);
					if (p[j].y > p[i].y) up = p[j].y;
					else down = p[j].y;
				}
			if ((100 - p[i].x) * (up - down) > ans) ans = (100 - p[i].x) * (up - down);
		}

	reverse(p, p + n);
	for (int i = 0; i < n; ++i)
		if (p[i].x > 0 && p[i].x < 100 && p[i].y > 0 && p[i].y < 100) {
			int up = 100, down = 0;
			for (int j = i + 1; j < n; ++j)
				if (p[j].x < p[i].x && p[j].y < up && p[j].y > down) {
					if ((p[i].x - p[j].x) * (up - down) > ans) ans = (p[i].x - p[j].x) * (up - down);
					if (p[j].y > p[i].y) up = p[j].y;
					else down = p[j].y;
				}
			if (p[i].x * (up - down) > ans) ans = p[i].x * (up - down);
		}

	printf("%d\n", ans);

	return 0;
}
