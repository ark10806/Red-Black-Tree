#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#define RED true
#define BLK false
using namespace std;

class Patient{
private:
    int num;
    string name;
    string tel;
    struct Address{
        int x;
        int y;
    };
    struct Record{
        string disease;
        int charge;
    };
    stack<Record*> Record_stk;
    Address addr;
public:
    Patient(int num, string name, string tel, int x, int y, string disease_name, int charge){
        this->num = num;
        this->name = name;
        this->tel = tel;
        addr.x = x;
        addr.y = y;
        
        Record* rec = new Record;
        rec->disease = disease_name;
        rec->charge = charge;
        Record_stk.push(rec);
    }
    int get_num(){ return num; }
    stack<Record*>* get_Record(){ return &Record_stk; }
    void prn(){
        cout << name << ' ' << tel << ' ' << addr.x << ' ' << addr.y << endl;
    }
    void append_record(string disease_name, int charge){
        Record* rec = new Record;
        rec->disease = disease_name;
        rec->charge = charge;
        Record_stk.push(rec);
    }
};

class Node{
public:
    int key;
    bool color;
    Patient* pat;
    Node* parent; // Parent node pointer.
    Node* left;
    Node* right;
public:
    Node(Patient* pat, Node* parent=NULL, Node* left=NULL, Node* right=NULL){
        key = pat->get_num();
        color = RED;
        this->pat = pat;
        this->parent = parent;
        this->left = left; // can be just a NULL
        this->right = right;
    }
    Node* get_uncle(){
        Node* grand = this->parent->parent;

        if(grand != NULL){
            if(grand->left == this->parent)
                return grand->right;
            else
                return grand->left;
        }
        return NULL;
    }
};

class RBtree{
private:
    Node* root;
public:
    RBtree(){
        root = NULL;
    }
    void insert(Node* node){
        Node* new_node = node;
        Node* par = find_loc(node->key);
        if(root == NULL){   // In the case of first-Insertion.
            root = node;
            root->color = BLK;
        }
        else{   
            if(node->key < par->key){
                par->left = node;
            }
            else if(node->key > par->key){
                par->right = node;
            }
            else{ // If the key already exists, then returns a Node which has the same key.
                cout << get_depth(par) << ' ' << 0 << endl;
                return;
            }
            node->parent = par;
            doubleRed(node);
        }
        cout << get_depth(new_node) << ' ' << 1 << endl;
    }
    int get_depth(Node* node){
        int depth = 0;
        while(node->parent != NULL){
            node = node->parent;
            depth++;
        }
        return depth;
    }
    Node* find_loc(int key){
        Node* curr = root;
        Node* prev = NULL;
        while(curr != NULL){
            prev = curr;
            if(key < curr->key){ curr = curr->left; }
            else if(key > curr->key){ curr = curr->right; }
            else{ return curr; }
        }
        return prev;
    }
    void preorder_traversal(Node* node, string disease_name, int& num_disease){
        if(node==NULL) return;
        preorder_traversal(node->left, disease_name, num_disease);
        if(node->pat->get_Record()->top()->disease == disease_name) num_disease++;
        preorder_traversal(node->right, disease_name, num_disease);
        
    }
    int search_disease(string disease_name){
        int num_disease = 0;
        preorder_traversal(root, disease_name, num_disease);
        return num_disease;
    }
    
    void doubleRed(Node* node){
        Node* par = node->parent;
        if(par==NULL) return;
        Node* uncle = node->get_uncle();
        if(par->color == RED){
            if(uncle == NULL){ Restructure(node); }
            else if(uncle->color == BLK){ Restructure(node); }
            else{ Recolor(node, uncle); }
        }
    }

    void Restructure(Node* node){
        Node* par = node->parent;
        Node* grand = par->parent;
        Node* head = grand->parent;

        int a = grand->key;
        int b = par->key;
        int c = node->key;

        Node* f1;
        Node* f2;
        Node* f3;

        Node* T1;
        Node* T2;
        Node* T3;
        Node* T4;

        if(a < b){
            f1 = grand;
            T1 = grand->left;
            if(b<c){ 
                f2 = par; f3 = node; 
                T2 = par->left;
                T3 = node->left;
                T4 = node->right;
            }
            else{ 
                f2 = node; f3 = par; 
                T2 = node->left;
                T3 = node->right;
                T4 = par->right;
            }
        }
        else{
            if(b<c){ 
                f1 = par; f2 = node; 
                T1 = par->left;
                T2 = node->left;
                T3 = node->right;
            }
            else{ 
                f1 = node; f2 = par; 
                T1 = node->left;
                T2 = node->right;
                T3 = par->right;
            }
            f3 = grand;
            T4 = grand->right;
        }
        
        f1->parent = f2;
        f1->left = T1;
        f1->right = T2;
        f1->color = RED;
        if(T1 != NULL){ T1->parent = f1; }
        if(T2 != NULL){ T2->parent = f1; }

        if(head == NULL)
            root = f2;
        else{
            if(head->left->key == a){ head->left = f2; }
            else{ head->right = f2; }
        }
        f2->parent = head;
        f2->left = f1;
        f2->right = f3;
        f2->color = BLK;

        f3->parent = f2;
        f3->left = T3;
        f3->right = T4;
        f3->color = RED;
        if(T3 != NULL){ T3->parent = f3; }
        if(T4 != NULL){ T4->parent = f3; }
    }
    void Recolor(Node* node, Node* uncle){
        node->parent->color = BLK;
        uncle->color = BLK;

        Node* grand = node->parent->parent;
        if(grand == root){ grand->color = BLK; }
        else{ grand->color = RED; }
        doubleRed(uncle->parent);
    }
};

int main(){
    RBtree rbtree;

    string option;
    int num;
    string name;
    string tel;
    int x;
    int y;
    string disease_name;
    int charge;

    int loop;
    cin >> loop;
    for(int i=0; i<loop; i++){
        cin >> option;
        cin.ignore();
        if(option == "I"){
            cin >> num >> name >> tel >> x >> y >> disease_name >> charge;
            rbtree.insert(new Node(new Patient(num, name, tel, x, y, disease_name, charge)));
        }
        else if(option=="F"){
            cin >> num;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << ' ';
                finded->pat->prn();
            }
            else { cout << "Not found" << endl; }
            
        }
        else if (option == "A"){
            cin >> num >> disease_name >> charge;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << endl;
                finded->pat->append_record(disease_name, charge);
            }
            else{ cout << "Not found" << endl; }
        }
        else if (option == "E"){
            cin >> disease_name;
            cout << rbtree.search_disease(disease_name) << endl;
        }
        else{ cout << "Wrong option." << endl; }
    }

    return 0;
}