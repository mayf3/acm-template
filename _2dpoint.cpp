typedef double Tdata;

const double INF = 1E10;
const double EPS = 1E-6;

inline int sign(Tdata x) { return x < -EPS ? -1 : x > EPS; }

inline Tdata max(Tdata a, Tdata b) { return sign(a - b) > 0 ? a : b; }

inline Tdata min(Tdata a, Tdata b) { return sign(a - b) < 0 ? a : b; }

struct Tpoint {
	Tdata x, y;
	Tpoint() {}
	Tpoint(Tdata x, Tdata y) : x(x), y(y) {}
	void get() { scanf("%lf%lf", &x, &y); }
	bool operator <(Tpoint p) const { return sign(x - p.x) < 0 || sign(x - p.x) == 0 && sign(y - p.y) < 0; }
	bool operator ==(Tpoint p) const { return sign(x - p.x) == 0 && sign(y - p.y) == 0; }
	bool operator !=(Tpoint p) const { return sign(x - p.x) || sign(y - p.y); }
	void operator -=(Tpoint p) { x -= p.x; y -= p.y; }
	void operator +=(Tpoint p) { x += p.x; y += p.y; }
	void operator *=(Tdata c) { x *= c; y *= c; }
	void operator /=(Tdata c) { x /= c; y /= c; }
	Tpoint operator +(Tpoint p) const { return Tpoint(x + p.x, y + p.y); }
	Tpoint operator -(Tpoint p) const { return Tpoint(x - p.x, y - p.y); }
	Tpoint operator *(Tdata c) const { return Tpoint(x * c, y * c); }
	Tpoint operator /(Tdata c) const { return Tpoint(x / c, y / c); }
};

inline Tdata sqr(Tdata x) { return x * x; }
inline Tdata norm2(Tpoint p) { return sqr(p.x) + sqr(p.y);}
inline Tdata norm(Tpoint p) { return sqrt(norm2(p));}
inline Tdata cross(Tpoint a, Tpoint b) { return a.x * b.y - b.x * a.y; }
inline Tdata cross(Tpoint o, Tpoint a, Tpoint b) { return cross(a - o, b - o); }
inline Tdata dot(Tpoint a, Tpoint b) { return a.x * b.x + a.y * b.y; }
inline Tdata dot(Tpoint o, Tpoint a, Tpoint b) { return dot(a - o, b - o); }
inline Tdata dist2(Tpoint a, Tpoint b) { return norm2(a - b); }
inline Tdata dist(Tpoint a, Tpoint b) { return norm(a - b); }

struct Tline {
	Tdata a, b, c;
	double ang;
	Tline() {}
	Tline(Tdata a, Tdata b, Tdata c) : a(a), b(b), c(c) { ang = atan2(b, -a); }
	void get() { scanf("%lf%lf%lf", &a, &b, &c); }
	bool operator <(Tline l) const { return sign(ang - l.ang) < 0 || sign(ang - l.ang) == 0 && sign(c - l.c) < 0; }
};

inline int side(Tline l, Tpoint p) { return sign(l.a * p.x + l.b * p.y + l.c); }

// change line from two point form to general form
// O(1)
// return line(general form)
inline Tline change_line(Tpoint a, Tpoint b) {
	Tdata tmp, A = a.y - b.y, B = b.x - a.x, C = cross(a, b);
	if (sign(A)) tmp = fabs(A);
	else tmp = fabs(B);
	return Tline(A / tmp, B / tmp, C / tmp);
}

// get the diameter of convex polygon
// O(N)
// p1, p2 are the points forming diameter
Tdata diam_convex_poly(Tpoint *P, int N, Tpoint &p1, Tpoint &p2) {
	if (N == 1) {
		p1 = p2 = P[0];
		return 0;
	}
	double ret = -INF;
	for (int j = 1, i = 0; i < N; ++i) {
		while (sign(cross(P[i], P[i + 1], P[j + 1]) - cross(P[i], P[i + 1], P[j])) > 0) j = (j + 1) % N;
		ret = max(ret, max(dist2(P[i], P[j]), dist2(P[i + 1], P[j + 1])));
	}
	return ret;
}

