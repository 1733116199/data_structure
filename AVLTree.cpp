template <class K>
class AVLTree{
public:
    struct Node{
        K key, size, height;
        Node * left, *right;
        Node(K k) : key(k), size(1), height(0), left(NULL), right(NULL) {}
    };
    Node * root;
    AVLTree() : root(NULL) {}

#define h(subtree) ((subtree) ? subtree->height : -1)
#define s(subtree) ((subtree) ? subtree->size : 0)

    void update(Node *&subtree){
        subtree->height = max(h(subtree->left), h(subtree->right)) + 1;
        subtree->size = s(subtree->left) + s(subtree->right) + 1;
    }
    void rotateLeft(Node *& t){
        Node* newSubRoot = t->right;
        t->right = newSubRoot->left;
        newSubRoot->left = t;
        t = newSubRoot;
        update(t->left);
        update(t);
    }
    void rotateRight(Node *& t){
        Node* newSubRoot = t->left;
        t->left = newSubRoot->right;
        newSubRoot->right = t;
        t = newSubRoot;
        update(t->right);
        update(t);
    }
    void rotateLeftRight(Node *& t){
        rotateLeft(t->left);
        rotateRight(t);
    }
    void rotateRightLeft(Node *& t){
        rotateRight(t->right);
        rotateLeft(t);
    }
    void rebalance(Node *& subtree){
        Node *l = subtree->left, *r = subtree->right;
        K diff = h(l) - h(r);
        if(abs(diff) <= 1)
            return update(subtree);
        if(diff > 1){
            diff = h(l->left) - h(l->right);
            return (diff >= 0) ? rotateRight(subtree) : rotateLeftRight(subtree);
        }else{
            diff = h(r->right) - h(r->left);
            return (diff >= 0) ? rotateLeft(subtree) : rotateRightLeft(subtree);
        }
    }
    void insert(K k){
        insert(root, k);
    }
    void insert(Node * & subtree, K k){
        if(subtree == NULL)
            subtree = new Node(k);
        else {
            subtree->size++;
            if (k > subtree->key)
                insert(subtree->right, k);
            else
                insert(subtree->left, k);
        }
        rebalance(subtree);
    }
    int numBetween(K lowerBound, K upperBound){
        return findInclusive(root, upperBound) - findExclusive(root, lowerBound);
    }

    int findInclusive(Node *& subtree, K k){
        if(subtree == NULL)
            return 0;
        if(k >= subtree->key){
            return s(subtree->left) + 1 + findInclusive(subtree->right, k);
        }else{
            return findInclusive(subtree->left, k);
        }
    }

    int findExclusive(Node *& subtree, K k){
        if(subtree == NULL)
            return 0;
        if(k > subtree->key){
            return s(subtree->left) + 1 + findExclusive(subtree->right, k);
        }else{
            return findExclusive(subtree->left, k);
        }
    }
};
