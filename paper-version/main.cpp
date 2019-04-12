//g++ *.cpp -std=c++11-lpthread

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <thread>
#include <fstream>
#include <chrono> 
#include <list> 
#include <iterator> 


#include <time.h>

#define SILENT


using namespace std;
using namespace std::chrono; 


mutex printlock;


 void printlocked(const char* s){
    #ifndef SILENT
    printf("one is defined ");
    printlock.lock();
    printf(s);
    printlock.unlock();
    #endif

    }



 class PaVTNode{

    public:

        int value;
        //public final Object item;
        volatile PaVTNode* leftSnapshot = NULL;
        volatile PaVTNode* rightSnapshot = NULL;

        volatile bool marked = false;

         PaVTNode* parent = NULL;
         volatile PaVTNode* right=NULL;
         volatile PaVTNode* left=NULL;




         recursive_mutex m_mutex;




        PaVTNode(int val) {
            value = val;
            //this.item = item;
            //this.marked = false;
        }

       PaVTNode(int val, PaVTNode* parent,
                PaVTNode* leftSnapshot, PaVTNode* rightSnapshot) {
            value = val;
            this->parent = parent;
            this->leftSnapshot = leftSnapshot;
            this->rightSnapshot = rightSnapshot;
        }
    };//PaVTNode




class PaVTBST {


private:
    PaVTNode* rightSentinel;
    PaVTNode* leftSentinel;



public:
        int invalidNumber;
    PaVTNode* root;


    PaVTBST(int min, int max) {
        invalidNumber=min-1;
        this->rightSentinel = new PaVTNode(min);
        this->leftSentinel = new PaVTNode(max);


        this->leftSentinel->parent = rightSentinel;
        this->rightSentinel->right = leftSentinel;
        this->leftSentinel->leftSnapshot = rightSentinel;
        this->rightSentinel->rightSnapshot = leftSentinel;
        this->root = leftSentinel;
    }



    int add(int val) {
        //printf("adding inside add 1 \n");
        //fflush(stdout);
        while (true) {
             PaVTNode* node = root;
             PaVTNode* child = NULL;
             int res = -1;
            while (true) {
                if (val == node->value){
                    res = 0;
                    break;
                }
                if (val < node->value) {
                    res = -1;
                    child = (PaVTNode*) node->left;
                } else {
                    res = 1;
                    child = (PaVTNode*) node->right;
                }
                if (child == NULL) {
                    break;
                }
                node = child;

            }
            if (val == node->value) {
                return val;
            }
            bool leftLast = val < node->value ;
          
            node->m_mutex.lock();

            if (node->marked || (leftLast && node->left != NULL) || (!leftLast && node->right != NULL)) {
                node->m_mutex.unlock();
                continue;
            }
            PaVTNode* upperNode = leftLast? (PaVTNode*) node->leftSnapshot : (PaVTNode*) node->rightSnapshot;
            if ((leftLast && (val <= upperNode->value)) ||
                    (!leftLast && (val >= upperNode->value)
                            )) {
                node->m_mutex.unlock();
                continue;
            }
            PaVTNode* newNode = new PaVTNode(val, node, val > node->value ? node : upperNode, res > 0? upperNode : node);

            if (!leftLast) {
                upperNode->leftSnapshot = newNode;
                node->rightSnapshot = newNode;
                node->right = newNode;
                node->m_mutex.unlock();
                return invalidNumber;
            }
            upperNode->rightSnapshot = newNode;
            node->leftSnapshot = newNode;
            node->left = newNode;
            
            node->m_mutex.unlock();
            return invalidNumber;
           
        }
    }


    void inorderPrint(){

        if(this->root->left==NULL){
          printf("[ empty ] \n");
        }
        inorderPrintRec( (PaVTNode*) this->root->left);
        printf("\n" );


    }



    void inorderPrintRec(PaVTNode* root){

        if(root==NULL) {

            return;
        }
        inorderPrintRec((PaVTNode*) root->left);
        printf("%d ",root->value);
        inorderPrintRec((PaVTNode*) root->right);



    }





