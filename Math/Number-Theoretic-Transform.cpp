#include <bits/stdc++.h>
using namespace std;

// NTT
#define ll long long
const ll M = (1<<20) + 20;
const ll MOD = 998244353;

int mult(int a, int b) {
	return (1LL * a * b) % MOD;
}
int sum(int a, int b) {
	if(a + b >= MOD)
		return a + b - MOD;
	return a + b;
}
int dif(int a, int b) {
	if(a - b < 0)
		return a - b + MOD;
	return a - b;
}

int bp(int a, int b) {
	int c = 1;
	while(b) {
		if(b&1)
			c = mult(c, a);
		b >>= 1;
		a = mult(a, a);
	}
	return c;
}
int inv(int a) {
	return bp(a, MOD - 2);
}


vector<int> dp(M, 0);
vector<int> pd, pd2;
int w[M];

void init() {
	int n = 20;
	w[0] = 1;
	w[1] = 646;
	// Подсчитываем омеги для модуля, чтобы их найти можете воспользоваться прекалком.
	/*
		w[1] = 2;
		for(;;w[1]++) {
			int z = w[1];
			for(int j = 1; j < n; j++)
				z = mult(z, z);
			if(z == MOD - 1)break;
		}
	*/
	for(int j = 2; j < (1<<n); j++)
		w[j] = mult(w[j - 1], w[1]);	
}

struct polycock {
	vector<int> coef;

	void NTT(ll n) {
		// Подстраиваем размер массива для подсчёта.
		pd.resize(1<<n), pd2.resize(1<<n);
		 
		for(int j = 0; j < (1<<n); j++) {
			// dp[j] - бит ревёрс 
			dp[j] = (dp[j>>1]>>1)^((j&1)? (1<<(n - 1)) : 0);
		    //Удалим последний бит и возьмём его реверс, потом сдвинем его и узнаем какое число стоит в нулевом бите
			pd[j] = (coef.size() > dp[j] ? coef[dp[j]] : 0); 
			// Строим базу для многочленов размера 1
    	}
		
		// Перебираемр размер отрезка
		int x;
		for(int j = 1; j <= n; j++) {	
			// Перебираем блок.
			int p = (1<<(j - 1));
			for(int i = 0; i < (1<<n); i += 2 * p) {
				// Считаем ответ для каждой точки для текущего многочлена
				for(int k = 0; k < p; k++) {
					int L = i + k, R = i + k + p;
					// нам нужен i-ый корень степени j
					// но у нас есть только корень степени n
					// чтобы получить i-ый корень степени j мы должны умножить на отношение 2^n/2^j = 2^(n-j)
					// (2 * pi * k)/(2^n), 2 * pi * (i * (2^(n-j)) / (2^n) = (2 * pi * i) / (2^j)
			        pd2[i + k] = sum(pd[L], mult(pd[R], w[k<<(20 - j)]));
			        pd2[i + p + k] = dif(pd[L], mult(pd[R], w[k<<(20 - j)]));
				}
			}
			pd.swap(pd2);															    
		}
		coef = pd;
	}
};

polycock operator *(polycock &a, polycock &b) {
	ll z = a.coef.size() + b.coef.size();
	ll pp = z;
	ll st = 0;
	while(z > 0)z >>= 1ll, st++;
	while((1<<st)/2 >= pp)
		st--;
	polycock F, S;
	F.coef = a.coef, S.coef = b.coef;
	F.NTT(st), S.NTT(st);
	//Умножаем
	for(int i = 0; i < F.coef.size(); i++) 
		F.coef[i] = mult(F.coef[i], S.coef[i]);

	// Делаем Обратное преобразование
	F.NTT(st);
	reverse(F.coef.begin() + 1, F.coef.end());
	for(auto &u : F.coef) u = mult(u, inv(1<<st));
	return F;			
}

polycock operator +(polycock &a, polycock &b) {
	ll z = max(a.coef.size(), b.coef.size());
	polycock c;
	c.coef.resize(max(a.coef.size(), b.coef.size()));
	for(int i = 0; i < z; i++)
		c.coef[i] = sum((a.coef.size() > i ? a.coef[i] : 0), (b.coef.size() > i ? b.coef[i] : 0));
	return c;			
}
polycock operator -(polycock &a, polycock &b) {
	ll z = max(a.coef.size(), b.coef.size());
	polycock c;
	c.coef.resize(max(a.coef.size(), b.coef.size()));
	for(int i = 0; i < z; i++)
		c.coef[i] = dif((a.coef.size() > i ? a.coef[i] : 0), (b.coef.size() > i ? b.coef[i] : 0));
	return c;			
}
// NTT
int main() {
	init();
}