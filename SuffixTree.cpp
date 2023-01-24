#include <iostream>

using namespace std;

// Class Node with the below 3 attributes and a constructor that initializes the attributes.
// 1) Array of pointers to children nodes. The array size equals to the alphabet size. (Method 1 in trie lecture)
// 2) The starting index (in the original string) of the characters on the edge going to this node.
// 3) The starting index (in the original string) of the suffix ending at this node

class Node
{
public:
    Node *pointers;
    int charactersIndex;
    int endingSuffixIndex;
public:
    Node()
    {
        charactersIndex = -1;
        endingSuffixIndex = -1;
    }

    Node(int size)
    {
        pointers = new Node[size];
        charactersIndex = -1;
        endingSuffixIndex = -1;
    }
};

// Class SuffixTree with the below attributes.
// 1) The tree construction time complexity must be O(n^2)
// 2) the tree space complexity must be O(n)
// 3) n is the string length.
// 4) The search complexity must be O(m+occ)
// 5) m is the length of the searched pattern
// 6) occ is the number of occurrences of the pattern inside the string.
// 7) Using edge structure is NOT allowed.
// 8) Do not use string class, use char arrays.
class SuffixTree
{
private:
    int wordSize;
    char *word;
    Node *rootNode;
public:
    SuffixTree(char *input)
    {
        //initialize the size to zero
        wordSize = 0;
        //Loop over the input to get the input size - will be stored in initialSize variable
        while (input[wordSize] != '$')
        {
            wordSize++;
        }
        //Store input in word array
        word = input;
        //Initialize rootNode with array of children of size of alphabet
        rootNode = new Node(27); // Alphabet size + 1 for $ sign
        //Call the function that builds the suffix tree
        buildSuffixTree();
    }

    void buildSuffixTree()
    {
        //Loop over the word and add each suffix to the tree
        for (int i = 0; i <= wordSize; i++)
        {
            //Call the function that adds the suffix to the tree
            addSuffix(i);
        }
    }

    int getIndex(char c)
    {
        //Get index of characters and map the $ symbol to the position 26
        if (c == '$')
        {
            return 26;
        }
        return c - 'a';
    }

    void addSuffix(int suffixIndex)
    {
        //Create a pointer to the root node
        Node *currentNode = rootNode;
        //Loop over the suffix
        for (int i = suffixIndex; i <= wordSize; i++)
        {
            //Case 1 - Root Doesn't contain this character -> insert directly               charactersIndex = -1 -> no in-edges going to this node
            if ((IsEmpty(&currentNode->pointers[getIndex(word[i])]) == true && currentNode->charactersIndex == -1))
            {
                //Create a new node
                Node *newNode = new Node(27);
                //Set the starting index of the characters on the edge going to this node
                newNode->charactersIndex = i;
                //Set the index of the suffix ending at this node
                newNode->endingSuffixIndex = suffixIndex;
                //Add the new node to the tree
                (currentNode->pointers[getIndex(word[i])]) = *newNode;

                //break don't add anything more
                break;
            }
            //Case 2 - Check if the current character is not in the tree                     && internal node
            else if (IsEmpty(&currentNode->pointers[getIndex(word[i])]) == true && currentNode->endingSuffixIndex == -1)
            {
                //Create new Node
                Node *newNode = new Node(27);
                newNode->charactersIndex = i;
                newNode->endingSuffixIndex = suffixIndex;
                currentNode->pointers[getIndex(word[i])] = *newNode;
                break;
            }
            //Case 3 - If the character exists then move to the new node and skip in the suffix index to the appropriate position
            else if (IsEmpty(&currentNode->pointers[getIndex(word[i])]) != true)
            {
                currentNode = &currentNode->pointers[getIndex(word[i])];
                int smallestIndex = wordSize;
                for (int j = 0; j < 27; j++)
                {
                    //Get the smallest index of the children of the node
                    if (IsEmpty(&currentNode->pointers[j]) == false)
                    {
                        if (currentNode->pointers[j].charactersIndex < smallestIndex)
                        {
                            smallestIndex = currentNode->pointers[j].charactersIndex;
                        }
                    }
                }

                //Get Length of characters that are on edge connected to this node'
                int length = smallestIndex - currentNode->charactersIndex;
                int counter = 0;

                //Loop on characters of currentNode edge  && compare with suffix
                for (int j = currentNode->charactersIndex; j < currentNode->charactersIndex + length; j++)
                {
                    if (word[j] == word[i])
                    {
                        i++;
                        counter++;
                    } else break;
                }

                i--;
                //Check if an intermediate node will be added or not
                if (length > counter)
                {
                    //Intermediate node "Split node at newCharIndex"
                    Node *newNode2 = new Node(27);
                    //Split position
                    int newCharIndex = currentNode->charactersIndex + counter;
                    newNode2->charactersIndex = newCharIndex;
                    newNode2->endingSuffixIndex = currentNode->endingSuffixIndex;

                    //Make current node a non leaf node
                    currentNode->endingSuffixIndex = -1;

                    //Move current node pointers to its new child
                    newNode2->pointers = currentNode->pointers;


                    //Reset current Node children pointer
                    currentNode->pointers = new Node[27];
                    //Add the new node th the current node
                    currentNode->pointers[getIndex(word[newCharIndex])] = *newNode2;
                }
            }
        }
    }

