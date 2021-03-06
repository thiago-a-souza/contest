#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map> 
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>
#include <stack>  

#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it) 
#define MP(a,b) make_pair((a),(b))
#define ALL(c) (c).begin(),(c).end()
#define FOR(i,v,n) for(int i=v; i<n; i++)
#define FORD(i,v,n) for(int i=(n-1); i>=v; i--)
#define REP(i,n) FOR(i,0,n)
#define REPD(i,n) FORD(i,0,n) 
#define PB push_back
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : (-a))
#define MAX_V 101
#define MAX_A 101
#define INF 10e9 
#define PRINT(x)
const double PI = acos(-1.0); 

using namespace std;

const int DIG = 4;
const int BASE = 10000;
const int TAM = 2048;

struct bigint{
  int v[TAM], n;
  bigint(int x = 0): n(1){ 
       memset(v, 0, sizeof(v));
       v[n++] = x; fix();
  }
  bigint(char *s): n(1){
      memset(v, 0, sizeof(v));
      int sign = 1;
      while(*s && !isdigit(*s)) if(*s++ == '-') sign *= -1; 
      char *t = strdup(s), *p = t + strlen(t);
      while(p > t){
          *p = 0; p = MAX(t, p - DIG);
          sscanf(p, "%d",&v[n]);
          v[n++] *= sign;
      }
      free(t); fix(); 
  }
  bigint& fix(int m = 0){
      n = max(m,n);
      int sign = 0;
      for(int i=1, e=0; i<=n || e && (n=i); i++){
          v[i] += e; e = v[i]/BASE; v[i]%BASE;
          if(v[i]) sign = (v[i] > 0) ? 1 : -1; 
      }
      for(int i=n-1; i>0; i--)
         if(v[i]*sign < 0){ v[i] += sign*BASE; v[i+1] -= sign;}
         while(n && !v[n]) n--;
         return *this;
  }
  int cmp(const bigint& x=0) const{ 
      int i = MAX(n, x.n), t = 0;
//    while(1){ t=::cmp(v[i], x.v[i]); /*if((t || i-- == 0) return t;*/   }
  }
  bool operator <(const bigint& x) const { return cmp(x) < 0; }
  bool operator ==(const bigint& x) const { return cmp(x) == 0; } 
  bool operator !=(const bigint& x) const { return cmp(x) != 0; }
  
  operator string() const{
      ostringstream s; s << v[n];
      for(int i = n - 1; i > 0; i--){
          s.width(DIG); s.fill('0'); s << ABS(v[i]);
      }
      return s.str();
  }
  friend ostream& operator <<(ostream& o, const bigint& x){
      return o << (string) x;
  }
  bigint& operator +=(const bigint& x){ 
      for(int i = 1; i <= x.n; i++) v[i] += x.v[i];
      return fix(x.n);
  }
  bigint operator +(const bigint& x){ return bigint(*this) += x;}
  bigint& operator -=(const bigint &x){  
      for(int i = 1; i <= x.n; i++) v[i] -= x.v[i];
      return fix(x.n);
  }
  bigint operator -(const bigint& x){ return bigint(*this) -= x; }
  bigint operator -(){ bigint r = 0; return r -= *this; } 
  void ams(const bigint& x, int m, int b){ //  *this += (x * m) << b;//////////////////
      for(int i = 1, e = 0; (i <= x.n || e) && (n = i + b); i++){
          v[i+b] += x.v[i] * m + e; e = v[i+b] / BASE; v[i+b] %= BASE; 
      }
  }
  bigint operator *(const bigint& x) const {
      bigint r;
      for(int i = 1; i <= n; i++) r.ams(x, v[i], i-1);
      return r;
  }
  bigint& operator *=(const bigint& x){ return *this = *this * x; } 
  bigint div(const bigint& x){
      if(x == 0) return 0;
      bigint q; q.n = MAX(n - x.n + 1, 0);
      int d = x.v[x.n] * BASE + x.v[x.n-1];
      for(int i = q.n; i > 0; i--){
          int j = x.n + i -1;
          q.v[i] = int((v[j] * double(BASE) + v[j-1])/d);
          ams(x, -q.v[i], i-1);
          if(i == 1 || j == 1) break;
          v[j-1] += BASE * v[j]; v[j] = 0;
      }
      fix(x.n ); return q.fix();
  }
  bigint& operator /=(const bigint& x){ return *this = div(x); }
  bigint& operator %=(const bigint& x){ div(x); return *this; }
  bigint operator /(const bigint& x){ return bigint(*this).div(x); } 
  bigint operator %(const bigint& x){ return bigint(*this) %= x; }
  bigint pow(int x){
      if(x < 0) return (*this == 1 || *this == -1) ? pow(-x) : 0;
      bigint r = 1;
      for(int i = 0; i < x; i++) r *= *this; 
      return r;
  }
  bigint root(int x){
      if(cmp() == 0 || cmp() < 0 && x%2 == 0) return 0;
      if(*this == 1 || x == 1) return *this;
      if(cmp() < 0) return -(-*this).root(x); 
      bigint a = 1, d = *this;
      while(d != 1){
          bigint b = a + (d /= 2);
          if(cmp(b.pow(x)) >= 0){ d += 1; a = b; }
      }
      return a;
  }
};

main(){
int n, dig[10], size;
bigint b;
vector<string> v;

/*string s = "012345";
int val = s[0];
printf("%d %d %d %d %d\n",(int)s[0],(int)s[1],(int)s[2],(int)s[3],(int)s[4],(int)s[5]);
return 0;
*/
    b = bigint("1");
    v.PB("1");
    for(int i=1; i<=366; i++){
       b *= i;
       v.PB((string)b);
    }
    

     
    
    
    while(true){ 
        scanf("%d",&n);
        if(n == 0)
           break;
        for(int i=0; i<10; i++)
            dig[i] = 0;
        size = v[n].size();
       // cout << "v--> " <<v[n] << endl;
        for(int i=0; i<size; i++)
            dig[(int)v[n][i]-48]++;
        printf("%d! --\n",n);
        printf("   (0)%5d    (1)%5d    (2)%5d    (3)%5d    (4)%5d\n",dig[0],dig[1],dig[2],dig[3],dig[4]);
        printf("   (5)%5d    (6)%5d    (7)%5d    (8)%5d    (9)%5d\n",dig[5],dig[6],dig[7],dig[8],dig[9]);
        
        
        
    }

}

