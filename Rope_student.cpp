/*
Write your Rope implementation in this file. 
You do not have to keep any of the code that is already in this file.
Functions that are required that you have not implemented have an assert 0
meaning they will always crash. This should help you make sure you implement
everything.
*/

#include <cassert>
#include "Rope.h"

//Should advance to the next Node using in-order traversal
//It can point at any Node, not just leaves
// in-order traversal
// if leaf, find prior unvisited parent 
//     (update visited with prior unvisited parent and return prior unvisited_parent)
// if not leaf
//   get farthest unvisited left (which could be the leaf)
//     (update visited with farthest unvisited left and return farthest unvisited_left)
// if no farthest unvisited left set to nearest unvisited right
//   now get farthest unvisited left
//       (update visited and return)
rope_iterator& rope_iterator::operator++(){

	// if NULL ptr return NULL
	if (!ptr_)
    	*this = NULL;

    //check if we can go to an unvisited left
    if(ptr_->left && visited.count(ptr_->left)==0)              //find next unvisited left
    	ptr_ = ptr_->left;
    else if (ptr_->right && visited.count(ptr_->right)==0)    //else next unvisited right
        ptr_ = ptr_->right;
    else if (ptr_->parent)
        ptr_ = ptr_->parent; //else go back to parent
    else{
        *this = rope_iterator(NULL);
        return *this;
    }

    while (ptr_ && ptr_->left && visited.count(ptr_->left)==0){    // attempt to go as much left as possible
        ptr_ = ptr_->left;
    }

    // at this point, have a valid farthest left unvisited ptr
    if (ptr_)
        visited.insert({ptr_, ptr_});                            // valid ptr_ visited
    return *this;

	//assert(0 && "rope_iterator operator++ not implemented!");
}

//Point to the first Node for in-order traversal
rope_iterator Rope::begin() const{
	if (!root)
       return rope_iterator(NULL);
 	Node* p = this->root;
  
	//the first node for inorder traversal is 
	// the farthest node left from root
 	if (!p)
 		return rope_iterator(p);
	if (p->left)
		p = p->left;
	else if(p->right)
		p = p->right;

	// loop until you get to the farthest left node
	while (p->left)
		p = p->left;

	// initiate visited map to track nodes visited
	rope_iterator rit = rope_iterator(p);
	rit.visited.clear();
	rit.visited.insert({p,p});
	return rit;
	//assert(0 && "rope_iterator begin() not implemented!");
}

Rope::Rope(){
	size_ = 0;
	root = NULL;
	//assert(0 && "Rope default constructor not implemented!");
}

//Should make the root = p and adjust any member variables
//This should not make a copy of the rope with root at p,
//it should just "steal" the Node*
Rope::Rope(Node* p){
	root = p;
	root->parent = NULL;
	size_ = RopeSize();
	//assert(0 && "Rope constructor from Node not implemented!");
}

int Rope::RopeSize(){
	int rs = 0;				// rope size

	rope_iterator it = begin();
	for(; it != end(); it++){
		if (is_leaf(it.ptr_))
			rs = rs + it.ptr_->weight;
	}
	//return size for rope
	return rs;
}

Rope::~Rope(){
	destroy_rope();
    size_=0;
	//assert(0 && "Rope destructor not implemented!");
}

void Rope::destroy_rope() { 

	// starting from the root go down to the farthest left leaf
	// disconnect it from the rope and delete the node
	// if we dont have the farthest leaf left node
	// go to the immediate right of the farthest left node
	// from this right node, traverse to the farthest left
	// repeat all the above steps in a while loop until we get to the root node
	// once root node is deleted, rope is empty, break
	Node* p = root;
    if (!p)
        return;
    while (p){
    	if (p->left)
            p = p->left;
        else if(p->right)
            p = p->right;
        else if (!p->parent){
            delete p;
            break;
        }
        while (p->left)
            p = p->left;
        if (is_leaf(p)){
            if(p->parent && p->parent->left == p){
                p->parent->left = NULL;
            }
            else if (p->parent && p->parent->right == p){
                p->parent->right = NULL;
            }
            delete p;
            p = root;
        }
        else if (p->right)
            p = p->right;
    }
}


Rope::Rope(const Rope& r){

	*this = r;
	//assert(0 && "Rope copy constructor not implemented!");
}

Rope& Rope::operator= (const Rope& r){

   if (&r != this) {
        this->root = NULL;
        this->size_ = 0;
        this->root = this->copy_tree(r.root, NULL); 		// copy entire tree of rope r to "this"
        this->size_ = r.size_;
        if(this->root)
            this->root->parent = NULL;
        return *this;
    }
    else
        return *this;

	//assert(0 && "Rope assignment not implemented!");
}

// function to copy entire tree and maintain parent/child node relationship
Node* Rope::copy_tree(Node* old_root, Node* prnt) {
	

	if (!old_root) {
		return NULL;
	}

	Node* temp = new Node;
	temp->value  = old_root->value;
	temp->weight = old_root->weight;
	temp->parent = prnt;

	// recursive copy 
	if(old_root->left)
        temp->left = copy_tree(old_root->left,   temp);		// copy all left nodes. parent for temp->left is temp
    if(old_root->right)
        temp->right = copy_tree(old_root->right, temp);		// copy all right nodes. parent for temp->right is temp
    return temp;

  }

