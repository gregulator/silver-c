#include "zlist.h"
#include <stdio.h>

typedef struct MyNode
{
    int v0;
    float v1;
    ZLINK(struct MyNode) link;
} MyNode;

int main(void)
{
    ZLIST(MyNode) mylist = ZLIST_NEW();
    MyNode *a;
    MyNode *b;
    MyNode *c;

    a = malloc(sizeof(MyNode));
    b = malloc(sizeof(MyNode));
    c = malloc(sizeof(MyNode));

    a->v0 = 100;
    a->v1 = 32.0f;
    b->v0 = 44;
    b->v1 = 29.0f;
    c->v0 = 5;
    c->v1 = 6.0f;

    ZLIST_ADD_TO_FRONT_EX(mylist, c, link);
    ZLIST_ADD_TO_FRONT_EX(mylist, b, link);
    ZLIST_ADD_TO_FRONT_EX(mylist, a, link);

    printf("%d %d\n", ZLIST_HEAD(mylist)->v0, ZLIST_NEXT(ZLIST_HEAD(mylist))->v0);
    MyNode *t0, *t1, *t2;
    t0 = ZLIST_POP_FRONT(mylist);
    t1 = ZLIST_POP_FRONT(mylist);
    t2 = ZLIST_POP_FRONT(mylist);
    printf("%f %f %f\n", t0->v1, t1->v1, t2->v1);
    
    return 0;
}
