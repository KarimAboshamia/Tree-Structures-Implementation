#include <iostream>

using namespace std;

template<typename T, int orderOfTree>
class Node {
private:
    T *keys = new T[orderOfTree];
    Node **children = new Node *[orderOfTree+1];
    int insertedChildren = 0;
    int insertedKeys = 0;
    bool leaf;
public:
    void setLeaf(bool leaf) {
        this->leaf = leaf;
    }

    int getChildrenNum() {
        return insertedChildren;
    }

    bool getLeaf() { return leaf; }

    void insertKeySplit(T keyInput) {
        keys[insertedKeys] = keyInput;
        insertedKeys++;
    }

    void insertKey(T keyInput) {
        int i;
        for (i = insertedKeys - 1; i >= 0; i--) {
            if (keyInput < keys[i]) {
                keys[i + 1] = keys[i];
            } else {
                break;
            }
        }
        keys[i + 1] = keyInput;
        insertedKeys++;
    }

    void insertChild(Node *child) {
        children[insertedChildren] = child;
        insertedChildren++;
    }

    void insertChildAt(Node *child, int index) {
        for (int i = orderOfTree; i > index; i--) {
            children[i] = children[i - 1];
        }
        children[index] = child;
        insertedChildren++;
    }

    Node *getChild(int index){
        return this->children[index];
    }
    Node *Split(Node<T, orderOfTree> *parent, int index) {
        //BaseCase
        if (parent == NULL) {
            Node<T, orderOfTree> *newChild = new Node<T, orderOfTree>;
            newChild->setLeaf(true);
            int median = (orderOfTree + 1) / 2; //shift right

            //copy all keys
            for (int i = median; i < orderOfTree; i++) {
                newChild->insertKey(this->getKey(i));
            }
            int range = this->getChildrenNum();
            for (int i = median; i < range; i++) {
                newChild->insertChild(this->getChild(i));
                this->updateInsertedChildren(this->getChildrenNum()  - 1);

            }

            this->updateKeys(orderOfTree - median);

            Node<T, orderOfTree> *newRoot = new Node<T, orderOfTree>;
            newRoot->setLeaf(false);
            newRoot->insertKey(this->getKey(median - 1));
            newRoot->insertChild(this);

            //Shift then insert
            newRoot->insertChildAt(newChild,(index+1));
            return newRoot;
        } else {

            Node<T, orderOfTree> *newChild = new Node<T, orderOfTree>;
            newChild->setLeaf(true);
            int median = (orderOfTree + 1) / 2; //shift right

            //copy all keys
            for (int i = median; i < orderOfTree; i++) {
                newChild->insertKey(this->getKey(i));
            }

            int range = this->getChildrenNum();
            for (int i = median; i < range; i++) {
                newChild->insertChild(this->getChild(i));
                this->updateInsertedChildren(this->getChildrenNum() - 1);

            }

            this->updateKeys(orderOfTree - median);
            parent->insertKey(this->getKey(median - 1));
            parent->insertChildAt(newChild, (index+1));
            return NULL;
        }
    }

    void SplitNotRoot();

    T getKey(int index) {
        return keys[index];
    };

    int getKeyNum() {
        return insertedKeys;
    }

    bool isLeaf() {
        return leaf;
    }

    Node *getChildren(int index) {
        return children[index];
    }

    void updateInsertedChildren(int num) { insertedChildren = num; }

    void updateKeys(int num) { insertedKeys = num; }

    bool isFull() {
        return insertedKeys == orderOfTree;
    }

};

template<typename T, int orderOfTree>
class BTree {
private:
    int m;
    Node<T, orderOfTree> *rootNode;
public:
    BTree() {
        m = orderOfTree;
        rootNode = NULL;
        rootNode = new Node<T, orderOfTree>;
        rootNode->setLeaf(true);
    }

    void Insert_I(Node<T, orderOfTree> *currentNode, Node<T, orderOfTree> *parent, T treeInput, int index) {
        if (currentNode->isLeaf()) {
            currentNode->insertKey(treeInput);

        } else {
            int i;
            for (i = currentNode->getKeyNum() - 1; i >= 0; i--) {
                if (treeInput > currentNode->getKey(i)) {
                    break;
                }
            }
            Insert_I(currentNode->getChildren(i + 1), currentNode, treeInput, (i + 1));
        }
        //If full split
        if (currentNode->isFull()) {
            Node<T, orderOfTree> *tmp = new Node<T, orderOfTree>;
            tmp = currentNode->Split(parent, index);
            if (tmp != NULL) {
                rootNode = tmp;
            }
        }

    }

    void Insert(T treeInput) {
        Insert_I(rootNode, NULL, treeInput, 0);
    }

    void print_Node(Node<T,orderOfTree> * printNode){
        for (int i = 0; i < printNode->getKeyNum(); i++) {
            cout << printNode->getKey(i) << " ";
        }
        cout << endl;

        for (int i = 0; i < printNode->getChildrenNum(); i++) {
            print_Node(printNode->getChild(i));
        }

    }

    void print() {
        print_Node(rootNode);
    }
};

int main() {
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;
    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);
    t1.print();

    // Construct a BTree of order 5, which stores char data
    BTree<char, 5> t;
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');
    t.print();

    return 0;
}

