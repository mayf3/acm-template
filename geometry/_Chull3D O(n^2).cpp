typedef double Tdata;

const int MAXN = 1000 + 10;
const int MAXF = MAXN * 6;
const double EPS = 1E-6;

inline int sign(Tdata x) { return x < -EPS ? -1 : x > EPS ? 1 : 0; }

struct Tpoint {
	Tdata x, y, z;
	
	Tpoint() {}
	Tpoint(Tdata x, Tdata y, Tdata z) : x(x), y(y), z(z) {}
	void get() { scanf("%lf%lf%lf", &x, &y, &z); }
	bool operator <(Tpoint p) const {
		int s = sign(x - p.x); if (s) return s < 0;
		s = sign(y - p.y); if (s) return s < 0;
		return sign(z - p.z) < 0;
	}
	bool operator ==(Tpoint p) const { return !sign(x - p.x) && !sign(y - p.y) && !sign(z - p.z); }
	void operator -=(Tpoint p) { x -= p.x; y -= p.y; z -= p.z; }
	void operator +=(Tpoint p) { x += p.x; y += p.y; z += p.z; }
	void operator *=(Tdata c) { x *= c; y *= c; z *= c; }
	void operator /=(Tdata c) { x /= c; y /= c; z /= c; }
	Tpoint operator +(Tpoint p) const { return Tpoint(x + p.x, y + p.y, z + p.z); }
	Tpoint operator -(Tpoint p) const { return Tpoint(x - p.x, y - p.y, z - p.z); }
	Tpoint operator *(Tdata c) const { return Tpoint(x * c, y * c, z * c); }
	Tpoint operator /(Tdata c) const { return Tpoint(x / c, y / c, z / c); }
};

inline Tdata sqr(Tdata x) { return x * x; }

inline Tdata norm2(Tpoint p) { return sqr(p.x) + sqr(p.y) + sqr(p.z); }

inline Tdata norm(Tpoint p) { return sqrt(norm2(p)); }

inline Tpoint cross(Tpoint a, Tpoint b) { return Tpoint(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y); }

inline Tpoint cross(Tpoint o, Tpoint a, Tpoint b) { return cross(a - o, b - o); }

inline Tdata det(Tpoint a, Tpoint b, Tpoint c) {
	#define D2(a, b, x, y) (a.x * b.y - a.y * b.x)
	return a.x * D2(b, c, y, z) - a.y * D2(b, c, x, z) + a.z * D2(b, c, x, y);
	#undef D2
}

