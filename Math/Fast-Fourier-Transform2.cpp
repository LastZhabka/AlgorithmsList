#include <bits/stdc++.h>
using namespace std;

// FFT
#define pild pair<double, double>
#define ll long long
#define mp make_pair
#define f first
#define s second
const double pi = acos(-1.0);
const ll M = (1<<20) + 20;
 
vector<int> dp(M, 0);
vector<pild> pd, pd2;
pild pr[30];
pild w[M];
 
struct polycock {
    vector<pild> coef;
 
    void FFT(ll n) {
        // ������������ ������ ������� ��� ��������.
        pd.resize(1<<n), pd2.resize(1<<n);
 
        pild z = mp(cos(2.0 * pi / (1<<n)), sin(2.0 * pi / (1<<n)));
         
        for(int j = 0; j < (1<<n); j++) {
            // dp[j] - ��� ����� 
            dp[j] = (dp[j>>1]>>1)^((j&1)? (1<<(n - 1)) : 0);
            //������ ��������� ��� � ������ ��� ������, ����� ������� ��� � ������ ����� ����� ����� � ������� ����
            pd[j] = mp((coef.size() > dp[j] ? coef[dp[j]].f : 0), (coef.size() > dp[j] ? coef[dp[j]].s : 0)); 
            // ������ ���� ��� ����������� ������� 1
 
            // ������ e^(2pi*k/sz)
            if(j == 0)w[j] = mp(1, 0);
            else {
                w[j].f = w[j - 1].f * z.f - w[j - 1].s * z.s;
                w[j].s = w[j - 1].s * z.f + w[j - 1].f * z.s;
            }
        }
        
        // ����������� ������ �������
        pild x;
        for(int j = 1; j <= n; j++) {   
            // ���������� ����.
            for(int i = 0; i < (1<<n); i += (1<<j)) {
                int p = (1<<j);
                // ������� ����� ��� ������ ����� ��� �������� ����������
                for(int k = 0; k < p/2; k++) {
                    int L = i + k, R = i + k + p/2;
                    // ��� ����� i-�� ������ ������� j
                    // �� � ��� ���� ������ ������ ������� n
                    // ����� �������� i-�� ������ ������� j �� ������ �������� �� ��������� 2^n/2^j = 2^(n-j)
                    // (2 * pi * k)/(2^n), 2 * pi * (i * (2^(n-j)) / (2^n) = (2 * pi * i) / (2^j)
                    x = w[k<<(n - j)];
                    pd2[i + k].f = pd[L].f + pd[R].f * x.f - pd[R].s * x.s;
                    pd2[i + k].s = pd[L].s + pd[R].f * x.s + pd[R].s * x.f;
                    pd2[i + k + p/2].f = pd[L].f - (pd[R].f * x.f - pd[R].s * x.s);
                    pd2[i + k + p/2].s = pd[L].s - (pd[R].f * x.s + pd[R].s * x.f);
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
    F.FFT(st), S.FFT(st);
    //��������
    for(int i = 0; i < F.coef.size(); i++) {
        pild x;
        x.f = F.coef[i].f * S.coef[i].f - F.coef[i].s * S.coef[i].s;                
        x.s = F.coef[i].s * S.coef[i].f + F.coef[i].f * S.coef[i].s;
        F.coef[i] = x;
    }
    // ������ �������� �������������� �����
    F.FFT(st);
    reverse(F.coef.begin() + 1, F.coef.end());
    for(auto &u : F.coef) u.f /= (1<<st), u.s /= (1<<st);
    return F;           
}
 
polycock operator +(polycock &a, polycock &b) {
    ll z = max(a.coef.size(), b.coef.size());
    polycock c;
    c.coef.resize(max(a.coef.size(), b.coef.size()));
    for(int i = 0; i < z; i++)
        c.coef[i].f = (a.coef.size() > i ? a.coef[i].f : 0) + (b.coef.size() > i ? b.coef[i].f : 0),
        c.coef[i].s = (a.coef.size() > i ? a.coef[i].s : 0) + (b.coef.size() > i ? b.coef[i].s : 0);        
    return c;           
}
polycock operator -(polycock &a, polycock &b) {
    ll z = max(a.coef.size(), b.coef.size());
    polycock c;
    c.coef.resize(max(a.coef.size(), b.coef.size()));
    for(int i = 0; i < z; i++)
        c.coef[i].f = (a.coef.size() > i ? a.coef[i].f : 0) - (b.coef.size() > i ? b.coef[i].f : 0),
        c.coef[i].s = (a.coef.size() > i ? a.coef[i].s : 0) - (b.coef.size() > i ? b.coef[i].s : 0);        
    return c;           
}
// FFT
int main() {

}