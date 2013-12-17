// a + b = c
void plus(const int a[], const int b[], int c[]) {
	c[0] = max(a[0], b[0]);
	for (int i = 1; i <= c[0]; ++i) {
		c[i] += a[i] + b[i];
		c[i + 1] += c[i] / 10;
		c[i] %= 10;
	}
	if (c[c[0] + 1]) ++c[0];
}

// a - b = c
void minus(const int a[], const int b[], int c[]) {
	for (int i = 1; i <= a[0]; ++i) {
		c[i] += a[i] - b[i];
		if (c[i] < 0) c[i] += 10, --c[i + 1];
	}
	for (c[0] = a[0]; c[0] > 1 && !c[c[0]]; --c[0]);
}

// a * b = c
void multi(const int a[], const int b[], int c[]) {
	for (int i = 1; i <= a[0]; ++i)
		for (int j = 1; j <= b[0]; ++j) {
			c[i + j - 1] += a[i] * b[j];
			c[i + j] += c[i + j - 1] / 10;
			c[i + j - 1] %= 10;
		}
	for (c[0] = a[0] + b[0]; c[0] > 1 && !c[c[0]]; --c[0]);
}

// a / b = c ...... d
void divide1(const int a[], const int b, int c[], int& d) {
	d = 0;
	for (int i = a[0]; i > 0; --i) {
		d = d * 10 + a[i];
		c[i] = d / b;
		d %= b;
	}
	for (c[0] = a[0]; c[0] > 1 && !c[c[0]]; --c[0]);
}

int cmp(const int x[], const int y[]) {
	if (x[0] > y[0]) return 1;
	if (x[0] < y[0]) return -1;

	int i = x[0];
	while (i > 0 && x[i] == y[i]) --i;
	return x[i] - y[i];
}

void divide2(const int a[], const int b[], int c[], int d[]) {
	d[0] = 1; d[1] = 0;
	for (int i = a[0]; i > 0; --i) {
		if (!(d[0] == 1 && d[1] == 0)) {
			for (int j = d[0]; j > 0; --j) d[j + 1] = d[j];
			++d[0];
		}
		d[1] = a[i]; c[i] = 0;
		int j;
		while ((int j = cmp(d, b)) >= 0) {
			minus(d, b, d); ++c[i];
			if (j == 0) break;
		}
	}
	for (c[0] = a[0]; c[0] > 1 && !c[c[0]]; --c[0]);
}