//MUST BE ITERATIVE
//Get a single character at index i
bool Rope::index(int i, char& c) const{
	rope_iterator it(root);
    bool l_found = false;
    while (!l_found){
    	if (is_leaf(it.ptr_)){
            l_found = true;
            c = it.ptr_->value[i];
        }
        else{    
            int wt = it.ptr_->weight;
            if (i < wt)
                it = it.ptr_->left;
            else{
                i = std::max(0, i- wt);
                it = it.ptr_->right;
            }
        }
    }
    return (l_found);
	//assert(0 && "Rope index() not implemented!");
}

//Add the other rope (r)'s string to the end of my string
void Rope::concat(const Rope& r){

	// create a brand new node which will become the new root
	Node *new_root = new Node; 

    new_root->parent = NULL; 
    new_root->left  = copy_tree(this->root, NULL); 						// create a copy of current root to the left of the new root
    new_root->right = copy_tree(r.root, NULL); 							// create a copy of r root to the right of the new root

    new_root->left->parent = new_root->right->parent = new_root; 		//set the parent of left and right to be new_root

    new_root->weight = this->size_; 
    Rope x = Rope(new_root);											//create the new concatinated rope using new_root
    x.size_ = x.RopeSize();
    this->destroy_rope();												// clean up current rope and set concatinated rope to be "this"
    *this = x;

	//assert(0 && "Rope concat() not implemented!");
}

//Get a substring from index i to index j.
//Includes both the characters at index i and at index j.
//String can be one character if i and j match
//Returns true if a string was returned, and false otherwise
//Function should be written with efficient running time.
bool Rope::report(int i, int j, std::string& s) const{
	s = "";
    for (int x = i; x<j+1; x++){
        char c = '\0';
        bool l_found = index(x, c);
        if (l_found)
            s += c;
        else
            break;
    }
    return s.length();
	//assert(0 && "Rope report() not implemented!");
}

//The first i characters should stay in the current rope, while a new
//Rope (rhs) should contain the remaining size_-i characters.
//A valid split should always result in two ropes of non-zero length.
//If the split would not be valid, this rope should not be changed,
//and the rhs rope should be an empty rope.
//The return value should be this rope (the "left") part of the string
//This function should move the nodes to the rhs instead of making new copies.
Rope& Rope::split(int i, Rope& rhs){
	int loc_wt = 0;

	bool first_l_node = true;
	bool first_r_node = true;
	Rope L;
	Rope R;

	// based on the index left nodes are collected for the lhs rope 
	// remaining nodes are collected for the rhs rope
	for (rope_iterator it=this->begin(); it!=this->end(); it++){
		if (is_leaf(it.ptr_)){
			if (loc_wt + it.ptr_->weight <= i){      // collecting nodes for lhs rope
				Node* t = new Node;
				t->weight = it.ptr_->weight; t->value = it.ptr_->value; t->left = NULL; t->right = NULL; t->parent = NULL;
				if (first_l_node){
		 			first_l_node = false;
		 			L = Rope(t);
				}
				else{
			 		L.concat(Rope(t));
			 		L.root->parent=NULL;
				}
			}
			else if (loc_wt < i && loc_wt + it.ptr_->weight > i){ // node requires split. the split left node is tl and the right is tr
				std::string s = it.ptr_->value;
				int lstr_pl = i-loc_wt;
				Node* tl = new Node;
				tl->left = NULL; tl->right = NULL; tl->parent = NULL;
				tl->weight = lstr_pl;
				tl->value = s.substr(0, lstr_pl);
				if (first_l_node){
			  		first_l_node = false;
			  		L = Rope(tl);
			 	}
			 	else{
			  		L.concat(Rope(tl));
			  		L.root->parent=NULL;
			 	}

				int rstr_pl = s.length()-lstr_pl;
				Node* tr = new Node;
				tr->left = NULL; tr->right = NULL; tr->parent = NULL;
				tr->weight = rstr_pl;
				tr->value = s.substr(lstr_pl, rstr_pl);
				if (first_r_node){
					first_r_node = false;
					R = Rope(tr);
				}
				else{
					R.concat(Rope(tr));
					R.root->parent=NULL;
				}
			}

			else if (loc_wt + it.ptr_->weight > i){ // all remaining right nodes collected for rhs rope
				Node* t = new Node;
				t->weight = it.ptr_->weight; t->value = it.ptr_->value; t->left = NULL; t->right = NULL; t->parent = NULL;
				if (first_r_node){
					first_r_node = false;
					R = Rope(t);
				}
				else{
					R.concat(Rope(t));
					R.root->parent=NULL;
				}
			}

			loc_wt = loc_wt + it.ptr_->weight;      // weights updated to compare with index
		}
	}

	if (L.size_ > 0 && R.size_ > 0){
		this->destroy_rope();
        rhs.destroy_rope();
        *this = L;                                    // "this" is the left, and rhs is the right rope
        rhs = R;
	}
	else     //empty rhs rope
		rhs = Rope();								// unable to slpit, send back empty rhs rope, "this" stays as is

	return *this;

	//assert(0 && "Rope split() not implemented!");
}