// intersection of two lines(general form)
// O(1)
// l1, l2 must have only one point of intersection
inline Tpoint inter_point(Tline l1, Tline l2) {
	Tdata d = l1.a * l2.b - l2.a * l1.b;
	return Tpoint((l1.b * l2.c - l2.b * l1.c) / d, -(l1.a * l2.c - l2.a * l1.c) / d);
}

// intersection of two lines(general form)
// O(1)
// 0: parallel, 1: intersect, 2: overlap
// p - point of intersection
int inter_line(Tline l1, Tline l2, Tpoint &p) {
	Tdata d = l1.a * l2.b - l2.a * l1.b;
	if (sign(d) == 0)
		if (sign(l1.a)) return sign(l1.a * l2.c - l2.a * l1.c) ? 0 : 2;
		else return sign(l1.b * l2.c - l2.b * l1.c) ? 0 : 2;
	p = Tpoint((l1.b * l2.c - l2.b * l1.c) / d, -(l1.a * l2.c - l2.a * l1.c) / d);
	return 1;
}

// intersection of two lines(two point form)
// O(1)
// 0: parallel, 1: intersect, 2: overlap
// p - point of intersection
int inter_line(Tpoint a, Tpoint b, Tpoint c, Tpoint d, Tpoint& p) {
	Tline l1 = change_line(a, b), l2 = change_line(c, d);
	return inter_line(l1, l2, p);
}

// intersection of line(two point form) and segment
// O(1)
// 0: no intersection, 1: proper intersection, 2: intersect at endpoint, 3: partial intersection
// p - point of intersection
int inter_line2seg(Tpoint a, Tpoint b, Tpoint c, Tpoint d, Tpoint& p) {
	double s1 = cross(a, b, c), s2 = cross(a, b, d);
	int d1 = sign(s1), d2 = sign(s2);
	if (d1 * d2 < 0) {
		p = Tpoint((c.x * s2 - d.x * s1) / (s2 - s1), (c.y * s2 - d.y * s1) / (s2 - s1));
		return 1;
	}
	if (d1 * d2 > 0) return 0;
	if (d1 == 0 && d2 == 0) return 3;
	p = d1 == 0 ? c : d;
	return 2;
}

// intersection of line(general form) and segment
// O(1)
// 0: no intersection, 1: proper intersection, 2: intersect at endpoint, 3: partial intersection
// p - point of intersection
int inter_line2seg(Tline l, Tpoint a, Tpoint b, Tpoint& p) {
	int d1 = side(l, a), d2 = side(l, b);
	if (d1 * d2 < 0) {
		double s1 = l.a * a.x + l.b * a.y + l.c, s2 = l.a * b.x + l.b * b.y + l.c;
		p = Tpoint((a.x * s2 - b.x * s1) / (s2 - s1), (a.y * s2 - b.y * s1) / (s2 - s1));
		return 1;
	}
	if (d1 * d2 > 0) return 0;
	if (d1 == 0 && d2 == 0) return 3;
	p = d1 == 0 ? a : b;
	return 2;
}

// intersection of two segments
// O(1)
// 0: no intersection, 1: proper intersection, 2: intersect at endpoint, 3: partial intersection
// p - point of intersection
int inter_seg(Tpoint a, Tpoint b, Tpoint c, Tpoint d, Tpoint& p) {
	double s1 = cross(a, b, c), s2 = cross(a, b, d), s3 = cross(c, d, a), s4 = cross(c, d, b);
	int d1 = sign(s1), d2 = sign(s2), d3 = sign(s3), d4 = sign(s4);
	if (d1 * d2 < 0 && d3 * d4 < 0) {
		p = Tpoint((c.x * s2 - d.x * s1) / (s2 - s1), (c.y * s2 - d.y * s1) / (s2 - s1));
		return 1;
	}
	if (sign(min(a.x, b.x) - max(c.x, d.x)) > 0 || sign(min(c.x, d.x) - max(a.x, b.x)) > 0 ||
		sign(min(a.y, b.y) - max(c.y, d.y)) > 0 || sign(min(c.y, d.y) - max(a.y, b.y)) > 0 || d1 * d2 > 0 || d3 * d4 > 0) return 0;
	if (d1 == 0 && d2 == 0) return 3;
	p = d1 == 0 ? c : d2 == 0 ? d : d3 == 0 ? a : b;
	return 2;
}

