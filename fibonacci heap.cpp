#include <iostream>

using namespace std;

#include <bits/stdc++.h>

typedef struct NodeFibo {
    int v;                  //vertex
    int data;               //node value
    NodeFibo *parent;       //parent pointer
    NodeFibo *lchild;       //left child pointer
    NodeFibo *lsibling;      //left sibling pointer
    NodeFibo *rsibling;      //right sibling pointer
    int degree;             //number of children
    int marked;             //1 if markded, 0 otherwise
} NodeFibo;

typedef struct FiboHeap {
    NodeFibo *min;
    int n;
} FiboHeap;

FiboHeap *makeFibHeap();

NodeFibo *makeFibNode(int vertex, int data);

FiboHeap *fibHeapInsert(FiboHeap *heap, int v, int data, vector<NodeFibo *>& nodes);

NodeFibo *fibHeapExtractMin(FiboHeap **heap);

void fibHeapLink(FiboHeap *heap, NodeFibo *y, NodeFibo *x);

void consolidate(FiboHeap **heap);

void cut(FiboHeap *heap, NodeFibo *x, NodeFibo *y);

void cascadingCut(FiboHeap *heap, NodeFibo *y);

void fibHeapDecreaseKey(FiboHeap *heap, NodeFibo *x, int k);

void display(FiboHeap *heap);

int main() {
    FiboHeap *heap = makeFibHeap();
    int N;
    vector<NodeFibo *> nodes(N, NULL);


//    heap = fibHeapInsert(heap, 24);
//    heap = fibHeapInsert(heap, 17);
//    heap = fibHeapInsert(heap, 3);
//    heap = fibHeapInsert(heap, 23);
//    heap = fibHeapInsert(heap, 7);
//    heap = fibHeapInsert(heap, 21);
//    heap = fibHeapInsert(heap, 18);
//    heap = fibHeapInsert(heap, 52);
//    heap = fibHeapInsert(heap, 38);


    display(heap);

    return 0;
}

void fibHeapDecreaseKey(FiboHeap *heap, NodeFibo *x, int k){
    if(k > x->data){
        cout << "New Key is greater than old key." << endl;
        return;
    }

    x->data = k;
    NodeFibo *y = x->parent;

    if(y != NULL && x->data < y->data){
        cut(heap, x, y);
        cascadingCut(heap, y);
    }

    if(x->data < heap->min->data)
        heap->min = x;
}

void cascadingCut(FiboHeap *heap, NodeFibo *y){
    NodeFibo *z = y->parent;
    if(z != NULL){
        if(y->marked == 0)
            y->marked = 1;
        else{
            cut(heap, y, z);
            cascadingCut(heap, z);
        }
    }
}

void cut(FiboHeap *heap, NodeFibo *x, NodeFibo *y){
    //REMOVE-x-FROM-CHILD-LIST-OF-y
    NodeFibo *left = x->lsibling;
    NodeFibo *right = x->rsibling;

    left->rsibling = right;
    right->lsibling = left;

    if(y->lchild == x){
        if(x->lsibling == x)
            y->lchild = NULL;
        else
            y->lchild = x->rsibling;
    }

    y->degree = y->degree - 1;

    //ADD-x-TO-THE-ROOT-LIST-OF-H
    NodeFibo *min = heap->min;
    if (min->lsibling != min) {
        NodeFibo *temp = min->lsibling;
        x->rsibling = min;
        x->lsibling = temp;
        temp->rsibling = x;
        min->lsibling = x;
    } else {
        x->rsibling = min;
        x->lsibling = min;
        min->lsibling = x;
        min->rsibling = x;
    }


    x->parent = NULL;
    x->marked = 0;
}

NodeFibo *fibHeapExtractMin(FiboHeap **heap) {
    NodeFibo *min = (*heap)->min;

    if (min != NULL) {
        NodeFibo *nodet = min->lchild;

        vector<NodeFibo *> childList;
        if (nodet != NULL) {
            do {
                childList.push_back(nodet);
                nodet = nodet->rsibling;
            } while (nodet != min->lchild);

            for (auto node: childList) {
                if (min->lsibling != min) {
                    NodeFibo *temp = min->lsibling;
                    node->rsibling = min;
                    node->lsibling = temp;
                    temp->rsibling = node;
                    min->lsibling = node;
                } else {
                    node->rsibling = min;
                    node->lsibling = min;
                    min->lsibling = node;
                    min->rsibling = node;
                }

                node->parent = NULL;
            }
        }

            min->lchild = NULL;

            //REMOVE-min-FROM-ROOT-LIST
            NodeFibo *left = min->lsibling;
            NodeFibo *right = min->rsibling;

            left->rsibling = right;
            right->lsibling = left;

            if (min == min->rsibling)
                (*heap)->min = NULL;
            else {
                (*heap)->min = min->rsibling;
                consolidate(heap);
            }

            (*heap)->n = (*heap)->n - 1;
        }

    return min;
}

