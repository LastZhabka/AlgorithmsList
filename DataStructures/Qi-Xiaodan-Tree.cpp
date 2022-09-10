// Farach-Colton-Bender by Mukhammadarif Zhabka Sakhmoldin
//#pragma GCC optimize("popcnt")
#include <bits/stdc++.h>
#define mp make_pair
using namespace std;

const int N = 5e5 + 10;

int n, q;

mt19937 bruh(chrono::steady_clock::now().time_since_epoch().count());

int a[N];
int pd[N];


vector<int> vers;

struct seg {
	int t[4 * N][30];
	void build(int v = 1, int tl = 1, int tr = n) {
		for(int j = 0; j < 30; j++)t[v][j] = 0;
		if(tl == tr) {
			t[v][__lg(a[tl])] = a[tl];
			return;
		}
		int m = (tl + tr) >> 1ll;
		build(v * 2, tl, m);
		build(v * 2 + 1, m + 1, tr);
		for(int j = 29; j >= 0; j--)t[v][j] = t[v * 2][j];
  		for(int j = 29; j >= 0; j--) {
  			int z = t[v * 2 + 1][j];
  			while(z) {
				if(!t[v][__lg(z)])t[v][__lg(z)] = z;
				else z ^= t[v][__lg(z)];  				
  			}
  		}
	}
	void get(int l, int r, int v = 1, int tl = 1, int tr = n) {
		if(l > tr || tl > r)return;
		if(l <= tl && tr <= r)return (void)(vers.push_back(v));
		int m = (tl + tr) >> 1ll;
		get(l, r, v * 2, tl, m), get(l, r, v * 2 + 1, m + 1, tr);
	}
	void upd(int pos, int z, int v = 1, int tl = 1, int tr = n) {
		if(tl == tr) {
			t[v][__lg(a[tl])] = 0;
			a[tl] = pd[z];
			t[v][__lg(a[tl])] = a[tl];
			return;
		}
		int m = (tl + tr) >> 1ll;
		if(pos <= m)
			upd(pos, z, v * 2, tl, m);
		else
			upd(pos, z, v * 2 + 1, m + 1, tr);
		for(int j = 29; j >= 0; j--)t[v][j] = t[v * 2][j];
  		for(int j = 29; j >= 0; j--) {
  			int z = t[v * 2 + 1][j];
  			while(z) {
				if(!t[v][__lg(z)])t[v][__lg(z)] = z;
				else z ^= t[v][__lg(z)];  				
  			}
  		}
	}
} rt;

int pref[N];

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	cin >> n >> q;
	for(int i = 0; i <= n; i++)
		pd[i] = abs((int)bruh())/4 + 1;
	pref[0] = pd[0];
	for(int i = 1; i <= n; i++) 
		pref[i] = pref[i - 1] ^ pd[i];
	for(int i = 1; i <= n; i++)
		cin >> a[i], a[i] = pd[a[i]];
	rt.build();
	while(q--) {
		char x;
		int L, R;
		cin >> x >> L >> R;
		if(x == '?') {
			rt.get(L, R);
			int pp[30] = {0};
			for(auto v : vers) {
				for(int j = 29; j >= 0; j--) {
					int z = rt.t[v][j];
					while(z) {
						if(!pp[__lg(z)])pp[__lg(z)] = z;
						else z ^= pp[__lg(z)];
					}	
				}
			}
			vers.clear();
			int l = 0, r = n, ans = -1;
			while(l <= r) {
				int m = (l + r) >> 1ll;
				int k = pref[m];
				while(k) {
					if(!pp[__lg(k)])break;
					k ^= pp[__lg(k)];
				}
				if(!k)
					l = m + 1, ans = m;
				else
					r = m - 1;
			}
			cout << ans + 1 << '\n';
		} else {
			rt.upd(L, R);			
		}
	}
}
/*
6 4
4 1 0 2 2 3
? 1 6
? 4 6
? 2 5
? 2 6
*/