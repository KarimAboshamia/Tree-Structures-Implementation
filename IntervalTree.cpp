#include <iostream>

using namespace std;

//struct node has low,high and max values and left,right children
struct Node
{
    int low, high, max;
    Node *leftNode, *rightNode;
    //default constructor initializes an empty node
    Node()
    {
        low = high = max = 0;
        leftNode = rightNode = NULL;
    }
};

//class IntervalTree has the root node and an array to store the deleted node and a counter to count the number of deleted nodes
class IntervalTree
{
private:
    Node *rootNode;
    int *deletedNodes = new int[1000000];
    int deletedNodesCount = 0;
public:
    //default constructor initializes the root node as an empty node
    IntervalTree()
    {
        rootNode = new Node();
    }


    //function insertInterval calls another function insertInterval with the root node as additional parameter to start inserting from it
    void insertInterval(int low, int high)
    {
        insertInterval(rootNode, low, high);
    }

    //function insertInterval inserts a new interval in the tree
    void insertInterval(Node *currentNode, int low, int high)
    {
        //base case,if the current node is empty,insert the interval(set low and high of current node with parameters low and high and set the max with high)
        if (currentNode->low == 0 && currentNode->high == 0)
        {
            currentNode->low = low;
            currentNode->high = high;
            currentNode->max = high;
            currentNode->leftNode = new Node();     //set the left child of the current node as empty node
            currentNode->rightNode = new Node();    //set the right child of the current node as empty node
        }
        //if the interval's low is less than current node's low ,go to left
        else if (low < currentNode->low) insertInterval(currentNode->leftNode, low, high);
        //else, go to right
        else insertInterval(currentNode->rightNode, low, high);
        //update the max of the current node if needed
        if (currentNode->max < high) currentNode->max = high;

    }

    //function overlap calls another function overlap with the root node as additional parameter to start searching from it
    bool overlap(int low, int high)
    {
        return overlap(rootNode, low, high);
    }
    //function overlap checks if the interval overlaps with any other interval in the tree
    bool overlap(Node *currentNode, int low, int high)
    {
        //base case, if the current node is empty return false
        if (currentNode->low == 0 && currentNode->high == 0)
        {
            cout<<"No Other Overlapping Interval Found"<<endl;
            return false;
        }
        //if the current node's low is less than or equal the interval's low and the current node's high is greater than or equal the interval's high, then the interval overlaps with the current node
        else if (currentNode->low <= high && currentNode->high >= low)
        {
            //print the overlapping interval found
            cout << "Overlapping Interval Found: [" << currentNode->low << "," << currentNode->high << "]\n";

            //store the found overlapping interval's low and high in the array of deleted nodes
            deletedNodes[deletedNodesCount] = currentNode->low;
            deletedNodes[deletedNodesCount + 1] = currentNode->high;
            //increase the deleted nodes count by 2(low and high)
            deletedNodesCount += 2;
            //delete the found overlapping interval to continue the search for another overlapping interval
            deleteInterval(currentNode->low, currentNode->high);

            return true;
        }
        //else,check if the left child's max is greater than or equal the interval's low,then go to left and check again
        else if (currentNode->leftNode->max >= low) return overlap(currentNode->leftNode, low, high);
        //else,go to right and check again
        else return overlap(currentNode->rightNode, low, high);
    }

    //function deleteInterval calls another function deleteInterval with the root node as additional parameter to start deleting from it
    void deleteInterval(int low, int high)
    {
        deleteInterval(rootNode, low, high);
    }