    int remove(int val) {

        while (true) {
            PaVTNode* node = root;
            PaVTNode* leftNode = leftSentinel;
            PaVTNode* rightNode = rightSentinel;
            PaVTNode* child = NULL;
            int res = -1;
            while (true) {
                if (val == node->value){
                    res = 0;
                    break;
                }
                if (val < node->value){
                    leftNode = node;
                    child = (PaVTNode*) node->left;
                    res = -1;
                } else {
                    rightNode = node;
                    child = (PaVTNode*) node->right;
                    res = 1;
                }
                if (child == NULL) {
                    break;
                }
                node = child;
            }
            if (res != 0) {
                bool leftLast = res < 0;
                PaVTNode* ref = leftLast? (PaVTNode*) node->leftSnapshot: (PaVTNode*) node->rightSnapshot;
                if (
                        ( leftLast && val <= ref->value ) ||
                        ( !leftLast && val >= ref->value ) ) {
                    continue;
                }
                return this->invalidNumber;
            }
            PaVTNode* parent = node->parent;

            printlocked("Acquiring parent lock \n");

            parent->m_mutex.lock();
            printlocked("parent lock acquired \n");
                if (node->parent != parent) {
                    if (node->marked) return this->invalidNumber;
                    parent->m_mutex.unlock();
                    continue;
                }

                printlocked("Acquiring node lock \n ");

                node->m_mutex.lock();
                printlocked("node lock acquired \n");
                    if (node->marked) {

                        //unlock
                        parent->m_mutex.unlock();
                        node->m_mutex.unlock();


                        return this->invalidNumber;
                    }
                    PaVTNode* left = (PaVTNode*) node->left;
                    PaVTNode* right = (PaVTNode*)  node->right;
                    bool leftChild = parent->left == node;
                    if (left == NULL && right == NULL) {
                        rightNode = (PaVTNode*) node->leftSnapshot;
                        leftNode = (PaVTNode*) node->rightSnapshot;
                        node->marked = true;
                        if (leftChild) {
                            parent->left = NULL;
                            parent->leftSnapshot = (PaVTNode*)  rightNode;
                            rightNode->rightSnapshot = (PaVTNode*)  parent;
                        } else {
                            parent->right = NULL;
                            parent->rightSnapshot = (PaVTNode*)  leftNode;
                            leftNode->leftSnapshot = (PaVTNode*)  parent;
                        }
                    } else if (left == NULL || right == NULL) {
                        child = left == NULL? (PaVTNode*) right : (PaVTNode*)  left;
                        rightNode = (PaVTNode*) node->leftSnapshot;
                        leftNode = (PaVTNode*)  node->rightSnapshot;
                      //  synchronized(child) {

                        printlocked("Acquiring child lock \n ");

                        child->m_mutex.lock();
                            PaVTNode* snapshotToLock = left == NULL? (PaVTNode*)  leftNode : (PaVTNode*) rightNode;
                            //synchronized(snapshotToLock) {

                            printlocked("Acquiring snapshotToLock lock \n ");

                            snapshotToLock->m_mutex.lock();
                                if ((left == NULL && snapshotToLock->leftSnapshot != node) ||
                                        (left != NULL && snapshotToLock->rightSnapshot != node) ||
                                        snapshotToLock->marked) {
                                        //UNLOCKING

                                        snapshotToLock->m_mutex.unlock();
                                        child->m_mutex.unlock();
                                        node->m_mutex.unlock();
                                        parent->m_mutex.unlock();


                                        continue;
                                }
                                node->marked = true;
                                child = left == NULL? right : left;
                                if (leftChild) {
                                    parent->left = (PaVTNode*)  child;
                                } else {
                                    parent->right = (PaVTNode*)  child;
                                }
                                child->parent = (PaVTNode*) parent;
                                rightNode->rightSnapshot = (PaVTNode*)  leftNode;
                                leftNode->leftSnapshot = (PaVTNode*)  rightNode;
                            snapshotToLock->m_mutex.unlock();
                            //} syn snapshotToLock
                        child->m_mutex.unlock();
                        //}// syn child
                    } else {
                       // synchronized(left) {
                            left->m_mutex.lock();
                            //synchronized(right) {
                                right->m_mutex.lock();
                                rightNode = (PaVTNode*) node->leftSnapshot;
                                leftNode = (PaVTNode*) node->rightSnapshot;
                               // synchronized(rightNode) {
                                    rightNode->m_mutex.lock();
                                    if (rightNode->rightSnapshot != node || rightNode->marked){


                                        rightNode->m_mutex.unlock();
                                        right->m_mutex.unlock();
                                        left->m_mutex.unlock();
                                        node->m_mutex.unlock();
                                        parent->m_mutex.unlock();




                                        continue;
                                    }
                                    if (right->left == NULL) {
                                        node->marked = true;
                                        right->left = (PaVTNode*) left;
                                        left->parent = (PaVTNode*) right;
                                        right->parent = (PaVTNode*) parent;
                                        if (leftChild) {
                                            parent->left = (PaVTNode*) right;
                                        } else {
                                            parent->right = (PaVTNode*) right;
                                        }
                                        rightNode->rightSnapshot = (PaVTNode*) leftNode;
                                        leftNode->leftSnapshot = (PaVTNode*) rightNode;
                                    } else {
                                        PaVTNode* succ = (PaVTNode*) leftNode;
                                        PaVTNode* succParent = (PaVTNode*) succ->parent;
                                        //synchronized(succParent) {
                                        succParent->m_mutex.lock();
                                            if (leftNode->parent != succParent || leftNode->marked){


                                                succParent->m_mutex.unlock();
                                                rightNode->m_mutex.unlock();
                                                right->m_mutex.unlock();
                                                left->m_mutex.unlock();
                                                node->m_mutex.unlock();
                                                parent->m_mutex.unlock();




                                                continue;
                                            }
                                            //synchronized(leftNode) {
                                             leftNode->m_mutex.lock();
                                                if (leftNode->leftSnapshot != node || leftNode->marked){

                                                leftNode->m_mutex.unlock();
                                                succParent->m_mutex.unlock();
                                                rightNode->m_mutex.unlock();
                                                right->m_mutex.unlock();
                                                left->m_mutex.unlock();
                                                node->m_mutex.unlock();
                                                parent->m_mutex.unlock();





                                                continue;
                                            }
                                                PaVTNode* succRight = (PaVTNode*) succ->right;
                                                if (succRight != NULL ) {
                                                   // synchronized(succRight) {
                                                    succRight->m_mutex.lock();
                                                        PaVTNode* succRightSnapshot = (PaVTNode*) succ->rightSnapshot;
                                                        if (succRightSnapshot != succRight) {
                                                            //synchronized(succRightSnapshot) {
                                                            succRightSnapshot->m_mutex.lock();
                                                                if (succRightSnapshot->leftSnapshot != succ || succRightSnapshot->marked) {



                                                                        succRightSnapshot->m_mutex.unlock();
                                                                        succRight->m_mutex.unlock();
                                                                        leftNode->m_mutex.unlock();
                                                                        succParent->m_mutex.unlock();
                                                                        rightNode->m_mutex.unlock();
                                                                        right->m_mutex.unlock();
                                                                        left->m_mutex.unlock();
                                                                        node->m_mutex.unlock();
                                                                        parent->m_mutex.unlock();



                                                                        continue;
                                                                     }
                                                                applyRemove(rightNode, node, parent, left, right,
                                                                        leftChild, succ, succParent, succRight, succRightSnapshot);
                                                                succRightSnapshot->m_mutex.unlock();
                                                           // } syn succRightSnapshot
                                                        } else {
                                                            applyRemove(rightNode, node, parent, left, right,
                                                                    leftChild, succ, succParent, succRight, succRightSnapshot);
                                                        }
                                                    succRight->m_mutex.unlock();
                                                    //} syn succcRight
                                                 } else {
                                                    PaVTNode* succRightSnapshot = (PaVTNode*) succ->rightSnapshot;
                                                    applyRemove(rightNode, node, parent, left, right,
                                                            leftChild, succ, succParent, succRight, succRightSnapshot);
                                                }
                                                leftNode->m_mutex.unlock();
                                            //} syn leftNode
                                        succParent->m_mutex.unlock();
                                       // } syn succParent
                                    }
                                    rightNode->m_mutex.unlock();
                                //} syn rightNode
                                right->m_mutex.unlock();
                            // syn right}
                            left->m_mutex.unlock();
                        // } syn left
                    }
                    node->m_mutex.unlock();
                //}// syn second
                parent->m_mutex.unlock();
            //}//syn first
            return  node->value;
        }
    }



