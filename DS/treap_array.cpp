#include "bits/stdc++.h"
using namespace std;
struct node {
    int val;
    int siz;
    int pri;
    node* left;
    node* right;
    node(int _val = -1) {
        val = _val;
        siz = 1;
        pri = rand();
        left = NULL;
        right = NULL;
    }
};
typedef node* pnode;
int size(pnode tree) {
    if (tree) {
        return tree -> siz;
    }
    return 0;
}
void update(pnode &tree) {
    if (tree) {
        tree -> siz = size(tree -> left) + 1 + size(tree -> right);
    }
}
void split(pnode tree , pnode &l , pnode &r , int key , int lcount) {
    if (!tree) {
        l = NULL;
        r = NULL;
    }
    else if (lcount + 1 + size(tree -> left) <= key) {
        split(tree -> right , tree -> right , r , key , lcount + 1 + size(tree -> left));
        l = tree;
    }
    else {
        split(tree -> left , l , tree -> left , key , lcount);
        r = tree;
    }
    update(tree);
}
void merge(pnode &tree , pnode l , pnode r) {
    if (!l || !r) {
        tree = l ? l : r;
    }
    else if (l -> pri > r -> pri) {
        merge(l -> right , l -> right , r);
        tree = l;
    }
    else {
        merge(r -> left , l , r -> left);
        tree = r;
    }
    update(tree);
}

void inorder(pnode tree) {
    if (tree) {
        inorder(tree -> left);
        cout << (tree->val) << " ";
        //printf("%d " , tree -> val);
        inorder(tree -> right);
    }
}
node* root = NULL;
void insert(int val) {
    merge(root , root , new node(val));
}

void query(int l , int r) {
    node* temp1 = NULL;
    node* temp2 = NULL;
    node* temp3 = NULL;
    node* temp4 = NULL;
    split(root , temp1 , temp2 , r , 0);
    split(temp1 , temp3 , temp4 , l - 1 , 0);
    merge(root , temp3 , temp2);
    merge(root , temp4 , root);
}
int n , m;
int inp;
int type , l , r;
int main() {
    cin >> n;
    //scanf("%d" , &n);
    for (int i = 1 ; i <= n ; ++i) {
        cin >> inp;
        //scanf("%d" , &inp);
        insert(inp);
    }
    cin >> m;
    //scanf("%d" , &m);
    while (m--) {
        cin >> l >> r;
        //scanf("%d %d", &l , &r);
        query(l, r);

    }
    inorder(root);
}