    //Pattern search function
    int Search(char *pattern)
    {
        int patternSize = 0;
        //Loop over the input to get the input size - will be stored in patternSize variable
        while (pattern[patternSize] != 0)
        {
            patternSize++;
        }

        //Pointer to root node
        Node *currentNode = rootNode;

        int i = 0;
        int found = 0;
        //Loop over each character on the pattern
        for (i = 0; i < patternSize; i++)
        {
            //if the node contains a pointer to a node that its edge starts with this character of the pattern
            if (IsEmpty(&currentNode->pointers[getIndex(pattern[i])]) != true)
            {
                //move the pointer to the new node
                currentNode = &currentNode->pointers[getIndex(pattern[i])];
                //mark this character of the pattern as matched
                found++;

                int smallestIndex = wordSize;
                //loop over the children of current node
                for (int j = 0; j < 27; j++)
                {
                    //Get the smallest index of the children of the node
                    if (IsEmpty(&currentNode->pointers[j]) == false)
                    {
                        //if the starting index is smaller than the current smallest Index we have
                        if (currentNode->pointers[j].charactersIndex < smallestIndex)
                        {
                            smallestIndex = currentNode->pointers[j].charactersIndex;
                        }
                    }
                }

                //Get Length of characters that are on edge connected to this node'
                int length = smallestIndex - currentNode->charactersIndex;
                int counter = 0;

                //Loop on characters of currentNode edge  && compare with suffix
                for (int j = currentNode->charactersIndex + 1; j < currentNode->charactersIndex + length; j++)
                {
                    //if there's a match
                    if (word[j] == pattern[found])
                    {
                        i++;
                        counter++;
                        found++;
                    } else break;
                }
            } else break;

        }
        //if we found all the characters of the pattern
        if (found == patternSize)
        {
            //print the indices of that pattern in the String
            printRoot(currentNode);
            return 0;
        }
    }

    bool IsEmpty(Node *currentNode)
    {
        //Check if the node is empty by checking charactersIndex "first index of the first character on the edge" && the suffix Index of the node
        if (currentNode->charactersIndex == -1 && currentNode->endingSuffixIndex == -1) return true;

        return false;

    }

    //Tree Destructor
    ~SuffixTree()
    {
        //Call the function that deletes the tree
        deleteSuffixTree(rootNode);
        delete rootNode;
    }

    //Delete function for suffix tree level by level recursively
    void deleteSuffixTree(Node *node)
    {
        //Loop over the children of the node
        for (int i = 0; i < 27; i++)
        {
            //If the child is not empty
            if (IsEmpty(&node->pointers[i]) == false)
            {
                //Call the function recursively on the child
                deleteSuffixTree(&node->pointers[i]);
            }
        }

        //All the Children of a node are recursively called -> delete this node pointers
        delete [] node->pointers;

    }

    void printRoot(Node *node)
    {
        //If the node is the last node in the structure then print this node only -> A leaf node can be treated as a root of an empty sub-tree
        if (IsEmpty(node) == false && node->endingSuffixIndex != -1)
        {
            cout << node->endingSuffixIndex << " ";
        }
        printSubTree(node);
        cout << endl;

    }

    void printSubTree(Node *node)
    {
        //Loop over the children of the node
        for (int i = 0; i < 27; i++)
        {
            //If the child is not empty
            if (IsEmpty(&node->pointers[i]) == false)
            {
                //if the non-empty child is a leaf
                if (node->pointers[i].endingSuffixIndex != -1)
                {
                    //print the suffix index of this leaf
                    cout << node->pointers[i].endingSuffixIndex << " ";
                }
                //Call the function recursively on the child
                printSubTree(&node->pointers[i]);

            }
        }
    }

};


int main()
{
    SuffixTree tree("bananabanaba$");
    tree.Search("naba");
    tree.Search("ana");
    tree.Search("bana");

    SuffixTree tree2("nabnanc$");
    tree2.Search("nab");
    tree2.Search("nc");
    tree2.Search("nanc");

    SuffixTree tree3("abcabxabcd$");
    tree3.Search("bca");
    tree3.Search("abc");
    tree3.Search("xab");

    SuffixTree tree4("kmankstst$");
    tree4.Search("man");
    tree4.Search("ks");
    tree4.Search("$");

    SuffixTree tree5("testatestalongtestletterstestcaseletterkrkrletter$");
    tree5.Search("test");
    tree5.Search("etter");
    tree5.Search("er");

    SuffixTree tree6("wearewearethewinners$");
    tree6.Search("we");
    tree6.Search("re");
    tree6.Search("win");

    SuffixTree tree7("ac$");
    tree7.Search("a");
    tree7.Search("c");
    tree7.Search("k");

    SuffixTree tree8("suffixsuffixingsuffer$");
    tree8.Search("suff");
    tree8.Search("suffer");
    tree8.Search("suffixing");


    SuffixTree tree9("a$");
    tree9.Search("a");
    tree9.Search("$");
    tree9.Search("k");

    SuffixTree tree10("bingbinbingingbning$");
    tree10.Search("in");
    tree10.Search("ning");
    tree10.Search("bin");


    return 0;
}
