//By myf
//#pragma comment(linker, "/STACK:16777216")  //C++
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <complex>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <list>

#define rep(i,n) for(int i=0;i<(n);i++)
#define REP(i,l,r) for(int i=(l);i<(r);i++)
#define fab(i,a,b) for(int i=(a);i<=(b);i++)
#define fba(i,b,a) for(int i=(b);i>=(a);i--)
//#define foreach(i,n) for(__typeof(n.begin()) i=n.begin();i!=n.end();i++) //G++
#define MP make_pair
#define PB push_back
#define X first
#define Y second
#define Cls(x) memset(x,0,sizeof x)
#define Print(n,x) for(int i=0;i<(n);i++) cout<<x<<" ";cout<<endl;
#define Sqr(x) (x)*(x)

using namespace std;

typedef long long LL;
typedef pair<int,int> PII;
typedef pair<PII,int> PIII;
typedef pair<LL,int> PLI;
typedef vector<int> VI;
typedef double T;
typedef pair<T,T> Point;

const int N=55,K=3;
const double eps=1E-15;

struct Line{
    double y1,y2;
    int val;
    Line(double y1=0,double y2=0,int val=0):y1(y1),y2(y2),val(val){}
    bool friend operator <(Line a,Line b){return a.y1+a.y2<b.y1+b.y2;}
}y[N*2];

int n;
double x[N*N*N];
double ans[N];

int sign(double x){
    if (x>eps) return 1;
    if (x<-eps) return -1;
    return 0;
}

Point operator -(Point a,Point b){return MP(a.X-b.X,a.Y-b.Y);}
T operator ^(Point a,Point b){return a.X*b.Y-a.Y*b.X;}

int segmemtCross(Point a,Point b,Point c,Point d){
    int f1=sign((a-b)^(c-b))*sign((a-b)^(d-b));
    int f2=sign((c-d)^(a-d))*sign((c-d)^(b-d));
    if (f1==1||f2==1) return -1;
    if (f1==-1&&f2==-1) return 1;
    return 0;
}

Point ip(Point p,Point p2,Point q,Point q2){
    Point v=p2-p,w=q2-q;
    Point u=p-q;
    double t=(w^u)/(v^w);
    return MP(p.X+v.X*t,p.Y+v.Y*t);
}

Point v[N][3];

int main(){
    int test;
    scanf("%d",&test);
    rep(cas,test){
        scanf("%d",&n);
        rep(i,n) rep(j,K) scanf("%lf%lf",&v[i][j].X,&v[i][j].Y);
        int nn=n,cnt=0;
        rep(i,n){
            if (sign((v[i][1]-v[i][0])^(v[i][2]-v[i][0]))==0) continue;
            rep(j,K) v[cnt][j]=v[i][j];
            cnt++;
        }
        n=cnt,cnt=0;
        rep(i,n) rep(j,K) x[cnt++]=v[i][j].X;
        rep(i,n) rep(j,n){
            rep(p,K) rep(q,K){
                if (segmemtCross(v[i][p],v[i][(p+1)%3],v[j][q],v[j][(q+1)%3])==1){
                    x[cnt++]=ip(v[i][p],v[i][(p+1)%3],v[j][q],v[j][(q+1)%3]).X;
                }
            }
        }
        sort(x,x+cnt);
        Cls(ans);
        int m;
        rep(k,cnt-1){
            m=0;
            double x1=x[k],x2=x[k+1];
            if (sign(x1-x2)==0) continue;
            rep(i,n){
                bool flag=false;
                rep(j,3){
                    if ( sign( min(v[i][j].X,v[i][(j+1)%3].X)-x1)<=0 && sign( max(v[i][j].X,v[i][(j+1)%3].X)-x2)>=0){
                        y[m++]=Line(ip(v[i][j],v[i][(j+1)%3],Point(x1,-1),Point(x1,101)).Y,
                                    ip(v[i][j],v[i][(j+1)%3],Point(x2,-1),Point(x2,101)).Y,1);
                        flag=true;
                    }
                }
                if (flag){
                    if (y[m-2]<y[m-1]) y[m-1].val=-1;
                    else y[m-2].val=-1;
                }
            }
            sort(y,y+m);
            int now=0;
            rep(i,m){
                if (i){
                    double L=(y[i].y1-y[i-1].y1)+(y[i].y2-y[i-1].y2);
                    ans[now]+=L/2*(x2-x1);
                }
                now+=y[i].val;
            }
        }
        rep(i,nn) printf("%.10lf\n",ans[i+1]);
    }
    return 0;
}
