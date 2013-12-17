/*
Big rectangle A is defined via inequalities 0 <= x <= MAXX, 0 <= y <= MAXY, where x and y are cartesian coordinates. There are N marked points inside the rectangle.
Rectangles B and C have sides parrallel to the sides of A. Intersection of B and C has null area. B and C have no marked points inside.

Please, write program which can determine maximum sum area of the rectangles B and C.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 100 + 10;

struct Tpoint {
	double x, y;
};

Tpoint p[maxn];
double x[maxn], y[maxn], f[maxn], g[maxn];
double MAXX, MAXY;
int n;

inline bool cmpx(Tpoint i, Tpoint j) {
	return i.x < j.x;
}

inline bool cmpy(Tpoint i, Tpoint j) {
	return i.y < j.y;
}

int main() {
	scanf("%lf%lf%d", &MAXX, &MAXY, &n);
	for (int i = 0; i < n; ++i) {
		scanf("%lf%lf", &p[i].x, &p[i].y);
		x[i] = p[i].x; y[i] = p[i].y;
	}
	x[n] = 0; x[n + 1] = MAXX;
	y[n] = 0; y[n + 1] = MAXY;

	sort(x, x + n + 2);
	int m = unique(x, x + n + 2) - x;
	double area1 = 0, area2 = 0;
	for (int i = 1; i < m; ++i)
		if (x[i] - x[i - 1] > area1) area2 = area1, area1 = x[i] - x[i - 1];
		else if (x[i] - x[i - 1] > area2) area2 = x[i] - x[i - 1];
	double ans = (area1 + area2) * MAXY;

	sort(y, y + n + 2);
	m = unique(y, y + n + 2) - y;
	area1 = 0, area2 = 0;
	for (int i = 1; i < m; ++i)
		if (y[i] - y[i - 1] > area1) area2 = area1, area1 = y[i] - y[i - 1];
		else if (y[i] - y[i - 1] > area2) area2 = y[i] - y[i - 1];
	if ((area1 + area2) * MAXX > ans) ans = (area1 + area2) * MAXX;

	memset(f, 0, sizeof(f));
	memset(g, 0, sizeof(g));
	sort(p, p + n, cmpx);
	for (int i = 0; i < n; ++i)
		if (p[i].x > 0 && p[i].x < MAXX && p[i].y > 0 && p[i].y < MAXY) {
			double up = MAXY, down = 0, area = 0;
			for (int j = i + 1; j < n; ++j)
				if (p[j].x > p[i].x && p[j].y < up && p[j].y > down) {
					if ((p[j].x - p[i].x) * (up - down) > area) area = (p[j].x - p[i].x) * (up - down);
					if (p[j].y > p[i].y) up = p[j].y;
					else down = p[j].y;
				}
			if ((MAXX - p[i].x) * (up - down) > area) area = (MAXX - p[i].x) * (up - down);
			f[i] = area;

			up = MAXY; down = area = 0;
			for (int j = i - 1; j >= 0; --j)
				if (p[j].x < p[i].x && p[j].y < up && p[j].y > down) {
					if ((p[i].x - p[j].x) * (up - down) > area) area = (p[i].x - p[j].x) * (up - down);
					if (p[j].y > p[i].y) up = p[j].y;
					else down = p[j].y;
				}
			if (p[i].x * (up - down) > area) area = p[i].x * (up - down);
			g[i] = area;
		}

	for (int i = n - 2; i >= 0; --i)
		if (f[i + 1] > f[i]) f[i] = f[i + 1];
	for (int i = 1; i < n; ++i)
		if (g[i - 1] > g[i]) g[i] = g[i - 1];

	for (int i = 0; i < n; ++i)
		if (f[i] + g[i] > ans) ans = f[i] + g[i];

	memset(f, 0, sizeof(f));
	memset(g, 0, sizeof(g));
	sort(p, p + n, cmpy);
	for (int i = 0; i < n; ++i)
		if (p[i].x > 0 && p[i].x < MAXX && p[i].y > 0 && p[i].y < MAXY) {
			double right = MAXX, left = 0, area = 0;
			for (int j = i + 1; j < n; ++j)
				if (p[j].y > p[i].y && p[j].x < right && p[j].x > left) {
					if ((p[j].y - p[i].y) * (right - left) > area) area = (p[j].y - p[i].y) * (right - left);
					if (p[j].x > p[i].x) right = p[j].x;
					else left = p[j].x;
				}
			if ((MAXY - p[i].y) * (right - left) > area) area = (MAXY - p[i].y) * (right - left);
			f[i] = area;

			right = MAXX; left = area = 0;
			for (int j = i - 1; j >= 0; --j)
				if (p[j].y < p[i].y && p[j].x < right && p[j].x > left) {
					if ((p[i].y - p[j].y) * (right - left) > area) area = (p[i].y - p[j].y) * (right - left);
					if (p[j].x > p[i].x) right = p[j].x;
					else left = p[j].x;
				}
			if (p[i].y * (right - left) > area) area = p[i].y * (right - left);
			g[i] = area;
		}

	for (int i = n - 2; i >= 0; --i)
		if (f[i + 1] > f[i]) f[i] = f[i + 1];
	for (int i = 1; i < n; ++i)
		if (g[i - 1] > g[i]) g[i] = g[i - 1];

	for (int i = 0; i < n; ++i)
		if (f[i] + g[i] > ans) ans = f[i] + g[i];

	printf("%.2lf\n", ans);

	return 0;
}