inline Tdata dot(Tpoint a, Tpoint b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline double volume(Tpoint p, Tpoint a, Tpoint b, Tpoint c) { return det(a - p, b - p, c - p); }

struct Chull3D {
	Tpoint P[MAXN];
	int face[MAXF][3];
	int del[MAXF];
	int lnk[MAXN][MAXN];
	bool used[MAXN];
	int N, F, face_num;
	Tdata vol, area;
	Tpoint cen;

	inline int vol_sgn(int o, int a, int b, int c) {
		Tdata v = volume(P[o], P[a], P[b], P[c]);
		return sign(v);
	}

	inline void add_face(int a, int b, int c) {
		face[F][0] = a; face[F][1] = b; face[F][2] = c; del[F] = 0;
		lnk[a][b] = lnk[b][c] = lnk[c][a] = F++;
	}

	inline bool can_see(int p, int f) { return vol_sgn(p, face[f][0], face[f][1], face[f][2]) < 0; }

	//return 0 if all in one plane or line
	bool find_tet() {
		for (int i = 1; i < N; ++i) if (P[i].x < P[0].x) swap(P[i], P[0]);
		for (int i = 2; i < N; ++i) if (P[i].x > P[1].x) swap(P[i], P[1]);
		for (int i = 3; i < N; ++i)
			if (fabs(norm2(cross(P[0], P[1], P[i]))) > fabs(norm2(cross(P[0], P[1], P[2])))) swap(P[2], P[i]);
		if (cross(P[0], P[1], P[2]) == Tpoint(0, 0, 0)) return 0;
		for (int i = 4; i < N; ++i)
			if (fabs(volume(P[0], P[1], P[2], P[i])) > fabs(volume(P[0], P[1], P[2], P[3]))) swap(P[3], P[i]);
		if (!vol_sgn(0, 1, 2, 3)) return 0;
		for (int i = 0; i < 4; ++i) {
			int a = (i + 1) % 4, b = (i + 2) % 4, c = (i + 3) % 4;
			if (vol_sgn(i, a, b, c) < 0) swap(b, c);
			add_face(a, b, c);
		}
		return 1;
	}

	void add(int p, int f) {
		if (del[f]) return;
		del[f] = 1;
		for (int i = 0; i < 3; ++i) {
			int opp = lnk[face[f][(i + 1) % 3]][face[f][i]];
			if (!del[opp]) {
				if (can_see(p, opp)) add(p, opp);
				else add_face(face[f][i], face[f][(i + 1) % 3], p);
			}
		}
	}

	bool coplanar(int f1, int f2, int p1, int p2) {
		int vs[4], m = 0;
		for (int i = 0; i < 3; ++i) {
			int v = face[f1][i];
			if (v != p1 && v != p2) vs[m++] = v;
		}
		for (int i = 0; i < 3; ++i) vs[m++] = face[f2][i];
		return vol_sgn(vs[0], vs[1], vs[2], vs[3]) == 0;
	}

	int cal_face() {
		int E = 0, V = 0;
		memset(used, 0, sizeof(used));
		for (int i = 0; i < F; ++i)
			if (!del[i])
				for (int j = 0; j < 3; ++j) {
					int k = lnk[face[i][(j + 1) % 3]][face[i][j]];
					if (!del[k] && !coplanar(i, k, face[i][j], face[i][(j + 1) % 3])) ++E, used[face[i][j]] = used[face[i][(j + 1) % 3]] = 1;
				}
		for (int i = 0; i < N; ++i) if (used[i]) ++V;
		return 2 + E / 2 - V;
	}

	double cal_volume() {
		double ret = 0;
		for (int i = 0; i < F; ++i)
			if (!del[i]) {
				Tpoint a = P[face[i][0]], b = P[face[i][1]], c = P[face[i][2]];
				ret += volume(Tpoint(0, 0, 0), a, b, c);
			}
		return fabs(ret) / 6.0;
	}

	double cal_area() {
		double ret = 0;
		for (int i = 0; i < F; ++i)
			if (!del[i]) {
				Tpoint a = P[face[i][0]], b = P[face[i][1]], c = P[face[i][2]];
				ret += fabs(norm(cross(a, b, c)) / 2.0);
			}
		return ret;
	}

	Tpoint cal_centroid() {
		Tpoint ret = Tpoint(0, 0, 0);
		for (int i = 0; i < F; ++i)
			if (!del[i]) {
				Tpoint a = P[face[i][0]], b = P[face[i][1]], c = P[face[i][2]];
				ret += (a + b + c) * volume(Tpoint(0, 0, 0), a, b, c);
			}
		return ret / cal_volume() / 24.0;
	}

	void get() {
		scanf("%d", &N);
		for (int i = 0; i < N; ++i) P[i].get();
		sort(P, P + N);
		N = unique(P, P + N) - P; F = 0;	
		vol = area = 0;
		memset(del, 0, sizeof(del));
		if (!find_tet()) return;
		random_shuffle(P + 4, P + N);
		for (int i = 4; i < N; ++i)
			for (int j = 0; j < F; ++j)
				if (!del[j] && can_see(i, j)) {
					add(i, j);
					break;
				}
		face_num = cal_face(); vol = cal_volume(); area = cal_area(); cen = cal_centroid();
	}
};
