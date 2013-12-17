#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>

using namespace std;

const int N=11111,M=2*N;

int n,k,tot;
int Tot,Min,Root;
int a[N],next[M];
int size[N],f[N];
int q[N];
pair<int,int> map[M],d[N];
bool use[N];

void Insert(int x,int y,int c){
    map[tot]=make_pair(y,c);
    next[tot]=a[x],a[x]=tot++;
}

void Get_Dist(int now,int dist,int fa){
    q[tot++]=dist;
    for(int p=a[now];p;p=next[p]){
        int y=map[p].first,c=map[p].second;
        if (use[y]||y==fa)continue;
        Get_Dist(y,dist+c,now);
    }
}

int Count(int x,int dist){
    int s=0;
    tot=0;
    Get_Dist(x,dist,-1);
    sort(q,q+tot);
    for(int i=0,j=tot-1;i<=j;i++){
        while(q[i]+q[j]>k&&i<j)j--;
        if (i<j) s+=j-i;
    }
    return s;
}

void Get_Root(int now,int fa){
    int big=-1;
    size[now]=1;
    for(int p=a[now];p;p=next[p]){
        int y=map[p].first,c=map[p].second;
        if (use[y]||y==fa) continue;
        Get_Root(y,now);
        size[now]+=size[y];
        big=max(big,size[y]);
    }
    big=max(big,Tot-size[now]);
    if (big<Min) Min=big,Root=now;
}
/*
void Dfs(int x){
     Tot=Min=size[x];
     Get_Root(x,-1);
     x=Root;
     f[x]=Count(x,0);
     //cout<<i<<" w "<<f[i]<<endl;
     use[x]=true;
     for(int p=a[x];p;p=next[p]){
         int y=map[p].first,c=map[p].second;
         if (use[y]) continue;
         f[x]-=Count(y,c);
         Dfs(y);
     }
 }
*/
void Dfs(){
    int top=0;
    size[0]=n;
    d[++top]=make_pair(0,a[0]);
    while(top>0){
        int now=d[top].first,p=d[top].second;
        if (!use[now]){
            Tot=Min=size[now];
            Get_Root(now,-1);
            now=Root,f[now]=Count(now,0);
            use[now]=true;
            d[top]=make_pair(now,a[now]);
            continue;
        }
        for(;p;p=next[p]){
            int y=map[p].first,c=map[p].second;
            if (!use[y]){
                f[now]-=Count(y,c);
                d[top].second=next[p];
                d[++top]=make_pair(y,a[y]);
                break;
            }
        }
        if (d[top].first!=now) continue;
        else top--;
    }
}


void In(int &x){
    char ch;
    while((ch=getchar())&&!(ch>='0'&&ch<='9'));
    x=ch-48;
    while((ch=getchar())&&ch>='0'&&ch<='9') x=x*10+ch-48;
}

int main(){
    while(true){
        In(n),In(k);
        if (!n&&!k) break;
        memset(a,0,sizeof(a));
        memset(use,0,sizeof(use));
        tot=1;
        for(int i=0,x,y,c;i!=n-1;i++){
            In(x),In(y),In(c);
            x--,y--;
            Insert(x,y,c);
            Insert(y,x,c);
        }
//        size[0]=n;
        Dfs();
        int ans=0;
        for(int i=0;i!=n;i++) ans+=f[i];
        printf("%d\n",ans);
    }
    return 0;
}
