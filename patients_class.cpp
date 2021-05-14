#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#define RED true
#define BLK false
using namespace std;

/*
알고리즘 시간복잡도를 보고서에서 다뤄야 한다.
표지 목차
주석
*/

class Patient{
private:
    unsigned int num;
    string name;
    string tel;
    struct Address{
        int x;
        int y;
    };
    struct Record{
        string disease;
        unsigned int charge;
    };
    Address addr;
    Record rec;
public:
    Patient(queue<string> que){
        num = stoi(que.front()); que.pop();
        name = que.front(); que.pop();
        tel = que.front(); que.pop();
        
        addr.x = stoi(que.front()); que.pop();
        addr.y = stoi(que.front()); que.pop();
        
        rec.disease = que.front(); que.pop();
        rec.charge = stoi(que.front()); que.pop();
    }
    int get_num(){
        return num;
    }
    void prn(){
        cout << num << ' ' << name << ' ' << tel << '\t';
        cout << addr.x << ", " << addr.y << '\t';
        cout << rec.disease << ' ' << rec.charge << endl;
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
    void set_parent(Node* parent){
        this->parent = parent;
    }
    Node* get_uncle(){
        Node* grand = this->parent->parent;
        if(grand->left == this->parent)
            return grand->right;
        else
            return grand->left;
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
        Node* par = find_loc(node->key);
        node->parent = par;
        if(node->key < par->key){
            par->left = node;
        }
        else{
            par->right = node;
        }
        doubleRed(node);
    }
    Node* find_loc(int key){
        Node* curr = root;
        Node* prev = NULL;
        while(curr != NULL){
            prev = curr;
            if(key < curr->key){
                curr = curr->left;
            }
            else{
                curr = curr->right;
            }
        }
        return prev;
    }
    
    void doubleRed(Node* node){
        Node* par = node->parent;
        Node* uncle = node->parent->get_uncle();
        if(par->color == RED){
            if(uncle->color == BLK){
                cout << "start Restructuring" << endl;
                Restructure(node);
            }
            else{
                cout << "start Recoloring" << endl;
                Recolor(node, uncle);
            }
        }
    }
    void getTwigs(Node* node, vector<int> family_keys, vector<Node*>& subtrees, vector<Node*>& postordered_family){
        getTwigs(node->left, family_keys, subtrees, postordered_family);
        if(find(family_keys.begin(), family_keys.end(), node->parent->key) == family_keys.end()){
            // inserts subtree to stl::vector subtrees in sequential if current node's parent is in (me, parent, grand)
            subtrees.push_back(node);
            postordered_family.push_back(node->parent);
        }
        getTwigs(node->right, family_keys, subtrees, postordered_family);
    }
    void Restructure(Node* node){
        Node* grand = node->parent->parent;
        Node* par = node->parent;
        vector<int> family_keys;
        family_keys.push_back(grand->key);
        family_keys.push_back(par->key);
        family_keys.push_back(node->key);

        vector<Node*> subtrees;
        vector<Node*> postordered_family;
        getTwigs(grand, family_keys, subtrees, postordered_family);
        Node* T1 = subtrees[0];
        Node* T2 = subtrees[1];
        Node* T3 = subtrees[2];
        Node* T4 = subtrees[3];

        postordered_family[0]->parent = postordered_family[1];
        postordered_family[0]->left = T1;
        postordered_family[0]->right = T2;

        postordered_family[1]->parent = grand->parent;
        postordered_family[1]->left = postordered_family[0];
        postordered_family[1]->right = postordered_family[2];

        postordered_family[2]->parent = postordered_family[1];
        postordered_family[2]->left = T3;
        postordered_family[2]->right = T4;
    }
    void Recolor(Node* node, Node* uncle){
        node->parent->color = BLK;
        uncle->color = BLK;
        uncle->parent->color = RED;
        doubleRed(uncle->parent);
    }
};

void Parser(queue<string> & que, string comm){
    int iter=0;
    int comm_size = comm.size();
    for(int i=0; i<comm_size; i++){
        if(comm[i] == ' '){
            que.push(comm.substr(iter, i-iter));
            iter = i+1;
        }
    }
    que.push(comm.substr(iter, comm_size));
}

int main(){
    RBtree rbtree;
    queue<string> command_line;
    string command;
    while(true){
        getline(cin, command);
        Parser(command_line, command);

        Patient pat(command_line);
        pat.prn();
        Node node(&pat);
        rbtree.insert(&node);
    }

    return 0;
}