#include <iostream>

#include<stdio.h>

using namespace std;

struct node
{
  int key_value;
  node *left;
  node *right;
};


class btree
{
    public:
        btree();
        ~btree();

        void insert(int key);
        node *search(int key);
        void destroy_tree();


        void inorderPrintRec(node* root){

                if(root==NULL) {

                return;
                }
                inorderPrintRec( root->left);
                printf("%d ",root->key_value);
                inorderPrintRec(root->right);



        }


        void inorderPrint(){

                if(this->root==NULL){
                    printf("[ empty ] \n");
                }
                inorderPrintRec(this->root);
                printf("\n" );


        }



        node* remove(int key, node *leaf)

        {
       // printf("calling remove");

        if(leaf==NULL){

          // printf("leaf==null");
            return leaf;
        }

        if( key < leaf->key_value ){ //search to left
                //printf("keay<leaf key val");
                leaf->left = remove(key,leaf->left);
                return leaf;
        }
        if( key > leaf->key_value ){ //search to right
            printf("keay>leaf key val");
            leaf->right = remove(key,leaf->right);
            return leaf;
        }

        //no child case
        if( leaf->left == NULL && leaf->right==NULL ){
           //printf("no child case");

            return NULL;
        }

        //one child case
        if( leaf->left==NULL ){
            printf("one child case left");
            return leaf->right;
        }

        if( leaf->right==NULL ){
            printf("one child case right");
            return leaf->left;


        }


        //two child removal case 1
        if( leaf->right->left== NULL ){
            printf("case1");
            leaf->key_value=leaf->right->key_value;
            leaf->right=leaf->right->right;
            return leaf;
        }

         //two child removal case 2
        //printf("case2");
        node* parent = leaf->right;
        node* child  = leaf->right->left;

        while( child->left!= NULL ){
            //printf("case2");
            parent = child;
            child = child->left;
        }

        //printf("Cambio puntatori\n");
        leaf->key_value=child->key_value;
        //printf("Faccio puntare %d  ", parent->left->key_value);
        //printf("%d",child->right->key_value);
        parent->left = child->right;
        return leaf;





        }


        void remove(int key)
        {

            remove(key,root);
        }




    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);

        node *root;
};



btree::btree()
{
  root=NULL;
}


btree::~btree()
{
  destroy_tree();
}

void btree::destroy_tree()
{
  destroy_tree(root);
}




void btree::insert(int key, node *leaf)
{
  if(key< leaf->key_value)
  {
    if(leaf->left!=NULL)
     insert(key, leaf->left);
    else
    {
      leaf->left=new node;
      leaf->left->key_value=key;
      leaf->left->left=NULL;    //Sets the left child of the child node to null
      leaf->left->right=NULL;   //Sets the right child of the child node to null
    }
  }
  else if(key > leaf->key_value)
  {
    if(leaf->right!=NULL)
      insert(key, leaf->right);
    else
    {
      leaf->right=new node;
      leaf->right->key_value=key;
      leaf->right->left=NULL;  //Sets the left child of the child node to null
      leaf->right->right=NULL; //Sets the right child of the child node to null
    }
  }
}

void btree::insert(int key)
{
  if(root!=NULL)
    insert(key, root);
  else
  {
    root=new node;
    root->key_value=key;
    root->left=NULL;
    root->right=NULL;
  }
}






//deletion bottom up
void btree::destroy_tree(node *leaf)
{
  if(leaf!=NULL)
  {
    destroy_tree(leaf->left);
    destroy_tree(leaf->right);
    delete leaf;
  }
}






int main()
{



    btree* tree = new btree();
    tree->insert(4);
    tree->insert(6);
    tree->insert(3);
    //tree->insert(7);
    //tree->insert(12);
    //tree->insert(11);
    //tree->insert(7);
    //tree->insert(12);
    tree->insert(5);
     tree->insert(8);
      tree->insert(12);

    printf("\n");

    tree->inorderPrint();

   // tree->remove(3);

    printf("\n");

    tree->inorderPrint();

    tree->remove(4);

    //tree->remove(2);
    // tree->remove(2);
    printf("\n");

    tree->inorderPrint();




    tree->destroy_tree();




    printf("\nEND");

    return 0;
}
