const int maxl = 16;

int f[maxl][maxl];

int nimMulNormal(int x, int y) {
	if (x > y) swap(x, y);
	if (x == 1) return y;
	int ret = 0;
	for (int i = 0; (1 << i) <= x; ++i)
		if ((x >> i) & 1)
			for (int j = 0; (1 << j) <= y; ++j)
				if ((y >> j) & 1) ret ^= f[i][j];
	return ret;
}

int nimMulPower(int x, int y) {
	if (x > y) swap(x, y);
	if (x == 0) return 1 << y;
	int ret = 1;
	for (int i = 0; y > 0; ++i, x >>= 1, y >>= 1) {
		if ((x & 1) + (y & 1) == 1) ret += 1 << (1 << i);
		if ((x & 1) + (y & 1) == 2) ret += nimMulNormal(ret, (1 << (1 << i)) / 2 * 3);
	}
	return ret;
}

void init() {
	for (int i = 0; i < maxl; ++i)
		for (int j = i; j < maxl; ++j)
			f[i][j] = f[j][i] = nimMulPower(i, j);
}