// intersection of half-planes
// O(N log N)
// ax + by + c >= 0
// P - points form the intersection, M - number of points
void inter_hplane(Tline *H, int N, Tpoint *P, int &M) {
	int *queue = new int[N + 1];
	sort(H, H + N);
	M = 0;
	for (int i = 0; i < N; ++i)	if (!i || sign(H[i].ang - H[queue[M - 1]].ang)) queue[M++] = i;
	int h = 0, t = 2;
	for (int i = 2; i < M; ++i) {
		while (h + 1 < t && side(H[queue[i]], inter_point(H[queue[t - 1]], H[queue[t - 2]])) < 0) --t;
		while (h + 1 < t && side(H[queue[i]], inter_point(H[queue[h]], H[queue[h + 1]])) < 0) ++h;
		queue[t++] = queue[i];
	}
	while (h + 1 < t && side(H[queue[h]], inter_point(H[queue[t - 1]], H[queue[t - 2]])) < 0) --t;
	while (h + 1 < t && side(H[queue[t - 1]], inter_point(H[queue[h]], H[queue[h + 1]])) < 0) ++h;
	M = 0;
	for (int i = h; i < t; ++i) queue[M++] = queue[i];
	queue[M] = queue[0];
	for (int i = 0; i < M; ++i) P[i] = inter_point(H[queue[i]], H[queue[i + 1]]);
	delete [] queue;
}

// convex hull
// O(N log N)
// stack - points in convex hull, top - number of points
void convex_hull(Tpoint *P, int N, Tpoint *stack, int &top) {
	sort(P, P + N);
	stack[0] = P[0]; stack[1] = P[1]; top = 2;
	for (int i = 2; i < N; ++i) {
		while (top > 1 && sign(cross(stack[top - 2], stack[top - 1], P[i])) <= 0) --top;
		stack[top++] = P[i];
	}
	for (int k = top, i = N - 2; i >= 0; --i) {
		while (top > k && sign(cross(stack[top - 2], stack[top - 1], P[i])) <= 0) --top;
		stack[top++] = P[i];
	}
	--top;
}

// calculate the area of polygon
// O(N)
// be careful the sign of the area
Tdata cal_area(Tpoint *P, int N) {
	if (N < 3) return 0;
	Tdata ret = 0;
	P[N] = P[0];
	for (int i = 0; i < N; ++i) ret += cross(P[i], P[i + 1]);
	return ret / 2.0;
}

// judge whether point in simple polygon
// O(N)
// -1: outside, 0: border, 1: inside
// P1[i]-P2[i]: edge of simple polygon
int point_in_simple_poly(Tpoint a, Tpoint *P1, Tpoint *P2, int N) {
	int cnt = 0;
	for (int i = 0; i < N; ++i) {
		Tpoint u = P1[i], v = P2[i];
		if (sign(u.x - v.x) > 0) swap(u, v);
		int d = sign(cross(a, u, v));
		if (d > 0) {
			if (sign(u.x - a.x) <= 0 && sign(a.x - v.x) < 0) ++cnt;
		}
		else if (d == 0 && sign((a.x - u.x) * (a.x - v.x)) <= 0 && sign((a.y - u.y) * (a.y - v.y)) <= 0) return 2;
	}
	if (cnt & 1) return 1;
	else return 0;
}

// judge whether point in convex polygon
// O(N)
// 0: outside, 1: inside, 2: border
int point_in_convex_poly(Tpoint a, Tpoint *P, int N) {
	int ret = 1;
	P[N] = P[0];
	for (int i = 0; i < N; ++i) {
		int d = sign(cross(a, P[i], P[i + 1]));
		if (d < 0) return 0;
		else if (d == 0) ret = 2;
	}
	return ret;
}

