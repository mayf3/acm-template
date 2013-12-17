//By Lin
#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;

struct    Node{
    int key,weight,size;
    Node *l,*r;
    Node(int _key , int _weight, Node *_l, Node* _r):
        key(_key),weight(_weight),l(_l),r(_r){
            size = 1;
            if ( l ) size += l->size;
            if ( r ) size += r->size;
        }
    Node *newnode(int key){
        return new Node(key,rand(),NULL,NULL);
    }
    inline int lsize(){ return l?l->size:0; }
    inline int rsize(){ return r?r->size:0; }
}*root[50005];
Node*    Meger(Node *a , Node *b ){
    if ( !a || !b ) return a?a:b;
    return a->weight>b->weight?
         new Node(a->key,a->weight,a->l,Meger(a->r,b)):
         new Node(b->key,b->weight,Meger(a,b->l),b->r);
}
Node*    Split_L(Node *a ,int size ){
    if ( !a || size == 0 ) return NULL;
    return a->lsize() < size?
        new Node(a->key,a->weight,a->l,Split_L(a->r,size-1-a->lsize())):
        Split_L(a->l,size);
}
Node*    Split_R(Node *a ,int size ){
    if ( !a || size == 0 ) return NULL;
    return a->rsize() < size?
        new Node(a->key,a->weight,Split_R(a->l,size-1-a->rsize()),a->r):
        Split_R(a->r,size);
}
int        Ask( Node *a ,int k ){
    if ( a->lsize() >= k ) return Ask(a->l,k);
    k -= a->lsize()+1;
    if ( k == 0 ) return a->key;
    return Ask(a->r,k);
}
int        len = 0;

int        main(){
    int d = 0 , cas;
    scanf("%d", &cas );
    root[0] = NULL;
    int cnt = 1,kind,v,p,c;
    char s[1005];
    while ( cas -- ) {
        scanf("%d", &kind );
        if ( kind == 1 ) {
            scanf("%d%s", &p , s );
            p-=d;
            Node *l = Split_L(root[cnt-1],p),
                 *r = Split_R(root[cnt-1],len-p);
            for (int i = 0; s[i]; i++ ){
                l = Meger(l,new Node(s[i],rand(),NULL,NULL));
                len++;
            }
            root[cnt++] = Meger(l,r);
        }
        else if ( kind == 2){
            scanf("%d%d", &p , &c );
            p-=d,c-=d;
            Node *l = Split_L(root[cnt-1],p-1),
                 *r = Split_R(root[cnt-1],len-p-c+1);
            len -= c;
            root[cnt++] = Meger(l,r);
        }
        else{
            scanf("%d%d%d", &v, &p , &c );
            v-=d,p-=d,c-=d;
            char ch;
            for (int i = p; i<p+c; i++) {
                printf("%c", ch = Ask(root[v],i) );
                if ( ch == 'c' ) d++;
            }
            puts("");
        }
    }
    return 0;
}
