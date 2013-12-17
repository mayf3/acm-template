struct flow {

    struct edge { int u, v, f, c, p; } ed[M];
    int en, hd[N];
    int best[N], prev[N];

    int min_cost_max_flow(int s, int t) {
        int flow = 0, cost = 0;
        while (1) {
            memset(best, 0x3f, sizeof(best)); best[s] = 0;
            memset(prev, 0xff, sizeof(prev));

            queue<int> q; q.push(s);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int z = hd[u]; z > -1; z = ed[z].p) {
                    int v = ed[z].v, f = ed[z].f, c = ed[z].c;
                    if (f && best[v] > best[u] + c) {
                        best[v] = best[u] + c;
                        prev[v] = z;
                        q.push(v);
                    }
                }
            }
            if (prev[t] == -1) break;

            int d = inf;
            for (int v = t, e; v != s; v = ed[e].u) {
                e = prev[v];
                d = min(d, ed[e].f);
            }
            flow += d;

            for (int v = t, e; v != s; v = ed[e].u) {
                e = prev[v];
                ed[e^0].f -= d;
                ed[e^1].f += d;
                cost += d * ed[e].c;
            }
        }
        return cost;
    }

    void add_edge(int u, int v, int f, int c) {
        ed[en] = (edge){u, v, f, +c, hd[u]}; hd[u] = en++; // XXX g++ only!!
        ed[en] = (edge){v, u, 0, -c, hd[v]}; hd[v] = en++; // XXX g++ only!!
    }

    void init() {
        en = 0;
        memset(hd, -1, sizeof(hd));
    }

} F;