// get the core of polygon
// O(N log N)
Tpoint core_of_poly(Tpoint *P, int N) {
	Tline *H = new Tline[N];
	Tpoint *A = new Tpoint[N];
	int M;
	P[N] = P[0];
	for (int i = 0; i < N; ++i) H[i] = change_line(P[i], P[i + 1]);
	inter_hplane(H, N, A, M);
	Tpoint ret = A[0];
	delete [] H; delete [] A;
	return ret;

}

// judge whether point in star polygon or convex polygon
// O(log N)
// 0: outside, 1: inside, 2: border
int point_in_poly(Tpoint a, Tpoint *P, int N, Tpoint o) {
	int l = 0, r = N - 1;
	while (l <= r) {
		int mid = (l + r) / 2;
		if (sign(cross(o, a, P[mid])) > 0) r = mid - 1;
		else l = mid + 1;
	}
	r = (r + N) % N;
	if (sign(cross(o, a, P[r])) == 0) return sign(norm(P[r]) - norm(a));
	else return sign(cross(a, P[r], P[r + 1]));
}

// judge whether points in star polygon or convex polygon
// O(M log N)
int points_in_poly(Tpoint *A, int M, Tpoint *P, int N) {
	Tpoint *B = new Tpoint[N + 1];
	Tpoint o = core_of_poly(P, N);
	int k = 0;
	for (int i = 1; i < N; ++i)
		if (atan2(P[k].y - o.y, P[k].x - o.x) > atan2(P[i].y - o.y, P[i].x - o.x)) k = i;
	for (int i = 0; i < N; ++i, k = (k + 1) % N) B[i] = P[k];
	B[N] = B[0];
	int ret = 0;
	for (int i = 0; i < M; ++i)
		if (point_in_poly(A[i], B, N, o) != -1) ++ret; // including on bounder
	return ret;
}

// get the length of segment in convex polygon
// O(N)
Tdata seg_in_convex_poly(Tpoint a, Tpoint b, Tpoint *P, int N) {
	int d1 = point_in_convex_poly(a, P, N), d2 = point_in_convex_poly(b, P, N);
	if (d2 == 1) swap(d1, d2), swap(a, b);
	if (d2 == 1) return dist(a, b);
	Tpoint p;
	P[N] = P[0];
	if (d1 == 1)
		for (int i = 0; i < N; ++i) {
			int d = inter_seg(a, b, P[i], P[i + 1], p);
			if (d == 1 || d == 2) return dist(a, p); // not including the boundaries, add "d == 3" for including the boundaries
		}
	else {
		int cnt = 0;
		Tpoint u, v;
		for (int i = 0; i < N; ++i) {
			int d = inter_seg(a, b, P[i], P[i + 1], p);
			if (d == 3) return 0; // on the boundaries
			if (cnt == 2) continue;
			if (d)
				if (!cnt) u = p, ++cnt;
				else if (u != p) v = p, ++cnt;
		}
		return cnt == 2 ? dist(u, v) : 0;
	}
}

// get the centroid of polygon
// O(N)
Tpoint cal_centroid(Tpoint *P, int N) {
	P[N] = P[0];
	Tpoint c(0, 0);
	Tdata s = 0;
	for (int i = 0; i < N; ++i) {
		Tdata tmp = cross(P[i], P[i + 1]);
		c += (P[i] + P[i + 1]) * tmp; s += tmp;
	}
	return c / (3 * s);
}

// get the distance of point, line(a, b)
// O(1)
Tdata dist_point2line(Tpoint p, Tpoint a, Tpoint b) {
	return abs(cross(a - p, b - p)) / norm(a - b);
}

// get the distance of point, line
// O(1)
Tdata dist_point2line(Tpoint p, Tline l) {
	return abs(l.a * p.x + l.b * p.y + l.c) / sqrt(sqr(l.a) + sqr(l.b));
}

// get the distance of point, segment
// O(1)
Tdata dist_point2seg(Tpoint p, Tpoint a, Tpoint b) {
	Tdata h = abs(cross(p, a, b)) / dist(a, b);
	if (sign(dot(b - a, p - a)) > 0 && sign(dot(a - b, p - b)) > 0) return h;
	return min(dist(p, a), dist(p, b));
}
