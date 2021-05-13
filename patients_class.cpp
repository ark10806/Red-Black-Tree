#include <iostream>
#include <queue>
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
    void prn(){
        cout << num << ' ' << name << ' ' << tel << '\t';
        cout << addr.x << ", " << addr.y << '\t';
        cout << rec.disease << ' ' << rec.charge << endl;
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

class Node{
public:
    int key;
    bool color;
    Patient* pat;
    Node* parent; // Parent node pointer.
    Node* left;
    Node* right;
public:
    Node(Patient& pat, Node* parent=null, Node* left=null, Node* right=null){
        key = pat->num;
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
}

class RBtree{
private:
    Node* root;
public:
    Patient pat;
    void insert(Node* node){
        Node* par = find_loc(node->get_key);
        node->parent = par;
        if(node->key < par->key){
            par->left = node;
        }
        else{
            par->right = node;
        }
    }
    Node* find_loc(int key){
        Node* curr = root;
        Node* prev = null;
        while(curr != null){
            prev = curr;
            if(key < curr->get_key()){
                curr = curr->left;
            }
            else{
                curr = curr->right;
            }
        }
        return prev;
    }
    
    void doubleRed(Node* node){
        Node* par = node->par;
        Node* uncle = node->par->par->;
        if(par->color == RED){
            if(par->get_uncle()->color == BLK){
                Restructure(node);
            }
            else{
                Recolor(node, uncle);
            }
        }
    }
    void Restructure(Node* node){
        Node* grand = node->parent->parent;
        Node* par = 
        Node* T1 = grand;
        Node* T2 = grand;
        Node* T3 = grand;
        Node* T4 = grand;
        while(T1->isFamily()){
            T1 = T1
        }
        

    }
    void Recolor(Node* node, Node* uncle){
        node->parent->color = BLK;
        uncle->color = BLK;
        uncle->parent->color = RED;
        doubleRed(uncle->parent);
    }
};

int main(){
    queue<string> command_line;
    string command;
    getline(cin, command);
    Parser(command_line, command);

    Patient pat(command_line);
    pat.prn();

    return 0;
}