    // function to find the inOrder Successor
    Node *inOrderSuccessor(Node *currentNode)
    {
        Node *tempNode = currentNode;
        //traverse the tree until the minimum (most left) node is found
        while (tempNode->leftNode->low != 0 && tempNode->leftNode->high != 0) tempNode = tempNode->leftNode;

        return tempNode;
    }
    //function to delete an interval
    void deleteInterval(Node *currentNode, int low, int high)
    {
        //base case,if it is empty node, return
        if (currentNode->low == 0 && currentNode->high == 0) return;
        //if the interval to be deleted is found
        if (currentNode->low == low && currentNode->high == high)
        {
            // case 1:if the node has no children(left node) ,delete it (set low,high and max with 0)
            if (currentNode->leftNode->low == 0 && currentNode->leftNode->high == 0 &&
                currentNode->rightNode->low == 0 && currentNode->rightNode->high == 0)
            {
                currentNode->low = 0;
                currentNode->high = 0;
                currentNode->max = 0;
                return;
            }
            //case 2:if the node has no left child(has one right child) ,replace the current node with its right child and delete the right child
            else if (currentNode->leftNode->low == 0 && currentNode->leftNode->high == 0)
            {
                Node *temp = currentNode->rightNode;
                currentNode->low = temp->low;
                currentNode->high = temp->high;
                currentNode->max = temp->max;
                currentNode->leftNode = temp->leftNode;
                currentNode->rightNode = temp->rightNode;
                return;
            }
            //case 3:if the node has no right child(has one left child) ,replace the current node with its left child and delete the left child
            else if (currentNode->rightNode->low == 0 && currentNode->rightNode->high == 0)
            {
                Node *temp = currentNode->leftNode;
                currentNode->low = temp->low;
                currentNode->high = temp->high;
                currentNode->max = temp->max;
                currentNode->leftNode = temp->leftNode;
                currentNode->rightNode = temp->rightNode;
                return;
            }
            //case 3:if the node has two children, find its inorder successor and replace the node with it, then delete the inorder successor
            else
            {
                Node *temp = inOrderSuccessor(currentNode->rightNode);
                currentNode->low = temp->low;
                currentNode->high = temp->high;
                deleteInterval(currentNode->rightNode, temp->low, temp->high);
            }
        }
        //else: the interval is not found, then check if the current node's low is less than the interval's low ,then go to left
        else if (low < currentNode->low) deleteInterval(currentNode->leftNode, low, high);
        //else, go to right
        else deleteInterval(currentNode->rightNode, low, high);
    }

    //function searchInterval calls the overlap function until all the overlapping intervals are found
    void searchInterval(int low, int high)
    {
        deletedNodesCount = 0;
        deletedNodes = new int[1000000];
        while (overlap(low, high));
        //loop on the array of deleted nodes to insert them again in the tree to restore its status
        for (int i = 0; i < deletedNodesCount; i += 2) insertInterval(deletedNodes[i], deletedNodes[i + 1]);

    }

    // function to print the tree
    void print()
    {
        print(rootNode);
    }

    void print(Node *currentNode)
    {
        if (currentNode->low == 0 && currentNode->high == 0) return;
        print(currentNode->leftNode);
        cout << "["<<currentNode->low << "," << currentNode->high << "] max:" << currentNode->max << endl;
        print(currentNode->rightNode);
    }


    //destructor to delete the tree after the program terminates
    ~IntervalTree()
    {
        deleteTree(rootNode);
    }

    //function to delete the tree
    void deleteTree(Node *currentNode)
    {
        if (currentNode->leftNode == NULL && currentNode->rightNode == NULL)
        {
            delete currentNode;
            return;
        }
        deleteTree(currentNode->leftNode);
        deleteTree(currentNode->rightNode);
    }
};