    void applyRemove(PaVTNode* rightNode,
            PaVTNode* node,
            PaVTNode* parent,
            PaVTNode* left,
            PaVTNode* right, bool leftChild,
            PaVTNode* succ,
            PaVTNode* succParent,
            PaVTNode* succRight,
            PaVTNode* succRightSnapshot) {
        node->marked = true;
        succ->right = right;
        right->parent = succ;
        succ->left = left;
        left->parent = succ;
        succ->parent = parent;
        if (leftChild) {
            parent->left = succ;
        } else {
            parent->right = succ;
        }
        succParent->left = succRight;
        succ->rightSnapshot = succRightSnapshot;
        succRightSnapshot->leftSnapshot = succ;
        if (succRight != NULL) {
            succRight->parent = succParent;
        }
        succ->leftSnapshot = rightNode;
        rightNode->rightSnapshot = succ;
    }



    void deleteNodes(){
        deleteNodes(this->root->left);
    }
    void deleteNodes(volatile PaVTNode* root){
        if(root == NULL) return;
        deleteNodes(root->left);
        deleteNodes(root->right);
        delete root;
    }

    bool contains(int val) {

        while (true) {
            PaVTNode* node = root;
            PaVTNode* child = NULL;
            int res = -1;
            while (true) {
                if (val == node->value){
                    res = 0;
                    break;
                }
                if (val < node->value) {
                    child = (PaVTNode*) node->left;
                    res = -1;
                } else {
                    child = (PaVTNode*) node->right;
                    res = 1;
                }
                if (child == NULL) {
                    break;
                }
                node = child;

            }
            if (res == 0) {
                return true;
            }
            PaVTNode* upperNode = res < 0? (PaVTNode*) node->leftSnapshot : (PaVTNode*) node->rightSnapshot;

            if (
                    (res < 0 && val <= upperNode->value ) ||
                    (res > 0 && val >= upperNode->value )
                    ) {
                continue;
            }

            return false;
        }
    }