void consolidate(FiboHeap **heap) {
    //INITIALISING-DEGREE-ARRAY-A
    vector<NodeFibo *> A(log2((*heap)->n) + 1, NULL);
    NodeFibo *min = (*heap)->min;

    //MAKING-ROOT-LIST
    vector<NodeFibo *> rootList;
    NodeFibo *temp = min;
    do {
        rootList.push_back(temp);
        temp = temp->rsibling;
    } while (temp != min);

    for (auto w: rootList) {
        NodeFibo *x = w;
        int d = x->degree;

        while (A[d] != NULL) {
            NodeFibo *y = A[d];

            if (x->data > y->data) {      //EXCHANGE-x-WITH-y
                NodeFibo *dumb = x;
                x = y;
                y = dumb;
            }

            fibHeapLink(*heap, y, x);
            A[d] = NULL;
            d = d + 1;
        }

        A[d] = x;
    }

    //UPDATE-MIN-PTR
    (*heap)->min = NULL;
    for (int i = 0; i < A.size(); ++i) {
        if (A[i] != NULL) {
            if ((*heap)->min == NULL)
                (*heap)->min = A[i];
            else {
                if (A[i]->data < (*heap)->min->data)
                    (*heap)->min = A[i];
            }
        }
    }
}

void fibHeapLink(FiboHeap *heap, NodeFibo *y, NodeFibo *x) {
    //REMOVE-y-FROM-ROOT-LIST-OF-HEAP
    y->parent = x;
    NodeFibo *prev_y = y->lsibling;
    NodeFibo *next_y = y->rsibling;

    if (prev_y != y) {
        prev_y->rsibling = next_y;
    }

    if (next_y != y) {
        next_y->lsibling = prev_y;
    }

    //MAKE-y-A-CHILD-OF-x
    if (x->lchild != NULL) {
        NodeFibo *sib = x->lchild;
        NodeFibo *left = sib->lsibling;
        y->rsibling = sib;
        y->lsibling = left;
        sib->lsibling = y;
        left->rsibling = y;
    }
    else{
        y->rsibling = y;
        y->lsibling = y;
    }

    x->lchild = y;
    x->degree = x->degree + 1;
    y->marked = 0;
}

void display(FiboHeap *heap) {
    NodeFibo *min = heap->min;
    if(min == NULL)
        return;
    NodeFibo *temp = min->lsibling;
    while (temp != min) {
        cout << temp->data << " ";
        temp = temp->lsibling;
    }
    cout << min->data << " ";
}

FiboHeap *fibHeapInsert(FiboHeap *heap, int v, int data, vector<NodeFibo *>& nodes) {
    NodeFibo *node = makeFibNode(v, data);
    nodes[v] = node;
    if (heap->min == NULL) {
        heap->min = node;
        node->lsibling = node;
        node->rsibling = node;
    } else {
        NodeFibo *min = heap->min;
        if (min->lsibling != min) {
            NodeFibo *temp = min->lsibling;
            node->rsibling = min;
            node->lsibling = temp;
            temp->rsibling = node;
            min->lsibling = node;
        } else {
            node->rsibling = min;
            node->lsibling = min;
            min->lsibling = node;
            min->rsibling = node;
        }

        if (node->data < min->data) {
            heap->min = node;
        }
    }

    heap->n = heap->n + 1;
    return heap;
}

FiboHeap *makeFibHeap() {
    FiboHeap *node = (FiboHeap *) malloc(sizeof(FiboHeap));
    node->min = NULL;
    node->n = 0;

    return node;
}

NodeFibo *makeFibNode(int vertex, int data) {
    NodeFibo *node = (NodeFibo *) malloc(sizeof(NodeFibo));
    node->v = vertex;
    node->data = data;
    node->parent = NULL;
    node->lchild = NULL;
    node->lsibling = NULL;
    node->rsibling = NULL;
    node->degree = 0;
    node->marked = 0;
    return node;
}
