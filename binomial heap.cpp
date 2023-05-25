#include <iostream>

using namespace std;

#include <bits/stdc++.h>

typedef struct NodeBino {
    int v;                  //vertex
    int data;               //node value
    NodeBino *parent;       //parent pointer
    NodeBino *lchild;       //left child pointer
    NodeBino *sibling;      //right sibling pointer
    int degree;             //number of children
} NodeBino;

NodeBino *makeBinomialHeap(int data, int v);

void binomialLink(NodeBino *x, NodeBino *y);

NodeBino *binomialHeapMerge(NodeBino *h1, NodeBino *h2);

NodeBino *binomialHeapMeld(NodeBino *h1, NodeBino *h2);

NodeBino *binomialHeapMinimum(NodeBino *head);

NodeBino *binomialHeapInsert(NodeBino **head, int data, int v);

void printHeap(NodeBino *head);

NodeBino *binomialReverseRootList(NodeBino *head);

NodeBino *binomialHeapExtractMin(NodeBino **head);

void binomialHeapDecreaseKey(NodeBino *head, NodeBino *x, int data);

void binomialHeapDelete(NodeBino *head, NodeBino *x);

int main() {
    NodeBino *head = NULL;

    int N = 5;
    vector<NodeBino *> nodes(N + 1, NULL);
    head = binomialHeapInsert(&head, 4, 0);
    nodes[0] = head;
    nodes[1] = binomialHeapInsert(&head, 3, 1);
    nodes[2] = binomialHeapInsert(&head, 5, 2);
    nodes[3] = binomialHeapInsert(&head, 6, 3);
    nodes[4] = binomialHeapInsert(&head, 2, 4);
    nodes[5] = binomialHeapInsert(&head, 26, 5);
    nodes[6] = binomialHeapInsert(&head, 17, 6);
    nodes[7] = binomialHeapInsert(&head, 18, 7);
    nodes[8] = binomialHeapInsert(&head, 23, 8);
    nodes[9] = binomialHeapInsert(&head, 55, 9);
    nodes[10] = binomialHeapInsert(&head, 24, 10);
    nodes[11] = binomialHeapInsert(&head, 72, 11);
    nodes[12] = binomialHeapInsert(&head, 13, 12);

//    nodes[0] = head;
//    for(int i = 1; i <= N; ++i){
//        nodes[i] = binomialHeapInsert(&head, 999999, i);
//    }

//    NodeBino *n = binomialHeapExtractMin(&head);
//    binomialHeapDecreaseKey(head, nodes[2], 1);

//    for(int i = 0; i < N; ++i) {
//        cout << nodes[i] << " " << nodes[i]->v << " " << nodes[i]->data;
//        cout <<  endl;
//    }


//    printHeap(head);

//    NodeBino *min = binomialHeapExtractMin(&head);
//    cout << min->data << endl;
//    printHeap(head);


    return 0;
}

NodeBino *binomialHeapExtractMin(NodeBino **head) {
    NodeBino *x = binomialHeapMinimum(*head);

    //REMOVE-x
    if (*head == x) {
        *head = (*head)->sibling;
    } else {
        NodeBino *temp = *head;

        while (temp->sibling != x)
            temp = temp->sibling;

        temp->sibling = x->sibling;
    }

    x->sibling = NULL;

    //MAKE-PARENT-PTRS-OF-CHILD-OF-x-NULL
    NodeBino *h1 = x->lchild;
    x->lchild = NULL;               //MAKE-CHILD-PTR-OF-x-NULL

    NodeBino *temp1 = h1;
    while(temp1 != NULL){
        temp1->parent = NULL;
        temp1 = temp1->sibling;
    }

    x->degree = 0;          //MAKE-DEGREE-OF-x-0

    //REVERSE-THE-ORDER-OF-LINKED-LIST-OF-x'S-CHILDREN-AND-SET-h1-POINT-TO-HEAD-OF-LINKED-LIST-FORMED
    h1 = binomialReverseRootList(h1);

    //MELD-BOTH-HEAPS
    *head = binomialHeapMeld(*head, h1);

    return x;
}

