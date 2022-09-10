#include <bits/stdc++.h>
using namespace std;

const int N = 2e5;

int n;

struct SegmentTree {
	int t[4 * N];
	void Update(int p, int z, int v = 1, int tl = 1, int tr = n) {
		if(tl == tr) {
			t[v] = z;
			return;
		}
		int m = (tl + tr) >> 1;
		if(p <= m)
			Update(p, z, v << 1, tl, m);
		else
			Update(p, z, v << 1 ^ 1, m + 1, tr);
		t[v] = t[v] + t[v * 2];
	}
} rt;

vector<int> g[N];
int tin[N], tout[N], sz[N], gl[N], HLDAnc[N];

void dfs(int v, int p) {
	sz[v] = 1;
	for(auto u : g[v])
		if(u != p) {
			dfs(u, v);
			sz[v] = sz[u] + sz[v];
		}		
}
void build(int v, int p, int hlp) {
    tin[v] = tout[0]++;
	gl[v] = gl[p] + 1;
	HLDAnc[v] = hlp;
	for(auto u : g[v])
		if(u != p && sz[u] * 2 > sz[v])
			build(u, v, hlp);
	for(auto u : g[v])
		if(u != p)
			build(u, v, u);				
	tout[v] = tout[0]++;
}

void lift(int a, int b) {
	while(a != b) {
		if(gl[a] < gl[b])swap(a, b);
	}
}

int main() {
	tout[0] = 1;
}