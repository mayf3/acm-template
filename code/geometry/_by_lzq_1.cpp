//By Lin
#include<cstdio>
#include<cstring>
#include<map>
#define mp(x,y) make_pair(x,y)
#define foreach(i,n) for(__typeof(n.begin()) i = n.begin(); i!=n.end(); i++)
#define X first
#define Y second
using namespace std;
typedef long long LL;
typedef pair<int,int> pii;

map<int,int> up[2];
map<int,int>::iterator iter,p,q;

int        strcmp( pii a, pii b, pii c){
    LL    ret = ((LL)b.X-a.X)*((LL)c.Y-a.Y)-((LL)b.Y-a.Y)*((LL)c.X-a.X);
    return ret>0?1:(ret==0?0:-1);
}

bool    pan( map<int,int> &g ,int x,int y){
    if ( g.size() == 0 ) return false;
    if ( g.find(x) != g.end() ) return y>=g[x];
    if ( g.begin()->X > x || (--g.end())->X < x ) return false;
    iter = g.lower_bound(x);
    p = q = iter;
    p--;
    return strcmp(*p,*q,mp(x,y))>=0;
}

void    insert( map<int,int> &g, int x,int y){
    if ( pan(g,x,y) ) return;
    g[x] = y;
    iter = g.find(x);
    while ( iter != g.begin() ){
        p = iter;
        p--;
        if ( p == g.begin() ) break;
        q = p;
        q--;
        if ( strcmp(*q,*iter,*p)>=0 ) g.erase(p);
        else break;
    }
    iter = g.find(x);
    while ( true ){
        p = iter;
        p++;
        if ( p == g.end() ) break;
        q = p;
        q++;
        if ( q == g.end() ) break;
        if ( strcmp(*iter,*q,*p)>=0 ) g.erase(p);
        else break;
    }
}
int        main(){
    int cas;
    scanf("%d", &cas );
    while ( cas -- ){
        int k,x,y
        scanf("%d%d%d", &k, &x, &y );
        if ( k == 1 )
            insert( up[0], x,y ),
            insert( up[1], x,-y);
        else
            printf( pan(up[0],x,y)&&pan(up[1],x,-y)?"YES\n":"NO\n" );
    }
    return 0;
}
