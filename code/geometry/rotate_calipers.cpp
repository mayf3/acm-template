double fix(double a, double b = 0) {
  a -= b;
  if (sign(a) < 0) a += 2 * pi;
  if (sign(a - 2 * pi) >= 0) a -= 2 * pi;
  return a;
}

double angle(Point a, Point b){
	return fix(arg(b - a));
}

double shadow_length(double alpha, Point a, Point b){
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	double c = cos(alpha);
	double s = sin(alpha);
	return fabs(dx * c + dy * s);
}

void rotate_calipers(Point ps[], int n, double &area, double &peri){
	area = peri = INF;
	n = find_convex(ps, n);
	ps[n] = ps[0];
	Point *q[4] = {NULL, NULL, NULL, NULL};
	for(int i = 0; i < n; i++){
		Point *p = &ps[i];
		if (!q[0] || q[0]->Y > p->Y || q[0]->Y == p->Y && q[0]->X > p->X) q[0] = p;
		if (!q[1] || q[1]->X < p->X || q[1]->X == p->X && q[1]->Y > p->Y) q[1] = p;
		if (!q[2] || q[2]->Y < p->Y || q[2]->Y == p->Y && q[2]->X < p->X) q[2] = p;
		if (!q[3] || q[3]->X > p->X || q[3]->X == p->X && q[3]->Y < p->Y) q[3] = p;
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
