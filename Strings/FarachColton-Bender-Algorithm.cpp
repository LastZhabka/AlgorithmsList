// Farach-Colton-Bender by Mukhammadarif Zhabka Sakhmoldin
#include <bits/stdc++.h>
#define mp make_pair
using namespace std;

const int N = 1e6 + 1; // Константа N должна быть  в два раза больше размера максимального графа.
const int B = max(1, __lg(N)/2);// Размер блока
const int M = (N/B) + 1;// Количество блоков

long long n;
int bsoz = 0;
vector<int> g[N];
int tin[N], tout[N];
int gl[N], egl[N], eul[N];

// Построение эйлерового обхода>
void dfs(int v, int p) {
	tin[v] = tout[0]++;
	eul[tin[v]] = v;
	egl[tin[v]] = gl[v];
	for(auto u : g[v]) {
		if(u == p)continue;
		gl[u] = gl[v] + 1;
		dfs(u, v);						
		egl[tout[0]] = gl[v];
		eul[tout[0]++] = v;
	}	
}

int cur = 1;
pair<int, int> sp[M][__lg(M) + 1];
int mymsk[M];
int msk[1<<(B - 1)][B][B];

//Декомпозиция массива на блоки и подсчёт на блоках.

void Decompose() {	
	// bsoz - размер блока
	bsoz = __lg(tout[0])/2;
	if(bsoz == 0)
		bsoz = 1ll;
	// Делаем размер массива кратным размеру блока, тут не больше bsoz действий.
	while(tout[0] % bsoz != 0) {
		egl[tout[0]] = egl[tout[0] - 1] + 1ll;
		eul[tout[0]] = n + 1;
		tout[0]++;
	}
	egl[tout[0]] = egl[tout[0] - 1] + 1ll;
	eul[tout[0]] = n + 1;
	// Разделяем массив на блоки размером bsoz, представитель блока чел с минимальной глубиной.
	pair<int, int> mn = mp(0, 0);
	for(int i = 1; i <= tout[0]; i++) {
		if((i - 1) % bsoz == 0) { 
		    if(i != 1) sp[cur++][0] = mn;
			mn = mp(egl[i], eul[i]);
		}
		else {
			int z = (i - 2 + bsoz) % bsoz;
			if(egl[i] > egl[i - 1])
				mymsk[cur] ^= (1<<z);
		}
		mn = min(mp(egl[i], eul[i]), mn);
	}
	sp[cur][0] = mn;
	// Строим Sparse Table на новом массиве.
	for(int i = 1; i <= __lg(cur); i++) {
		for(int j = 1; j <= cur - (1<<i) + 1; j++) {
			sp[j][i] = min(sp[j][i - 1], sp[j + (1<<(i-1))][i - 1]);			
		}
	}
	// Обрабатываем каждую маску блока.
	for(int i = 0; i < (1 << (bsoz - 1)); i++) {
		int ms[bsoz] = {0};
		for(int j = 0; j < bsoz - 1; j++) {
		    ms[1 + j] = ms[j] - 1;
			if((1<<j)&i)
				ms[1 + j] = ms[j] + 1;
		}
		for(int l = 0; l < bsoz; l++) {
		    int mn = ms[l], pos = l;
			for(int r = l; r < bsoz; r++) {			
				if(ms[r] < mn)
					mn = ms[r], pos = r;
				msk[i][l][r] = pos;
			}			    
		}
	}				
}

// Sparse Table Get
pair<int,int> get(int l, int r) {	
	if(l > r) return mp(1e9, -1);
	int h = __lg(r - l + 1);
	return min(sp[l][h], sp[r - (1<<h) + 1][h]);
}

// LCA вершин a и b
int LCA(int a, int b) {
	// Получаем отрезок на котором находятся элементы.
	int p = tin[a], p2 = tin[b];
	if(p > p2)swap(p, p2);
	// Находим блоки в которых лежат вершины.
	int L = (p - 1) / bsoz + 1, R = (p2 - 1) / bsoz + 1;
	// Находим позицию внутри блоков.
	int x = p - ((L - 1) * bsoz + 1), y = p2 - ((R - 1) * bsoz + 1);
	// Если вершины в одном блоке то сразу возвращаем ответ.
	if(L == R) {	
		return eul[((L - 1) * bsoz + 1) + msk[mymsk[L]][x][y]];	
	}
	// Получаем LCA на всех блоках кроме крайних.
	pair<int,int> s = get(L + 1, R - 1);
	// Находим LCA в блоках.
	int indL = ((L - 1) * bsoz + 1) + msk[mymsk[L]][x][bsoz - 1];
	int indR = ((R - 1) * bsoz + 1) + msk[mymsk[R]][0][y];
	// Берём самый низкий.
	s = min(s, min(mp(egl[indL], eul[indL]), mp(egl[indR], eul[indR])));
	return s.second;		
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	int n, q;
	cin >> n >> q;
	tout[0] = 1ll;
	// Считываем граф
	for(int i = 1, a, b; i < n; i++) {
		cin >> b;
		a = i + 1;
		b++;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	// Подвешиваем за корень и запускаем dfs
	int root = 1;
	dfs(root, 0);
	// Начинаем декомпозицию.
	Decompose();
	for(int i = 1, a, b; i <= q; i++) {
		cin >> a >> b;
		a++, b++;
		cout << LCA(a, b) - 1 << '\n';		
	}
}