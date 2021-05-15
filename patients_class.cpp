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

/*
To-do
1. 신규가입 I
    -깊이와 거절/승인을 출력
    -입력하려는 환자 정보가 이미 존재한다면 그 노드의 깊이를 출력만 한다. 신규가입 거절

2. 환자검색 F
    -환자번호로 탐색. [깊이, 이름, 연락처, x,y]를 출력
    -없다면 Not found를 출력

3. 추가진료 A
    -환자 번호로 찾고 지료정보 추가
    -[병명, 진료비]
    ->진료기록 갱신시 깊이 출력
    ->없으면 Not found를 출력

4. 유행병 조사 E
    -해당 병명을 집계.
    -[E, 병명]
    ->주어진 병명과 같은 환자의 수.
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
        cout << name << ' ' << tel << ' ' << addr.x << ' ' << addr.y << endl;
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
        cout << "me:\t" << this->key << endl;
        cout << "par:\t" << this->parent->key << endl;
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
                cout << '<' << get_depth(par) << ' ' << 0 << '>' << endl;
                return;
            }
            node->parent = par;
            doubleRed(node);
        }
        // prn(root);
        cout << '<' << get_depth(new_node) << ' ' << 1 << '>' << endl;
    }
    int get_depth(Node* node){
        int depth = 0;
        while(node->parent != NULL){
            node = node->parent;
            depth++;
        }
        return depth;
    }
    void prn(Node* node){
        if(node == NULL) return;
        prn(node->left);
        cout << node->key << ' ' ;
        if(node->color == RED)
            cout << "RED" << endl;
        else
            cout << "BLK" << endl;
        prn(node->right);
    }
    Node* find_loc(int key){
        Node* curr = root;
        Node* prev = NULL;
        while(curr != NULL){
            prev = curr;
            if(key < curr->key){
                curr = curr->left;
            }
            else if(key > curr->key){
                curr = curr->right;
            }
            else{
                return curr;
            }
        }
        return prev;
    }
    
    void doubleRed(Node* node){
        Node* par = node->parent;
        if(par==NULL) return;
        Node* uncle = node->get_uncle();
        if(par->color == RED){
            if(uncle == NULL){
                cout << "##start Restructuring" << endl;
                Restructure(node);
            }
            else if(uncle->color == BLK){
                cout << "##start Restructuring" << endl;
                Restructure(node);
            }
            else{
                cout << "##start Recoloring" << endl;
                Recolor(node, uncle);
            }
        }
    }
    void getTwigs(Node* node, vector<int> family_keys, vector<Node*>& subtrees, vector<Node*>& preordered_family){
        if(node==NULL) return;
        getTwigs(node->left, family_keys, subtrees, preordered_family);
        if(find(family_keys.begin(), family_keys.end(), node->key) != family_keys.end()){
            preordered_family.push_back(node);

            if(node->left == NULL)  subtrees.push_back(NULL);
            else if(find(family_keys.begin(), family_keys.end(), node->left->key) == family_keys.end()){
                subtrees.push_back(node->left);
            }
            if(node->right == NULL)  subtrees.push_back(NULL);
            else if(find(family_keys.begin(), family_keys.end(), node->right->key) == family_keys.end()){
                subtrees.push_back(node->right);
            }
        }
        
        getTwigs(node->right, family_keys, subtrees, preordered_family);
    }
    void Restructure(Node* node){
        Node* par = node->parent;
        Node* grand = par->parent;
        Node* head = grand->parent;

        vector<int> family_keys;
        family_keys.push_back(grand->key);
        family_keys.push_back(par->key);
        family_keys.push_back(node->key);

        vector<Node*> subtrees;
        vector<Node*> preordered_family;   // me, par, grand in increasing order.
        getTwigs(grand, family_keys, subtrees, preordered_family);
        cout << "subTree number:\t" << subtrees.size() << endl;
        // cout << "preordered_family:\t" << preordered_family.size() << endl;
        // cout << "preordered_family[0].key: " << preordered_family[0]->key << '\t';
        Node* T1 = subtrees[0];
        Node* T2 = subtrees[1];
        Node* T3 = subtrees[2];
        Node* T4 = subtrees[3];
        cout << "Ts: " << T1 << ' ' << T2 << ' ' << T3 << ' ' << T4 << endl;


        cout << preordered_family[0]->key << ' ' << preordered_family[1]->key << ' ' << preordered_family[2]->key << endl;
        
        preordered_family[0]->parent = preordered_family[1];
        preordered_family[0]->left = T1;
        preordered_family[0]->right = T2;
        preordered_family[0]->color = RED;

        

        if(head == NULL)
            root = preordered_family[1];
        else{
            if(head->left == grand){ head->left =  preordered_family[1]; }
            if(head->right == grand){ head->right =  preordered_family[1]; }
        }
        preordered_family[1]->parent = head;
        // cout << "head: " << preordered_family[1]->parent->key << endl;
        preordered_family[1]->left = preordered_family[0];
        preordered_family[1]->right = preordered_family[2];
        preordered_family[1]->color = BLK;

        preordered_family[2]->parent = preordered_family[1];
        preordered_family[2]->left = T3;
        preordered_family[2]->right = T4;
        preordered_family[2]->color = RED;
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
    string command;
    vector<string> test;

    command = "I 1005691 Mary 01012345678 1123 90 Pneumonia 50000";
    test.push_back(command);
    command = "I 1024129 Dorothy 01014832345 3453 6660 Diabetes 10000";
    test.push_back(command);
    command = "I 1009711 Frank 01090123141 5453 5678 Fracture 10000";
    test.push_back(command);
    command = "I 1008353 Athur 01065461752 23 2365 Measles 10000";
    test.push_back(command);
    command = "I 1012317 Anna 01048713158 111 2234 Flu 100000";
    test.push_back(command);
    command = "I 1014748 Edward 01097123455 3245 1234 Bruise 10000";
    test.push_back(command);

    command = "I 1011062 Nancy 01078954184 766 445 Fracture 10000";
    test.push_back(command);
    command = "I 1028522 Henry 01015648964 4346 6567 Fracture 10000";
    test.push_back(command);
    command = "F 1005691";
    test.push_back(command);
    command = "F 1003200";
    test.push_back(command);

    command = "I 1014748 Susan 01093223455 322 124 Fracture 10000";
    test.push_back(command);
    int i =0;
    while(true){
        queue<string> command_line;
        command = "";
        if(i==test.size()) break;
        getline(cin, command);
        command = test[i];
        i++;
        Parser(command_line, command);
    
        string option = command_line.front(); command_line.pop();
        cout << '\t' << option << endl;
        if(option == "I"){
            cout << "comm_line.front():" << command_line.front() << endl;
            rbtree.insert(new Node(new Patient(command_line)));
        }
        else if(option=="F"){
            int key = stoi(command_line.front());
            Node* finded = rbtree.find_loc(key);
            if(finded->key == key){
                cout << rbtree.get_depth(finded) << ' ';
                finded->pat->prn();
            }
            else
                cout << "Not found" << endl;
            
        }
        else if (option == "A"){

        }
        else if (option == "E"){

        }
        else{
            cout << "Wrong option." << endl;
        }

        // Patient* pat(command_line);
        // Patient* pat = new Patient(command_line);
        // rbtree.insert(new Node(pat));
        // cout << i+1 << ' ' << "pat->prn(): ";
        // pat->prn();
        // cout << endl;
        // i++;
    }

    return 0;
}