#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>

#define Sqr(x) (x) * (x)

using namespace std;

const double EPS = 1E-8;
const double INF = 1E+9;
const double PI = acos(-1.0);

inline int sign(double x){
	return x < -EPS ? -1 : x > EPS;
}

/*
 * Point 
 */
struct Point{
	double x, y;
	Point(double x = 0, double y = 0) : x(x), y(y){}
	bool operator<(const Point &p) const{
		return sign(x - p.x) * 2 + sign(y - p.y) < 0;
	}
	bool operator==(const Point &p) const {
		return sign(x - p.x) == 0 && sign(y - p.y) == 0;
	}
};

void read(Point *p){
	scanf("%lf%lf", &p->x, &p->y);
}

void print(Point &p){
	printf("%.3lf %.3lf\n", p.x, p.y);
}

Point operator+(Point a, Point b){
	return Point(a.x + b.x, a.y + b.y);
}

Point operator-(Point a, Point b){
	return Point(a.x - b.x, a.y - b.y);
}

double cross(Point a, Point b, Point c){
	return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

double dist(Point a, Point b){
	return hypot(a.x - b.x, a.y - b.y);
}

/*
 * mid point
 */
Point mp(Point a, Point b){
	return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

/*
 * next point
 */
Point np(Point a, double alpha, double d){
	return Point(a.x + d * cos(alpha), a.y + d * sin(alpha));
}

/*
 * PA * PB
 */
double dot(Point p, Point a, Point b){
	return (a.x - p.x) * (b.x - p.x) + (a.y - p.y) * (b.y - p.y);
}

/*
 * delta angle of ABC
 */
inline double delta_angle(Point a, Point b, Point c){
	return acos(dot(b, a, c) / (dist(a, b) * dist(b, c)));
}

/*
 * if ABC a sharp triangle
 */
int sharp(Point a, Point b, Point c){
	return sign(dot(a, b, c)) > 0 && sign(dot(b, a, c)) > 0 && sign(dot(c, a, b)) > 0;
}

double fix(double a, double b = 0){
	a -= b;
	if (sign(a) < 0) a += 2 * PI;
	if (sign(a - 2 * PI) >= 0) a -= 2 * PI;
	return a;
}

double angle(Point a, Point b){
	return fix(atan2(b.y - a.y, b.x - a.x));
}

Point rotate(Point a, Point b, double alpha){
	double s, c, x, y;
	a.x -= b.x;
	a.y -= b.y;
	c = cos(alpha);
	s = sin(alpha);
	x = a.x * c - a.y * s;
	y = a.x * s + a.y * c;
	a.x = x + b.x;
	a.y = y + b.y;
	return a;
}

double area_heron(double a, double b, double c){
	double s = (a + b + c) / 2.0;
	if (sign(a - s) >= 0 || sign(b - s) >= 0 || sign(c - s) >= 0) return -1;
	return sqrt(s * (s - a) * (s - b) * (s - c));
}

double area_triangle(Point a, Point b, Point c){
	return fabs(a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - b.x * a.y - c.x * b.y) / 2;
}

double area_polygon(Point a[], int n){
	double area = 0;
	for(int j = 0, i = n - 1; j < n; i = j++){
		area += a[i].x * a[j].y - a[i].y * a[j].x;
	}
	return fabs(area) / 2;
}

Point centroid(Point a[], int n){
	double area = 0;
	Point c;
	for(int i = 0; i < n; i++){
		int j = (i + 1) % n;
		area += a[i].x * a[j].y - a[i].y * a[j].x;
		c.x += (a[i].x + a[j].x) * (a[i].x * a[j].y - a[i].y * a[j].x);
		c.y += (a[i].y + a[j].y) * (a[i].x * a[j].y - a[i].y * a[j].x);
	}
	area = fabs(area) / 2;
	c.x /= 6 * area;
	c.y /= 6 * area;
	return c;
}

/*
 * convex hull
 */

Point __o;

bool acmp(Point a, Point b){
	int c = sign(cross(__o, a, b));
	return c > 0 || !c && dist(a, __o) < dist(b, __o);
}

/*
 * find convex hull of p[n] in place
 * return # of points of resulting convex hull
 */
int find_convex(Point p[], int n){
	__o = *min_element(p, p + n);
	sort(p, p + n, acmp);
	int top = 0;
	Point *stack = new Point[n]; //XXX new here!
	for(int i = 0; i < n; i++){
		while(top >= 2 && sign(cross(stack[top - 2], stack[top - 1], p[i])) <= 0) top--;
		stack[top++] = p[i];
	}
	copy(stack, stack + top, p);
	free(stack);
	return top;
}

/*
 * rotate calipers
 */
double shadow_length(double alpha, Point a, Point b){
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	double c = cos(alpha);
	double s = sin(alpha);
	return fabs(dx * c + dy * s);
}

/*
 * min area & min peri rectangle covering, using rotate calipers
 */
void rotate_calipers(Point ps[], int n, double &area, double &peri){
	area = peri = INF;
	n = find_convex(ps, n);
	ps[n] = ps[0];
	Point *q[4] = {NULL, NULL, NULL, NULL};
	for(int i = 0; i < n; i++){
		Point *p = &ps[i];
		if (!q[0] || q[0]->y > p->y || q[0]->y == p->y && q[0]->x > p->x) q[0] = p;
		if (!q[1] || q[1]->x < p->x || q[1]->x == p->x && q[1]->y > p->y) q[1] = p;
		if (!q[2] || q[2]->y < p->y || q[2]->y == p->y && q[2]->x < p->x) q[2] = p;
		if (!q[3] || q[3]->x > p->x || q[3]->x == p->x && q[3]->y < p->y) q[3] = p;
	}
	double alpha = 0;
	for(int k = 0; k < n + 5; k++){
		int bi = -1;
		double gap_min = INF;
		for(int i = 0; i < 4; i++){
			double gap = fix(angle(q[i][0], q[i][1]), alpha + i * PI / 2);
			if (gap < gap_min){
				gap_min = gap;
				bi = i;
			}
		}
		if (++q[bi] == ps + n) q[bi] = ps + 0;
		alpha = fix(alpha + gap_min);
		double a = shadow_length(alpha + PI / 2, *q[0], *q[2]);
		double b = shadow_length(alpha, *q[1], *q[3]);
		area = min(area, a * b);
		peri = min(peri, a + a + b + b);
	}
}

/*
 * lines
 */
struct Line{
	Point p, q;
	Line(){}
	Line(Point p, Point q) : p(p), q(q) {}
};

int parallel(Line u, Line v){
	return !sign((u.p.x - u.q.x) * (v.p.y - v.q.y) - (v.p.x - v.q.x) * (u.p.y - u.q.y));
}

/*
 * same side : 1
 * at least one of a, b touches l : 0;;
 * other wise : -1
 */
int side(Line m, Point p, Point q){
	return sign(cross(m.p, m.q, p)) * sign(cross(m.p, m.q, q));
}

int on_line(Line l, Point p){
	return !sign(cross(l.p, l.q, p));
}

/*
 * u, v : line
 */
int coinside(Line u, Line v){
	return on_line(u, v.p) && on_line(u, v.q);
}

/*
 * u, v : line segment, inclusive
 */
int intersected(Line u, Line v){
	return !parallel(u, v) && side(u, v.p, v.q) <= 0 && side(v, u.p, u.q) <= 0;
}

/*
 * u, v : line segment , exclusive
 */
int intersected_exclusive(Line u, Line v){
	return !parallel(u, v) && side(u, v.p, v.q) < 0 && side(v, u.p, u.q) < 0;
}

/*
 * intersection point
 * must check whether exist or not
 */
Point ip(Line u, Line v){
	double n = (u.p.y - v.p.y) * (v.q.x - v.p.x) - (u.p.x - v.p.x) * (v.q.y - v.p.y);
	double d = (u.q.x - u.p.x) * (v.q.y - v.p.y) - (u.q.y - u.p.y) * (v.q.x - v.p.x);
	double r = n / d;
	return Point(u.p.x + r * (u.q.x - u.p.x), u.p.y + r * (u.q.y - u.p.y));
}

/*
 * if P on the Line segment l, inclusive
 */
int on_lineseg(Line l, Point p){
	return on_line(l, p) && sign(dot(p, l.p, l.q)) <= 0;
}

/*
 * if P on the segment l, exclusive 
 */
int on_lineseg_exclusive(Line l, Point p){
	return on_line(l, p) && sign(dot(p, l.p, l.q)) < 0;
}

double dist_line_point(Line l, Point a){
	return fabs(cross(l.p, l.q, a)) / dist(l.p, l.q);
}

double dist_lineseg_point(Line l, Point a){
	if (on_lineseg(l, a)) return 0;
	if (on_line(l, a) || !sharp(l.p, a, l.q)) return min(dist(l.p, a), dist(l.q, a));
	return dist_line_point(l, a);
}

/*
 * u : line segment
 * ab : ray, if p is the resulting intersection point
 */
int intersected_lineseg_ray(Line u, Line v, Point &p){
	if (parallel(u, v)) return 0;
	p = ip(u, v);
	return on_lineseg(u, p) && (on_lineseg(v, p) || on_lineseg(Line(v.p, p), v.q));
}

/*
 * if point a inside polygon p[n]
 */
int inside_polygon(Point p[], int n, Point a){
	double sum = 0;
	for(int i = 0; i < n; i++){
		int j = (i + 1) % n;
		if (on_lineseg(Line(p[i], p[j]), a)) return 0;
		double angle = acos(dot(a, p[i], p[j]) / dist(a, p[i]) / dist(a, p[j]));
		sum += sign(cross(a, p[i], p[j])) * angle;
	}
	return sign(sum);
}

/*
 * if lineseg l strickly inside polygon p[n]
 */
int lineseg_inside_polygon(Point p[], int n, Line l){
	for(int i = 0; i < n; i++){
		int j = (i + 1) % n;
		Line l1(p[i], p[j]);
		if (on_lineseg_exclusive(l, p[i])) return 0;
		if (intersected_exclusive(l, l1)) return 0;
	}
	return inside_polygon(p, n, mp(l.p, l.q));
}

/*
 * if lineseg l intersect convex polygon p[n]
 */
int intersect_convex_lineseg(Point p[], int n, Line l){
	if (n < 3) return 0;
	Point q[4];
	int k = 0;
	q[k++] = l.p;
	q[k++] = l.q;
	for(int i = 0; i < n; i++){
		if (on_lineseg(l, p[i])){
			q[k++] = p[i];
		}
		else{
			int j = (i + 1) % n;
			Line a(p[i], p[j]);
			Point tmp = ip(a, l);
			if (on_lineseg(l, tmp) && on_lineseg(a, tmp)) q[k++] = tmp;
		}
	}
	sort(q, q + k);
	for(int i = 0; i + 1 < k; i++){
		if (inside_polygon(p, n, mp(q[i], q[i + 1]))) return 1;
	}
	return 0;
}

Line perpendicular(Line l, Point a){
	return Line(a, Point(a.x + l.p.y - l.q.y, a.y + l.q.x - l.p.x));
}

Point pedal(Line l, Point a){
	return ip(l, perpendicular(l, a));
}

Point mirror(Line l, Point a){
	Point p = pedal(l, a);
	return Point(p.x * 2 - a.x, p.y * 2 - a.y);
}

Point perpencenter(Point a, Point b, Point c){
	Line u = perpendicular(Line(b, c), a);
	Line v = perpendicular(Line(a, c), b);
	return ip(u, v);
}

/*
 * Inscribed circle center
 */
Point icc(Point A, Point B, Point C){
	double a = dist(B, C);
	double b = dist(C, A);
	double c = dist(A, B);
	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	Point cp;
	cp.x = (a * A.x + b * B.x + c * C.x) / (a + b + c);
	cp.y = (a * A.y + b * B.y + c * C.y) / (a + b + c);
	return cp;
}

/*
 * Perpendicular bisector
 */
Line pb(Point a, Point b){
	return perpendicular(Line(a, b), mp(a, b));
}

/*
 * circumcicle center
 */
Point ccc(Point A, Point B, Point C){
	double a1 = B.x - A.x, b1 = B.y - A.y, c1 = (Sqr(a1) + Sqr(b1)) / 2;;
	double a2 = C.x - A.x, b2 = C.y - A.y, c2 = (Sqr(a2) + Sqr(b2)) / 2;;
	double d = a1 * b2 - a2 * b1;
	Point cp;
	cp.x = A.x + (c1 * b2 - c2 * b1) / d;
	cp.y = A.y + (a1 * c2 - a2 * c1) / d;
	return cp;
}

/*
 * translate l with distance e and direction s
 */
Line translate(Line l, double e, int s){
	double d = dist(l.p, l.q);
	double x = l.p.y - l.q.y;
	double y = l.q.x - l.p.x;
	x *= s * e / d;
	y *= s * e / d;
	l.p.x += x; l.p.y += y;
	l.q.x += x; l.q.y += y;
	return l;
}

/*
 * area of the part of convex polygon p[n] on the positive side of l
 */
double cut_area(Point *p, int n, Line l){
	int ai, bi;
	Point ap, bp;
	ai = bi = -1;
	for(int i = 0; i < n; i++){
		Line v(p[i], p[i + 1]);
		if (parallel(v, l)) continue;
		Point cp = ip(v, l);
		if (cp == p[i] || on_lineseg_exclusive(v, cp)){
			if (ai == -1){
				ai = i;
				ap = cp;
			}
			else{
				bi = i;
				bp = cp;
			}
		}
	}
	Point *q = new Point[n + 2]; //XXX new here
	int m = 0;
	for(int i = 0; i < n; i++){
		if (sign(cross(l.p, l.q, p[i])) >= 0){
			q[m++] = p[i];
		}
		if (i == ai){
			q[m++] = ap;
		}
		if (i == bi){
			q[m++] = bp;
		}
		double res = area_polygon(q, m);
		free(q);
		return res;
	}
}

/*
 * -----intersection points convex hull--------
 */
bool lcmp(Line u, Line v){
	int c = sign((u.p.x - u.q.x) * (v.p.y - v.q.y) - (v.p.x - v.q.x) * (u.p.y - u.q.y));
	return c < 0 || !c && sign(cross(u.p, u.q, v.p)) < 0;
}

/*
 * XXX sizeof(p) MUST be as large as n * 2
 * return # of points of resulting convex hull
 */
int ip_convex(Line l[], int n, Point p[]){
	for(int i = 0; i < n; i++){
		if (l[i].q < l[i].p) swap(l[i].p, l[i].q);
	}
	sort(l, l + n, lcmp);
	int n1 = 0;
	for(int i = 0, j = 0; i < n; i = j){
		while(j < n && parallel(l[i], l[j])) j++;
		if (j - i == 1){
			l[n1++] = l[i];
		}
		else{
			l[n1++] = l[i];
			l[n1++] = l[j - 1];
		}
	}
	n = n1;
	l[n + 0] = l[0];
	l[n + 1] = l[1];
	int m = 0;
	for(int i = 0, j = 0; i < n; i++){
		while(j < n + 2 && parallel(l[i], l[j])) j++;
		for(int k = j; k < n + 2 && parallel(l[j], l[k]);k++){
			p[m++] = ip(l[i], l[k]);
		}
	}
	return find_convex(p, m);
}

/*
 * ---------circles----------------
 */
struct Circle{
	Point o;
	double r;
	Circle(Point o = Point(), double r = 1) : o(o), r(r){}
	Circle(double x, double y, double r = 1) : o(x, y), r(r){}
};

int intersected_circle_line(Circle c, Line l){
	return sign(dist_line_point(l, c.o) - c.r) < 0;
}

int ip_circle_line(Circle c, Line l, Point &p1, Point &p2){
	Point a = l.p, b = l.q;
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	double sdr = Sqr(dx) + Sqr(dy);
	double dr = sqrt(sdr);
	double d, disc, x, y;
	a.x -= c.o.x; a.y -= c.o.y;
	b.x -= c.o.x; b.y -= c.o.y;
	d = a.x * b.y - b.x * a.y;
	disc = Sqr(c.r) * sdr - Sqr(d);
	if (disc < -EPS) return 0;
	if (disc < +EPS){
		disc = 0;
	}
	else{
		disc = sqrt(disc);
	}
	x = disc * dx * (dy > 0 ? 1 : -1);
	y = disc * fabs(dy);
	p1.x = (+d * dy + x) / sdr + c.o.x;
	p2.x = (+d * dy - x) / sdr + c.o.x;
	p1.y = (-d * dx + y) / sdr + c.o.y;
	p2.y = (-d * dx - y) / sdr + c.o.y;
	return disc > EPS ? 2 : 1;
}

int ip_circle_circle(const Circle &c1, const Circle &c2, Point &p1, Point &p2){
	double mx = c2.o.x - c1.o.x, sx = c2.o.x + c1.o.x, mx2 = Sqr(mx);
	double my = c2.o.y - c1.o.y, sy = c2.o.y + c1.o.y, my2 = Sqr(my);
	double sq = mx2 + my2, d = -(sq - Sqr(c1.r - c2.r)) * (sq - Sqr(c1.r + c2.r));
	if (!sign(sq)) return 0;
	if (d + EPS < 0) return 0;
	if (d < EPS){
		d = 0;
	}
	else{
		d = sqrt(d);
	}
	double x = mx * ((c1.r + c2.r) * (c1.r - c2.r) + mx * sx) + sx * my2;
	double y = my * ((c1.r + c2.r) * (c1.r - c2.r) + my * sy) + sy * mx2;
	double dx = mx * d, dy = my * d;
	sq *= 2;
	p1.x = (x + dy) / sq; p1.y = (y - dx) / sq;
	p2.x = (x - dy) / sq; p2.y = (y + dy) / sq;
	return d > EPS ? 2 : 1; 
}

double circle_circle_intersection_area(Circle A, Circle B){
	double d, dA, dB, tx, ty;
	d = hypot(B.o.x - A.o.x, B.o.y - A.o.y);
	if ((d < EPS) || (d + A.r <= B.r) || (d + B.r <= A.r)){
		return Sqr((B.r < A.r) ? B.r : A.r) * PI;
	}
	if (d >= A.r + B.r){
		return 0;
	}
	dA = tx = (Sqr(d) + Sqr(A.r) - Sqr(B.r)) / d / 2;
	ty = sqrt(Sqr(A.r) - Sqr(tx));
	dB = d - dA;
	return Sqr(A.r) * acos(dA / A.r) - dA * sqrt(Sqr(A.r) - Sqr(dA)) + Sqr(B.r) * acos(dB / B.r) - dB * sqrt(Sqr(B.r) - Sqr(dB));
}

/*
 * return 2 points of tangecy of c and p
 */
void circle_tangents(Circle c, Point p, Point &a, Point &b){
	double d = Sqr(c.o.x - p.x) + Sqr(c.o.y - p.y);
	double para = Sqr(c.r) / d;
	double perp = c.r * sqrt(d - Sqr(c.r)) / d;
	a.x = c.o.x + (p.x - c.o.x) * para - (p.y - c.o.y) * perp;
	a.y = c.o.y + (p.y - c.o.y) * para + (p.x - c.o.x) * perp;
	b.x = c.o.x + (p.x - c.o.x) * para + (p.y - c.o.y) * perp;
	b.y = c.o.y + (p.y - c.o.y) * para - (p.x - c.o.x) * perp;
}

/*
 * +0 : on circle;
 * +1 : inside circle;
 * -1 : outside circle;
 */
int on_circle(Circle c, Point a){
	return sign(c.r - dist(a, c.o));
}

/*
 * minimum circle that covers 2 points
 */
Circle cc2(Point a, Point b){
	return Circle(mp(a, b), dist(a, b) / 2);
}

Circle cc3(Point p, Point q, Point r){
	Circle c;
	if (on_circle(c = cc2(p, q), r) >= 0) return c;
	if (on_circle(c = cc2(p, r), q) >= 0) return c;
	if (on_circle(c = cc2(q, r), p) >= 0) return c;
	c.o = ccc(p, q, r);
	c.r = dist(c.o, p);
	return c;
}

Circle min_circle_cover(Point p[], int n){
	if (n == 1) return Circle(p[0], 0);
	if (n == 2) return cc2(p[0], p[1]);
	random_shuffle(p, p + n);
	Point *ps[4] = {&p[0], &p[1], &p[2], &p[3]};
	Circle c = cc3(*ps[0], *ps[1], *ps[2]);
	while(true){
		Point *b = p;
		for(int i = 1; i < n; i++){
			if (dist(p[i], c.o) > dist(*b, c.o)) b = &p[i];
		}
		if (on_circle(c, *b) >= 0) return c;
		ps[3] = b;
		for(int i = 0; i < 3; i++){
			swap(ps[i], ps[3]);
			if (on_circle(c = cc3(*ps[0], *ps[1], *ps[2]), *ps[3]) >= 0) break;
		}
	}
}

/*
 * new add by myf
 */

double cross(Point a, Point b){
	return a.x * b.y - a.y * b.x;
}
