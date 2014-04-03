//By myf
//#pragma comment(linker, "/STACK:16777216")  //C++
#include <bits/stdc++.h>

#define rep(i, n) for(int i = 0; i < (n); i++)
#define REP(i, l, r) for(int i = (l) ; i < (r); i++)
#define MP make_pair
#define PB push_back
#define Sqr(x) (x)*(x)
#define Cls(x) memset(x,0,sizeof x)
#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define sign(x) ((x < -EPS) ? -1 : x > EPS)
#define X real()
#define Y imag()


using namespace std;

typedef long long LL;
typedef complex<double> Point;
typedef Point Vec;
typedef pair<Point, Point> Line;

const int N = 1000000;
const double EPS = 1E-8;
const double PI = acos(-1.0);

struct Point3{
	double x, y, z;
	Point3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};
typedef Point3 Vec3;

Vec3 operator +(Vec3 a, Vec3 b){
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator -(Vec3 a, Vec3 b){
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator *(Vec3 a, double p){
	return Vec3(a.x * p, a.y * p, a.z * p);
}

Vec3 operator /(Vec3 a, double p){
	return Vec3(a.x / p, a.y / p, a.z / p);
}

double dot(Vec3 a, Vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 cross(Vec3 a, Vec3 b){
	return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double len(Vec3 a){
	return sqrt(dot(a, a));
}

Point3 p0[3];
Point p[3];
double r;

double cross(Point a, Point b){
	return a.X * b.Y - a.Y * b.X;
}

double cross(Point a, Point b, Point c){
	return cross(b - a, c - a);
}

double dot(Point a, Point b){
	return a.X * b.X + a.Y * b.Y;
}

double dot(Point a, Point b, Point c){
	return dot(b - a, c - a);
}

double len(Line l){
	return abs(l.second - l.first);
}

double dis(Point p, Line l){
	return fabs(cross(p, l.first, l.second) / len(l));
}

bool inter(Line a, Line b, Point &p){
	double s1 = cross(a.first, a.second, b.first);
	double s2 = cross(a.first, a.second, b.second);
	if (!sign(s1 - s2)) return false;
	p  = (s1 * b.second - s2 * b.first) / (s1 - s2);
	return true;
}

Vec dir(Line l){
	return l.second - l.first;
}

Vec normal(Vec v){
	return Vec(-v.Y, v.X);
}

Vec unit(Vec v){
	return v / abs(v);
}

bool onseg(Point p, Line l){
	return sign(cross(p, l.first, l.second)) == 0 && sign(dot(p, l.first, l.second)) <= 0;
}

double arg(Vec a, Vec b){
	double d = arg(b) - arg(a);
	if (d > PI) d -= 2 * PI;
	if (d < -PI) d += 2 * PI;
	return d;
}

double area(Point a, Point b){
	double s1 = 0.5 * cross(a, b);
	double s2 = 0.5 * arg(a, b) * r * r;
	return fabs(s1) < fabs(s2) ? s1 : s2;
}

double area(){
	double s = 0;
	rep(i, 3){
		Point O(0, 0), A = p[i], B = p[(i + 1) % 3];
		Line AB(A, B);
		double d = dis(O, AB);
		if (sign(d - r) >= 0){
			s += area(A, B);
		}
		else{
			Point P;
			inter(AB, Line(O, O + normal(dir(AB))), P);
			Vec v = sqrt(r * r - d * d) * unit(dir(AB));
			Point P1 = P - v, P2 = P + v;
			if (!onseg(P1, AB) && !onseg(P2, AB)){
				s += area(A, B);
			}
			else{
				s += area(A, P1);
				s += area(P1, P2);
				s += area(P2, B);
			}
		}
	}
	return fabs(s);
}

void init(){
	rep(i, 3){
		int x, y, z;
		scanf("%d%d%d", &x, &y, &z);
		p0[i] = Point3(x, y, z);
	}
	Vec3 n = cross(p0[1] - p0[0], p0[2] - p0[0]);
	double d = dot(p0[0], n) / len(n);
	r = sqrt(Sqr(100) - d * d);
	Point3 o = n * d / len(n);
	Vec3 ox = p0[0] - o;
	if (sign(len(ox)) == 0){
		swap(p0[0], p0[1]);
		ox = p0[0] - o;
	}
	Vec3 oy = cross(ox, n);
	ox = ox / len(ox);
	oy = oy / len(oy);
	rep(i, 3){
		p[i] = Point(dot(p0[i] - o, ox), dot(p0[i] - o, oy));
	}
}

int main(){
	init();
	printf("%.12lf\n", area());
	return 0;
}
