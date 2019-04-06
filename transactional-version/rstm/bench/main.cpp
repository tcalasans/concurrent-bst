#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <api/api.hpp>
#include <common/platform.hpp>
#include <common/locks.hpp>
#include "bmconfig.hpp"
#include "../include/api/api.hpp"




#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <thread>



using namespace std;

const int THREAD_COUNT = 4;
const int NUM_TRANSACTIONS = 100000;

// shared variable that will be incremented by transactions
int x = 0;



















#define SILENT


#define synchronized(m) \
    m.lock(); for(int ___ct=0 ; ___ct++<1 ; m.unlock() )
using namespace std;


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

/*
        String toString() {
            String delimiter = "  ";
            //StringBuilder sb = new StringBuilder();
            return sb.append(value + (marked? "(marked)" : "") + delimiter).toString();
        }
*/
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
          //  node->m_mutex.lock();
             //printf("adding inside add before mutex \n");
             //fflush(stdout);
            TM_BEGIN(atomic) {
//synchronized(node->m_mutex) {

                if ( TM_READ(node->marked) || (leftLast &&  TM_READ(node->left) != NULL) || (!leftLast && node->right != NULL)) {
                    //node->m_mutex.unlock();
                    stm::restart();
                    continue;
                }
                PaVTNode* upperNode = leftLast? (PaVTNode*) node->leftSnapshot : (PaVTNode*) node->rightSnapshot;
                if ((leftLast && (val <= upperNode->value)) ||
                        (!leftLast && (val >= upperNode->value)
                                )) {
                    
                    stm::restart();
                    //node->m_mutex.unlock();
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

           }TM_END;

         //   node->m_mutex.unlock();
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






class TestCase{



public:
    int n;
    PaVTBST* tree = new PaVTBST(-1500000000,1500000000);
 //   Dummy* tree = new Dummy();
    int* array ;
    int nThreads;
    TestCase(int n, int nThreads ){
        this->n = n ;
        this->nThreads = nThreads;
        array = new int[n];
        srand (time(NULL));
        populateArray();
    }

    ~TestCase(){
        tree->deleteNodes();
        delete array;


    }


    void populateArray(){

        for( int i = 0; i < n; i++ ){
            array[i] = i;

            //printf("%d, ", array[i]);

        }
      //  int k = 0 ;
        for( int i = 0; i < n; i++ ){

            int tmp = array[i];
            int r = rand()%n;
            array[i] = array[r];
            array[r] = tmp;

          }

    //for( int i = 0; i < n; i++ ){

      //     printf("%d, ", array[i]);

         // }


           //printf("\n");
    }
    void adder(PaVTBST* tree ,int* array, int begin, int end){
        // printf("%d, %d\n", begin, end);
    
        // init thread for rstm
        TM_THREAD_INIT();

        while(begin < end) {
            printlocked("adding\n");
            tree->add(array[begin]);

            begin++;
        }
    }
    void remover(PaVTBST* tree ,int* array, int begin, int end){
         //printf("%d, %d\n", begin, end);

        while(begin < end) {

            printlocked("removing\n");

            while(tree->remove(array[begin])==tree->invalidNumber)
                ;

            begin++;
        }
    }
    void run(){


        thread workersAdd[nThreads];
        int size = n/nThreads;
        for (int i=0; i<nThreads; i++) {

            workersAdd[i] = thread(&TestCase::adder, this, tree , array, i*size, (i+1)*size);
        }

        //tree->inorderPrint();

        //printf("size: %d\n",tree->size()) ;


        thread workersRemove[nThreads];

        for (int i=0; i<nThreads; i++) {

            workersRemove[i] = thread(&TestCase::remover, this, tree , array, i*size, (i+1)*size);
        }
        for (int i=0; i<nThreads; i++){
            workersAdd[i].join();
        }


        for (int i=0; i<nThreads; i++){
            workersRemove[i].join();
        }

        tree->inorderPrint();
        printf("size: %d\n",tree->size()) ;




    }//run






};





















Config::Config() :
    bmname(""),
    duration(1),
    execute(0),
    threads(THREAD_COUNT),
    nops_after_tx(0),
    elements(256),
    lookpct(34),
    inspct(66),
    sets(1),
    ops(1),
    time(0),
    running(true),
    txcount(0)
{
}

Config CFG TM_ALIGN(64);

/*
void* run_thread(void* i) {
    // each thread must be initialized before running transactions
    TM_THREAD_INIT();

    for(int i=0; i<NUM_TRANSACTIONS; i++) {
        // mark the beginning of a transaction
        TM_BEGIN(atomic)
        {
            // add this memory location to the read set
            int z = TM_READ(x);
            // add this memory location to the write set
            TM_WRITE(x, z+1);
        }
        TM_END; // mark the end of the transaction
    }

    TM_THREAD_SHUTDOWN();
}

*/
/*
int main(int argc, char** argv) {
    printf("hello world\n");
    TM_SYS_INIT();

    // original thread must be initalized also
    TM_THREAD_INIT();

    void* args[256];
    pthread_t tid[256];

    // set up configuration structs for the threads we'll create
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    for (uint32_t i = 0; i < CFG.threads; i++)
        args[i] = (void*)i;

    // actually create the threads
    for (uint32_t j = 1; j < CFG.threads; j++)
        pthread_create(&tid[j], &attr, &run_thread, args[j]);

    // all of the other threads should be queued up, waiting to run the
    // benchmark, but they can't until this thread starts the benchmark
    // too...
    run_thread(args[0]);

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    for (uint32_t k = 1; k < CFG.threads; k++)
        pthread_join(tid[k], NULL);

    // And call sys shutdown stuff
    TM_SYS_SHUTDOWN();

    printf("x = %d\n", x); // x should equal (THREAD_COUNT * NUM_TRANSACTIONS)

    return 0;
}*/

int main(int agrc, char**argv){
    TM_SYS_INIT();

    // original thread must be initalized also
    TM_THREAD_INIT();

    int i = 10;

   while(i<=64){
        int elements = 100000*i;
        printf("Test with %d threads - %d\n", i, elements );
        TestCase test = TestCase(elements,i);
        test.run();

        i++;
    }

    return 0;
}