    int size() {
        PaVTNode* n = (PaVTNode*) root->left;
        return size(n);
    }

    int size(PaVTNode* n) {
        if (n == NULL) return 0;
        return 1 + size((PaVTNode*) n->left) +  size((PaVTNode*) n->right);
    }


};//PaVTree


class Dummy{



public:
    void add( int val ) {


        printf("dummy %d \n", val);




    }



};


class TestCase
{

  public:
    int n;
    PaVTBST* tree = new PaVTBST(-1500000000,1500000000);

    //   Dummy* tree = new Dummy();
    int *array;
    int nThreads;
    int percAdd, percRemove, percContains;
    int transactionSize;
    char *filename;

    struct op {
        char type; // C - contains/ R- remove/ A - add
        int arg; // value to be added or removed
    };

    list<op*> *createOpList(int numOperations) {
        list<op*> *newList = new list<op*>();

        for (int i = 0; i < numOperations; i++) {
            newList->push_back(createOpRandomly());
        }

        return newList;
    } 
    op* createOpRandomly() {
        int randNum = rand() % 100;

        char operation = 'A';
        if (randNum < percAdd) {
            operation = 'A';
        }
        else if (randNum < percAdd + percContains) {
            operation = 'C';
        }
        else {
            operation = 'R';

        }
        
        op* o = new op {
            operation,
            rand()%n
        };
        
        return o;
    }

