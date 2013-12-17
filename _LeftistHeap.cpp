const int MAXN = 2000; //number of nodes

struct node {
	int key, npl, parent, left, right;
};

node heap[MAXN];

void init() {
	heap[0].npl = -1;
}

int merge(int a, int b) {
	if (!a) return b;
	if (!b) return a;
	if (heap[a].key > heap[b].key) swap(a, b);
	heap[a].right = merge(heap[a].right, b);
	heap[b].parent = a;
	if (heap[heap[a].right].npl > heap[heap[a].left].npl) swap(heap[a].left, heap[a].right);
	heap[a].npl = heap[heap[a].right].npl + 1;
	return a;
}

void remove(int x) {
	int q = heap[x].parent, p = merge(heap[x].left, heap[x].right);
	heap[p].parent = q;
	if (q && heap[q].left == x) heap[q].left = p;
	if (q && heap[q].right == x) heap[q].right = p;
	while (q) {
		if (heap[heap[q].left].npl < heap[heap[q].right].npl) swap(heap[q].left, heap[q].right);
		if (heap[heap[q].right].npl + 1 == heap[q].npl) break;
		heap[q].npl = heap[heap[q].right].npl + 1;
		p = q; q = heap[q].parent;
	}
}
