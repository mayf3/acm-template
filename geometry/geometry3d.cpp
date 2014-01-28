#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#define Sqr(x) (x) * (x)

using namespace std;

const double EPS = 1E-8;

inline int sign(double x){
	return x < -EPS ? -1 : x > EPS;
}

inline double frand(){
	return rand() / (RAND_MAX + 1.0);
}

/*
 * ------points & vectors-----
 */
struct Point3{
	double x, y, z;
	Point3(){}
	Point3(double x, double y, double z) : x(x), y(y), z(z) {}
	void in(){
		scanf("%lf%lf%lf", &x, &y, &z);
	}
	void print(){
		printf("%lf %lf %lf", x, y, z);
	}
};
typedef Point3 Vector3;

inline Vector3 operator+(Point3 a, Point3 b){
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator-(Point3 a, Point3 b){
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator*(double t, Vector3 a){
	return Vector3(a.x * t, a.y * t, a.z * t);
}

inline Vector3 operator*(Vector3 a, double t){
	return Vector3(a.x * t, a.y * t, a.z * t);
}

inline Vector3 operator/(Vector3 a, double t){
	return Vector3(a.x / t, a.y / t, a.z / t);
}

inline Vector3 operator*(Vector3 a, Vector3 b){
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline double operator^(Vector3 a, Vector3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double len(Vector3 a){
	return sqrt(a ^ a);
}

inline double len2(Vector3 a){
	return a ^ a;
}

inline int zero(Vector3 a){
	return !sign(a.x) && !sign(a.y) && !sign(a.z);
}

/*
 * ----------lines, line segment & planes----------
 */
struct Line{
	Point3 p, q;
	Line(){}
	Line(Point3 p, Point3 q) : p(p), q(q) {}
	double len2(){
		return (q - p) ^ (q - p);
	}
	double len(){
		return sqrt((q - p) ^ (q - p));
	}
};

/*
 * returns a vector that perps to u
 */
Vector3 perp_vector(Vector3 u){
	Vector3 v, n;
	while(true){
		v.x = frand();
		v.y = frand();
		v.z = frand();
		if (!zero(n = u * v)) return v;
	}
}

/*
 * check if point a inside line l
 */
int on_seg(Line l, Point3 a){
	return zero((a - l.p) * (a - l.q)) && sign((l.p - a) ^ (l.q - a)) <= 0;
}

/*
 * relation of a & b base on l
 * same side : +1;
 * opposite side : -1;
 * otherwise : 0;
 */
inline int side(Line l, Point3 a, Point3 b){
	return sign(((l.p - l.q) * (a - l.p)) ^ ((l.p - l.q) * (b - l.p)));
}

/*
 * intersetion point of plane(norm, A) and lineseg l 
 * ret is the result
 */
int ip_plane_seg(Vector3 norm, Point3 a, Line l, Point3 &ret){
	double lhs = norm ^ (l.q - l.p);
	double rhs = norm ^ (a - l.p);
	double t = rhs / lhs;
	if (sign(t) >= 0 && sign(t- 1) <= 0){
		ret = l.p + t * (l.q - l.p);
		return 1;
	}
	return 0;
}

/*
 * check if 2 linesegs l1 & l2 touched with each other
 */
int touched_segs(Line l1, Line l2){
	if (zero((l1.q - l1.p) * (l2.q - l2.p))){
		return on_seg(l1, l2.p) || on_seg(l1, l2.q) || on_seg(l2, l1.p) || on_seg(l2, l1.q);
	}
	else{
		return side(l1, l2.p, l2.q) <= 0 && side(l2, l1.p, l1.q) <= 0;
	}
}

/*
 * return the projection of point a to line l
 */
Point3 project(Line l, Point3 a){
	double t = ((l.q - l.p) ^ (a - l.p)) / ((l.q - l.p) ^ (l.q - l.p));
	return l.p + t * (l.q - l.p);
}

/*
 * return the closest point in line l
 */
Point3 closest_point_seg(Line l, Point3 a){
	double t = ((l.q - l.p) ^ (a - l.p)) / ((l.q - l.p) ^ (l.q - l.p));
	return l.p + max(0.0, min(t, 1.0)) * (l.q - l.p);
}


/*
 * ----------plane----------
 */
struct Plane{
	Point3 a;
	Vector3 n;
};

/*
 * check if the point in the plane
 */
int on_plane(Plane pl, Point3 p){
	return !sign(pl.n ^ (p - pl.a));
}

/*
 * return the distance between point and the plane
 */
double dist_plane_point(Plane pl, Point3 a){
	return fabs(pl.n ^ (a - pl.a)) / len(pl.n);
}

/*
 * closest point in the plane
 */
Point3 closest_point_plane(Plane pl, Point3 a){
	return a + ((pl.n ^ (pl.a - a)) / (pl.n ^ pl.n)) * pl.n;
}

/*
 * mappint from 3D point to 2D point
 */
Point3 to_plane(Point3 a, Point3 b, Point3 c, Point3 p){
	Vector3 norm, ydir, xdir;
	Point3 res;
	norm = (b - a) * (c - a);
	xdir = b - a;
	xdir = xdir / len(xdir);
	ydir = norm * xdir;
	ydir = ydir / len(ydir);
	res.x = (p - a) ^ xdir;
	res.y = (p - a) ^ ydir;
	res.z = 0;
	return res;
}

/*
 * given two lines in 3D space , find distance of closest approach
 */
double dist_line_line(Line l1, Line l2){
	Vector3 v = (l1.q - l1.p) * (l2.q - l2.p);
	if (zero(v)){
		if (zero((l1.q - l1.p) * (l2.p - l1.p))) return 0;
		return len((l2.p - l1.p) * (l2.q - l1.p)) / len(l2.p - l2.q);
	}
	return fabs((l1.p - l2.p) ^ v) / len(v);
}

/*
 * this is the same as dist_line_line, but it also return s the points of closest approach
 */
double closest_approach(Line l1, Line l2, Point3 &p, Point3 &q){
	double s = (l2.q - l2.p) ^ (l1.q - l1.p);
	double t = (l1.p - l2.p) ^ (l2.q - l2.p);
	double num, den, tmp;
	den = l1.len2() * l2.len2() - s * s;
	num = t * s - l2.len2() * ((l1.p - l2.p) ^ (l1.q - l1.p));
	if (!sign(den)){
		p = l1.p;
		q = l2.p + (l2.q - l2.p) * t / l2.len();
		if (!sign(s)) q = l1.p;
	}
	else{
		tmp = num / den;
		p = l1.p + (l1.q - l1.p) * tmp;
		q = l2.p + (l2.q - l2.p) * (t + s * tmp) / l2.len2();
	}
	return len(p - q);
}

/*
 * --------balls(spheres)----------
 */
struct Ball{
	Point3 o;
	double r;
	Ball(Point3 o = Point3(0, 0, 0), double r = 1) : o(o), r(r) {}
};

/*
 * ip between ball o and line l
 */
int ip_ball_line(Ball o, Line l, Point3 &p, Point3 &q){
	Vector3 v;
	Point3 d = project(l, o.o);
	if (len2(o.o - d) > o.r * o.r) return 0;
	v = sqrt((o.r * o.r - len2(o.o - d)) / l.len2()) * (l.p - l.q);
	p = d + v;
	q = d - v;
	return 1;
}

/*
 * Given the latitude and longitude of two points in degrees
 * calculates the distance over the sphere between them.
 * Latitude is given in the range [-PI/2,PI/2] degrees,
 * Longitude is given in the range [-PI,PI] degrees.
 */
double greatcircle(double lat1, double long1, double lat2, double long2){
	return acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long2 - long1));
}

/*
 * Solves the determinant of a n*n matrix recursively
 */
double det(double m[4][4], int n){
	double s[4][4], res = 0, x;
	int i, j, skip, ssize;
	if (n == 2){
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}
	for(skip = 0; skip < n; skip++){
		for(i = 1; i < n; i++){
			for(j = 0, ssize = 0; j < n; j++){
				if (j == skip) continue;
				s[i - 1][ssize++] = m[i][j];
			}
		}
		x = det(s, n - 1);
		if (skip % 2){
			res -= m[0][skip] * x;
		}
		else{
			res += m[0][skip] * x;
		}
	}
	return res;
}

/*
 * Given 4 points:
 * Returns 0 if the points are coplanar
 * Returns 1 if the points are not coplanar with:
 * 	o = center of sphere
 * 	r = radius of sphere
 */
int make_sphere(Point3 p[4], Ball o){
	double m[4][5], s[4][4], sol[5];
	int ssize, skip, i, j;
	for(i = 0; i < 4; i++){
		s[i][0] = p[i].x;
		s[i][1] = p[i].y;
		s[i][2] = p[i].z;
		s[i][3] = 1;
	}
	if (!sign(det(s, 4))) return 0;
	for(i = 0; i < 4; i++){
		m[i][0] = 0;
		m[i][0] += Sqr(m[i][1] = p[i].x);
		m[i][0] += Sqr(m[i][2] = p[i].y);
		m[i][0] += Sqr(m[i][3] = p[i].z);
		m[i][4] = 1;
	}
	for(skip = 0; skip < 5; skip++){
		for(i = 0; i < 4; i++){
			for(j = 0, ssize = 0; j < 5; j++){
				if (j == skip) continue;
				s[i][ssize++] = m[i][j];
			}
		}
		sol[skip] = det(s, 4);
	}
	for(i = 1; i < 5; i++){
		sol[i] /= (sol[0] * ((i % 2) ? 1 : -1));
	}
	for(i = 1; i < 4; i++){
		sol[4] += Sqr(sol[i] /= 2);
	}
	o.o.x = sol[1];
	o.o.y = sol[2];
	o.o.z = sol[3];
	o.r = sqrt(sol[4]);
	return 1;
}

/*
 * ----------polygons---------- 
 */

/*
 * check if point A inside polygon p[n]
 */
int inside_polygon(Point3 *p, int n, Vector3 norm, Point3 A){
	if (sign(norm ^ (A - p[0]))) return 0;
	p[n] = p[0];
	for(int i = 0; i < n; i++){
		if (on_seg(Line(p[i], p[i + 1]), A)) return 1;
	}
	double sum = 0;
	for(int i = 0; i < n; i++){
		Vector3 a = p[i] - A;
		Vector3 b = p[i + 1] - A;
		sum += sign(norm ^ (a * b)) * acos((a ^ b) / (len(a) * len(b)));
	}
	return sign(sum);
}

/*
 * check if lineseg l touches polygon p[n] with normal vector norm
 */
int intersected_polygon_seg(Point3 *p, int n, Vector3 norm, Line l){
	p[n] = p[0];
	if (!sign((l.p - l.q) ^ norm)){
		if (sign(norm ^ (l.p - p[0]))) return 0;
		if (inside_polygon(p, n, norm, l.p) || inside_polygon(p, n, norm, l.q)) return 1;
		for(int i = 0; i < n; i++){
			if (touched_segs(l, Line(p[i], p[i + 1]))) return 1;
		}
		return 0;
	}
	Point3 ret;
	if (ip_plane_seg(norm, p[0], l, ret)){
		return inside_polygon(p, n, norm, ret);
	}
	return 0;
}

/*
 * normal vector of polygon p[n]
 */
Vector3 normal(Point3 *p, int n){
	Vector3 b, norm;
	p[n] = p[0];
	p[n + 1] = p[1];
	for(int i = 0; i < n; i++){
		norm = (p[i + 1] - p[i + 2]) * (p[i] - p[i + 1]);
		if (!zero(norm)) return norm;
	}
	return perp_vector(p[0] - p[1]);
}

/*
 * check if 2 polygons p[n] & q[m] touched with each other
 */
int touched_polygons(Point3 *p, int n, Point3 *q, int m){
	Vector3 norm;
	norm = normal(q, m);
	p[n] = p[0];
	for(int i = 0; i < n; i++){
		if (intersected_polygon_seg(q, m, norm, Line(p[i], p[i + 1]))) return 1;
	}
	norm = normal(p, n);
	q[m] = q[0];
	for(int i = 0; i < m; i++){
		if (intersected_polygon_seg(p, n, norm, Line(q[i], q[i + 1]))) return 1;
	}
	return 0;
}

/*
 * new add by myf
 */

struct Plane3{
	Point3 a, b, c;
	Plane3(){}
	Plane3(Point3 a, Point3 b, Point3 c) : a(a), b(b), c(c) {}
};

double triple(Point3 a, Point3 b, Point3 c){
	return a ^ (b * c);;
}

double polygon_volume(Plane3 *p, int n){
	double volume = 0.0;
	for(int i = 0; i < n; i++){
		volume += triple(p[i].a, p[i].b, p[i].c);
	}
	return fabs(volume) / 6.0;
}