    TestCase(char* filename, int numElements, int nThreads, int percAdd, int percRemove, int percContains, int transactionSize)
    {
        this->filename = filename;
        this->n = numElements;
        this->nThreads = nThreads;
        this->percAdd = percAdd;
        this->percRemove = percRemove;
        this->percContains = percContains;

        this->transactionSize = transactionSize;
        array = new int[n];
        srand(time(NULL));
        populateArray();
        
    }

    void run2() {
        thread workers[nThreads];
        int size = n / nThreads;
        
        list<op*>* opList[nThreads];
        for(int i =0 ;i < nThreads; i++) {
            opList[i] = createOpList(size);
        }
        
        auto start = high_resolution_clock::now(); 

        for(int i = 0 ;i < nThreads; i++) {
            workers[i] = thread(&TestCase::work, this, tree, opList[i]);
        }
        
        for(int i = 0; i < nThreads; i++) {
            workers[i].join();
         }
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 


        //printf("Tree size: %d\n", tree->size());

        outputToFile(filename, duration.count(),
                     this->n, 
                     this->nThreads,
                     this->percAdd,
                     this->percRemove,
                     this->percContains, 
                     1);
    }
    void populateArray()
    {

        for (int i = 0; i < n; i++)
        {
            array[i] = i;

            //printf("%d, ", array[i]);
        }
        
        for (int i = 0; i < n; i++)
        {

            int tmp = array[i];
            int r = rand() % n;
            array[i] = array[r];
            array[r] = tmp;
        }
    }

    void populateTree() {
        for (int i = 0; i < n/2; i++) {
            tree->add(rand()%n);
        }
    }

    void printList(list<op> *listOps) {
        for(list<op>::iterator it = listOps->begin(); it != listOps->end(); ++it ) {
            printf("%c - %d, ", it->type, it->arg);
        }

        printf("\n");
    }
    void work(PaVTBST *tree, list<op*> *listOps) {
        //printList(listOps);
        
        
        for(list<op*>::iterator it = listOps->begin(); it != listOps->end(); ++it ) {
            //printf("%p tree \n", tree);
            //printf("%c - %d, ", it->type, it->arg);
            op *operaton = *it;
            if (operaton->type == 'A') {
                tree->add(operaton->arg);
 
            }
            else if (operaton->type == 'R') {
                tree->remove(operaton->arg); 
            }
            else { //contains
                tree->contains(operaton->arg);
            }
        }
        

        
    }

    void outputToFile(char* filename, int64_t duration,
                        int elements, int nThreads,
                        int percAdd,
                        int percRemove,
                        int percContains, 
                        int transactionSize) {
        char *delimiter = ", ";

        ofstream outfile;
        outfile.open(filename, ios_base::app);
        outfile << duration << delimiter;
        outfile << elements << delimiter;
        outfile << nThreads << delimiter;
        
        outfile << percAdd << delimiter;
        outfile << percRemove << delimiter;
        outfile << percContains<< delimiter;

        
        outfile << transactionSize << delimiter;

        outfile << endl;
    }
};






int main(int argc, char **argv)
{
    if (argc < 8) {
        printf("insufficient arguments\n");
        return 1;
    }
    
    // num elements
    int numElements = atoi(argv[1]);
    // num threads
    int numThreads = atoi(argv[2]);
    // percAdd
    int percAdd = atoi(argv[3]);
    //percRemove
    int percRemove = atoi(argv[4]);
    // percContains
    int percContains = atoi(argv[5]);
    //trans size
    int transactionSize = atoi(argv[6]);
    //output filename
    char *fname = argv[7];

    TestCase test = TestCase(fname, numElements, numThreads, percAdd, percRemove, percContains, transactionSize);
    test.run2();

    return 0;
}