void binomialHeapDelete(NodeBino *head, NodeBino *x) {
    binomialHeapDecreaseKey(head, x, -999999);
    binomialHeapExtractMin(&head);
}

void binomialHeapDecreaseKey(NodeBino *head, NodeBino *x, int data) {
    if (data > x->data) {
        cout << "New Key is greater than old key." << endl;
        return;
    }

    x->data = data;
    NodeBino *y = x;
    NodeBino *z = y->parent;

    while (z != NULL && y->data < z->data) {
        int t = y->data;
        y->data = z->data;
        z->data = t;

        y = z;
        z = y->parent;
    }
}

NodeBino *binomialReverseRootList(NodeBino *head) {
    NodeBino *prev_x = NULL;
    NodeBino *x = head;
    NodeBino *next_x = NULL;

    while (x != NULL) {
        next_x = x->sibling;
        x->sibling = prev_x;
        prev_x = x;
        x = next_x;
    }

    return prev_x;
}

void printHeap(NodeBino *head) {
    while (head != NULL) {
        cout << head->data << " ";
        printHeap(head->lchild);
        head = head->sibling;
    }
}

NodeBino *binomialHeapMinimum(NodeBino *head) {
    NodeBino *y = NULL;
    NodeBino *x = head;
    int min = 999999;

    while (x != NULL) {
        if (x->data < min) {
            min = x->data;
            y = x;
        }
        x = x->sibling;
    }
    return y;
}

NodeBino *binomialHeapInsert(NodeBino **head, int data, int v) {
    NodeBino *x = makeBinomialHeap(data, v);
    *head = binomialHeapMeld(*head, x);
    return x;
}

NodeBino *binomialHeapMeld(NodeBino *h1, NodeBino *h2) {
    if (h1 == NULL)
        return h2;

    if (h2 == NULL)
        return h1;

    NodeBino *head = binomialHeapMerge(h1, h2);
    NodeBino *prev_x = NULL;
    NodeBino *x = head;
    NodeBino *next_x = x->sibling;

    while (next_x != NULL) {
        if (x->degree != next_x->degree) {        //CASE 1
            prev_x = x;
            x = next_x;

        } else {                                //x->degree == next_x->degree
            if (next_x->sibling != NULL && next_x->sibling->degree == x->degree) {       //CASE 2
                prev_x = x;
                x = next_x;
            } else if (x->data <= next_x->data) {               //CASE 3
                x->sibling = next_x->sibling;
                binomialLink(next_x, x);
            } else {                   //CASE 4: x->data > next_x->data
                if (prev_x != NULL)
                    prev_x->sibling = next_x;
                else                //prev_x == NULL
                    head = next_x;

                binomialLink(x, next_x);
                x = next_x;
            }
        }
        next_x = x->sibling;
    }

    return head;
}

void binomialLink(NodeBino *x, NodeBino *y) {
    //Makes x as child of y

    x->parent = y;
    x->sibling = y->lchild;
    y->lchild = x;
    y->degree = y->degree + 1;
}

NodeBino *binomialHeapMerge(NodeBino *h1, NodeBino *h2) {
    if (h1 == NULL)
        return h2;

    if (h2 == NULL)
        return h1;

    NodeBino *head = NULL;
    NodeBino *temp = NULL;

    if (h1->degree <= h2->degree) {
        head = h1;
        h1 = h1->sibling;
    } else {
        head = h2;
        h2 = h2->sibling;
    }

    temp = head;

    while (h1 != NULL && h2 != NULL) {
        if (h1->degree <= h2->degree) {
            temp->sibling = h1;
            h1 = h1->sibling;
        } else {
            temp->sibling = h2;
            h2 = h2->sibling;
        }
        temp = temp->sibling;
    }

    if (h1 != NULL) {
        temp->sibling = h1;
    }
    if (h2 != NULL) {
        temp->sibling = h2;
    }

    return head;
}

NodeBino *makeBinomialHeap(int data, int v) {
    NodeBino *node = (NodeBino *) malloc(sizeof(NodeBino));
    node->v = v;
    node->data = data;
    node->parent = NULL;
    node->lchild = NULL;
    node->sibling = NULL;
    node->degree = 0;
    return node;
}

