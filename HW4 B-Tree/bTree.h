#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>
#include <queue>
#include <cassert>
#include <map>
#include <cmath>
using namespace std;

//insert
  //Full and Nonfull
//traverse
  //Inorder traversal
struct node{
public:
  //pointer to an array of keys, which now become an array of key
  int* key;
  //double pointer to get the children of the node, which now become a array of child pointer to a node
  node** child;
  bool leaf;
  // number of keys in node
  int keyCount;
  //constructor
  node(int degree){
    //create an array of key of the degree
    //max is the degree, but this will rarely be the case as the tree will split if the node is full
    key = new int[degree];
    leaf = true;
    //same concept as array of keys, this time the array is holding child pointer to a node
    child = new node*[degree+1];
    //Set all child pointer to NULL
    for(int i = 0; i < degree; i++){
      child[i] = NULL;
    }
  }

  node* searchValidNode(node*temp, node* root, int data, int t);
  void split(node* temp, int t);
  //void fullCheck(node* temp, int t);
  void printLevel(node* root, int level, ofstream& fout);
  void height(node* temp, int height, int& result);
  void inorder(node* root);
  void inorder2(ofstream& fout);
};


/////////////////////////////////////////////////////////
class btree{
public:
  node* root;
  //degree t
  int t;

  btree(int degree){
    root = NULL;
    t = degree;
  }

  node* returnRoot(){
    return root;
  }

  //2 cases, if the root is NULL, insert by calling node contructor
  //If not, call searchNode from srtuct node
  void insert(int data){
    if(root == NULL){
      root = new node(t);
      root -> key[0] = data;
      root-> leaf = true;
      root-> keyCount = 1;
    }
    else{
      //this will search for a valid position and insert the data
      root = root ->searchValidNode(root, root, data, t);
    }
  }

  //Call struct node printLevel
  void printLevel(int level, ofstream &fout){
    root -> printLevel(root, level, fout);
  }

  //Call struct node inorder traversal
  void inorder2(ofstream& fout){
    root->inorder2(fout);
  }

  //call struct node height and return it
  int height(){
    int result;
    root -> height(root, 1, result);
    return result;
  }
};



/////////////////////////////////////////////////////////
node* node::searchValidNode(node* current, node* temp, int data, int t){
  //case 1: leaf node
  if(leaf){
    int index = t;
    //go through keys, find a non null value from the value before it
    while(!key[index-1]){
      index--;
    }
    //move right if data before key is greater
    while(key[index-1] > data && index != 0){
      key[index] = key[index-1];
      index--;
    }
    //insert and increase count
    key[index] = data;
    keyCount = keyCount +1;
  }
  //case 2: non leaf, recursive call until it is leaf
  else{
    int index = 0;
    //go to the appropriate child
    while(index < keyCount && data > key[index]){
      index++;      
    }
    child[index]->searchValidNode(this,temp,data,t);
  }

  //if the node is full after insertion, split it
  if(keyCount == t){
     //case 1: if root, make the old root the child of the new root node, then split the new root's child
    if(this == temp){
      node* n = new node(t);
      n->leaf = false;
      n->child[0] = this;
      n->split(this,t);
      return n;
    }
    //case 2: split the child of this root using another function
    else{
      current->split(this,t);
    }
  }
  return temp;
}

/////////////////////////////////////////////////////////
void node::split(node* full, int t){
  node* right = new node(t);
  //shift up using median value (t-1)/2 as index
  int indexRight = 0;
  //index for the data that to be shiftUp, the median value
  int shiftUp = (t-1)/2;
  int fullKeyCount = full->keyCount;
  //keys for the parent
  int parentData =  full->key[shiftUp];
  int indexChild = 0;
  //if the node has children while splitting
  //round up value using ceil
  float val1 = ceil(float(t)/2);
  int shiftDown = val1;

  //normal splitting
  for(int i = shiftUp+1; i < fullKeyCount; i++){
    //populated right with data from full node
    right -> key[indexRight] = full->key[i];
    indexRight++;
    //update key count
    full->keyCount = (full->keyCount)-1;
    right->keyCount = (right->keyCount)+1;
  }

  //if the node has children
  if(full->leaf == false){
    for(int a = shiftDown; a <= t; a++){
      //indexChild start from 0
      right->child[indexChild] = full->child[a];
      indexChild++;
    }
    //change the property of the node
    right->leaf = full->leaf;
  }


  //make space for data
  int transfer = t-1;
  while(child[transfer] != full){
    child[transfer+1] = child[transfer];
    transfer--;
  }
  child[transfer+1] = right;
  int j = t-1;
  while(!key[j-1] && j != 0){
    j--;
  }
  while(key[j-1] > parentData && j != 0){
    key[j] = key[j-1];
    j--;
  }

  //clean up
  key[j] = parentData;
  keyCount = keyCount +1;
  full->keyCount = (full->keyCount)-1; 
}



/////////////////////////////////////////////////////////
void node::printLevel(node* root,int level, ofstream& fout){
  if(level > 1 && leaf){
    return;
  }
  else if(level == 1){
    for(int i = 0; i < keyCount; i++){
      fout << key[i] << " ";
    }
  }

  else{
    for(int i = 0;  i <= keyCount; i++ ){
      child[i]->printLevel(root,level-1,fout);
    }
  }
}



/////////////////////////////////////////////////////////
void node:: height(node* temp, int h, int& result){
  if(leaf){
    result = h;
    return;
  }
  child[0]->height(temp,h+1,result);
}



/////////////////////////////////////////////////////////
void node::inorder(node* root){
  for(int i = 0; i < root->keyCount; ++i){
    inorder(root->child[i]);
    cout << root->key[i] << endl;
  }
  inorder(root->child[root->keyCount-1]);
}


void node::inorder2(ofstream& fout){
  int i;
  for(i = 0; i < keyCount; i++){
    if(leaf == false){
      child[i]->inorder2(fout);
    }
    fout << key[i] << " ";
  }
  if(leaf == false){
    child[i]->inorder2(fout);
  }
}