int main()
{
    IntervalTree *tree1 = new IntervalTree();
    tree1->insertInterval(17, 19);
    tree1->insertInterval(5, 8);
    tree1->insertInterval(21, 24);
    tree1->insertInterval(4, 8);
    tree1->insertInterval(15, 18);
    tree1->insertInterval(7, 10);
    tree1->insertInterval(16, 22);
    cout<<"\nInorder Travarsal on tree1:\n";
    tree1->print();

    cout<<"\nquery1: [6,7]\n";
    tree1->searchInterval(6, 7);

    cout<<"\nquery2: [30,35]\n";
    tree1->searchInterval(30, 35);

    cout<<"\nquery3: [3,4]\n";
    tree1->searchInterval(3, 4);

    tree1->deleteInterval(5,8);
    cout<<"\ninterval [5,8] has been deleted\n";

    cout<<"\nquery4: [5,10]\n";
    tree1->searchInterval(5, 10);

    tree1->deleteInterval(16,22);
    cout<<"\ninterval [16,22] has been deleted\n";

    cout<<"\nquery5: [15,20]\n";
    tree1->searchInterval(15, 20);

    cout<<"\nquery6: [1,2]\n";
    tree1->searchInterval(1, 2);

    cout<<"\nquery7: [4,22]\n";
    tree1->searchInterval(4, 22);

    cout<<"\nquery8: [21,24]\n";
    tree1->searchInterval(21, 24);

    cout<<"\nquery9: [21,24]\n";
    tree1->searchInterval(21, 24);

    tree1->deleteInterval(17,19);
    cout<<"\ninterval [17,19] has been deleted\n";

    cout<<"\nquery10: [17,18]\n";
    tree1->searchInterval(17, 18);


    IntervalTree *tree2 = new IntervalTree();
    tree2->insertInterval(15,20);
    tree2->insertInterval(10,30);
    tree2->insertInterval(17,19);
    tree2->insertInterval(5,20);
    tree2->insertInterval(12,15);
    tree2->insertInterval(30,40);
    cout<<"\nInorder Travarsal on tree2:\n";
    tree2->print();

    cout<<"\nquery1: [6,7]\n";
    tree2->searchInterval(6, 7);

    cout<<"\nquery2: [10,12]\n";
    tree2->searchInterval(10, 12);

    cout<<"\nquery3: [0,4]\n";
    tree2->searchInterval(0, 4);

    cout<<"\nquery4: [40,50]\n";
    tree2->searchInterval(40, 50);

    tree2->deleteInterval(5,20);
    cout<<"\ninterval [5,20] has been deleted\n";

    cout<<"\nquery5: [40,50]\n";
    tree2->searchInterval(40, 50);

    cout<<"\nquery6: [4,5]\n";
    tree2->searchInterval(4, 5);

    tree2->deleteInterval(10,30);
    cout<<"\ninterval [10,30] has been deleted\n";

    cout<<"\nquery7: [10,10]\n";
    tree2->searchInterval(10, 10);

    cout<<"\nquery8: [17,17]\n";
    tree2->searchInterval(17, 17);

    tree2->deleteInterval(12,15);
    cout<<"\ninterval [12,15] has been deleted\n";

    cout<<"\nquery9: [11,16]\n";
    tree2->searchInterval(11, 16);

    cout<<"\nquery10: [0,0]\n";
    tree2->searchInterval(0, 0);

    IntervalTree *tree3 = new IntervalTree();
    tree3->insertInterval(17,19);
    tree3->insertInterval(5,11);
    tree3->insertInterval(22,23);
    tree3->insertInterval(4,8);
    tree3->insertInterval(15,18);
    cout<<"\nInorder Travarsal on tree3:\n";
    tree3->print();

    cout<<"\nquery1: [14,16]\n";
    tree3->searchInterval(14, 16);

    cout<<"\nquery2: [12,14]\n";
    tree3->searchInterval(12, 14);

    cout<<"\nquery3: [6,7]\n";
    tree3->searchInterval(6, 7);

    tree3->deleteInterval(22,23);
    cout<<"\ninterval [22,23] has been deleted\n";

    cout<<"\nquery4: [21,23]\n";
    tree3->searchInterval(21, 23);

    cout<<"\nquery5: [8,10]\n";
    tree3->searchInterval(8, 10);

    tree3->deleteInterval(15,18);
    cout<<"\ninterval [15,18] has been deleted\n";

    cout<<"\nquery6: [14,18]\n";
    tree3->searchInterval(14, 18);

    cout<<"\nquery7: [3,3]\n";
    tree3->searchInterval(3, 3);

    tree3->deleteInterval(4,8);
    cout<<"\ninterval [4,8] has been deleted\n";

    cout<<"\nquery8: [2,4]\n";
    tree3->searchInterval(2, 4);

    cout<<"\nquery9: [5,11]\n";
    tree3->searchInterval(5, 11);

    cout<<"\nquery10: [17,19]\n";
    tree3->searchInterval(17, 19);



    IntervalTree *tree4 = new IntervalTree();
    tree4->insertInterval(1, 4);
    tree4->insertInterval(12, 15);
    tree4->insertInterval(0, 3);
    tree4->insertInterval(2, 11);
    tree4->insertInterval(10, 16);
    tree4->insertInterval(14, 17);
    tree4->insertInterval(17, 22);
    cout<<"\nInorder Travarsal on tree4:\n";
    tree4->print();

    cout<<"\nquery1: [10,13]\n";
    tree4->searchInterval(10, 13);

    cout<<"\nquery2: [15,18]\n";
    tree4->searchInterval(15, 18);

    cout<<"\nquery3: [5,7]\n";
    tree4->searchInterval(5, 7);

    tree4->deleteInterval(2,11);
    cout<<"\ninterval [2,11] has been deleted\n";


    IntervalTree *tree5 = new IntervalTree();
    tree5->insertInterval(18, 22);
    tree5->insertInterval(4, 7);
    tree5->insertInterval(10, 15);
    tree5->insertInterval(19, 24);
    tree5->insertInterval(20, 27);
    tree5->insertInterval(8, 13);
    tree5->insertInterval(1, 4);
    cout<<"\nInorder Travarsal on tree5:\n";
    tree5->print();

    cout<<"\nquery1: [5,7]\n";
    tree5->searchInterval(5, 7);

    cout<<"\nquery2: [20,24]\n";
    tree5->searchInterval(20, 24);

    cout<<"\nquery3: [8,13]\n";
    tree5->searchInterval(8, 13);

    tree5->deleteInterval(1,4);
    cout<<"\ninterval [1,4] has been deleted\n";


    return 0;
}

