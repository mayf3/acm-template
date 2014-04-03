#define sqr(x) (x * x)

struct point {
	double x, y;
};

double dist(point A, point B) {
	return sqrt(sqr(A.x - B.x) + sqr(A.y - B.y));
}

void Circumcenter(point A, point B, point C, point& cp) {
	double a1 = B.x - A.x, b1 = B.y - A.y, c1 = (sqr(a1) + sqr(b1)) / 2;
	double a2 = C.x - A.x, b2 = C.y - A.y, c2 = (sqr(a2) + sqr(b2)) / 2;
	double d = a1 * b2 - a2 * b1;
	cp.x = A.x + (c1 * b2 - c2 * b1) / d; cp.y = A.y + (a1 * c2 - a2 * c1) / d;
}

double Incenter(point A, point B, point C, point& cp) {
	double a = dist(B, C), b = dist(C, A), c = dist(A, B), p = (a + b + c) / 2, s = sqrt(p * (p - a) * (p - b) * (p - c));
	double r = s / p;
	cp.x = (a * A.x + b * B.x + c * C.x) / (a + b + c); cp.y = (a * A.y + b * B.y + c * C.y) / (a + b + c);
	return r;
}

void Orthocenter(point A, point B, point C, point& cp) {
	Circumcenter(A, B, C, cp);
	cp.x = A.x + B.x + C.x - 2 * cp.x; cp.y = A.y + B.y + C.y - 2 * cp.y;
}
