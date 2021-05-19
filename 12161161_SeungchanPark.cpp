#include <iostream>
#include <stack>
#define RED true    // 가독성을 위하여 RED를 true,
#define BLK false   // BLACK을 BLK 즉 false로 선언하였다.
using namespace std;

class Patient{
    // 환자의 정보 [번호, 이름, 연락처, 주소, 기록]을 저장하는 클래스
    
private:// 환자의 기록은 함부로 접근, 변경할 수 없게 private 변수로 선언하였다.
    int num;
    string name;
    string tel;
    struct Address{
        // 주소는 x와 y를 원소로 갖는 구조체로 구현.
        int x;
        int y;
    };
    struct Record{
        // 환자 진료 기록은 진단병명과 진료비를 원소로 갖는 구조체로 구현.
        string disease;
        int charge;
    };
    stack<Record*> Record_stk;
    // 기록은 마지막 진료 기록만 활용하므로, LIFO구조를 따르는 Stack
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
        // Record_stk 이라는 이름을 가진 Stack 자료구조에 환자의 (진료기록, 진료비)를 차례로 삽입.
        Record* rec = new Record;
        rec->disease = disease_name;
        rec->charge = charge;
        Record_stk.push(rec);
    }
};

class Node{
    // Red-Black Tree(이하 RB-tree)의 원소가 되는 Node이고, 환자번호를 key로 사용.
public:
    int key;
    bool color; // Color는 위에서 선언한 RED(true)또는 BLACK(false)이 된다.
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
        // 부모의 형제를 찾아 반환하는 함수이다. 성공시 uncle노드의 주소를, 실패시 NULL을 반환한다.
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
        // 인자로 건네받은 node기준으로 NULL이 발견될때까지 계속하여 parent를 구함으로써 해당 노드의 depth를 구하여 반환.
        int depth = 0;
        while(node->parent != NULL){
            node = node->parent;
            depth++;
        }
        return depth;
    }
    Node* find_loc(int key){
        // 이진탐색하여 마지막으로 위치한 노드의 부모를 반환한다.
        // 실패시 NULL을 반환하며, 실패하는 경우는 RB-tree에 첫 원소 즉, root를 삽입하는 경우이다.
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
        // 아래의 search_disease()에서 호출된다. 전위순회하며 '(4)유행병조사'에서 찾는 병명의 수를 구한다.
        // 재귀호출로 구현하여 반환받기 어려우므로 참조변수를 이용하여 결과를 받도록 하였다.
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
        // 삽입된 node 기준으로 부모의 color를 검사하는 함수.
        // uncle의 color를 고려하여 Restructure / Recolor 할지 정한다.
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
        // 강의노트 기준 'v' 즉, 부모의 노드
        Node* par = node->parent;   
        // 'v'의 부모 노드
        Node* grand = par->parent;  
        // grand의 부모 노드. 순서조건에 맞게 재구성된 subtree의 부모가 될 노드이다.
        Node* head = grand->parent; 

        // a, b, c는 각각 grand, paret, me 의 key를 담고 있다.
        int a = grand->key;
        int b = par->key;
        int c = node->key;

        // 순서조건에 맞게 재구성될 노드. f1, f2, f3의 key는 오름차순으로 구성된다.
        Node* f1;
        Node* f2;
        Node* f3;

        // 강의 노트 기준 T1, T2, T3, T4.
        // 재구성 후 Restructuring 순서에 맞게 위치한다.
        Node* T1;
        Node* T2;
        Node* T3;
        Node* T4;

        // Restructuring 순서에 맞게 a, b, c 즉 grand, parent, me 의 key를 고려하여 구성.
        // 총 네 가지의 경우의 수가 존재한다.
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
        
        // 위에서 구한 순서조건의 결과를 결합.
        f1->parent = f2;    // f2 즉 key의 크기 상 중간 순서에 위치하는 노드가 재구성 subtree의 root 노드가 된다.
        f1->left = T1;
        f1->right = T2;
        f1->color = RED;
        // T1이 NULL일 경우 NULL의 부모노드를 지정할 수 없는데 반해, T1이 또 다른 subtree일 경우 부모 노드를 설정해줘야 한다.
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

    string option;  // 수행할 질의를 나타내는 기호
    int num;        // 환자번호
    string name;    // 이름
    string tel;     // 연락처
    int x;          // 주소의 x좌표
    int y;          // 주소의 y좌표
    string disease_name;    // 병명
    int charge;             // 진료비

    int loop;
    cin >> loop;
    for(int i=0; i<loop; i++){
        cin >> option;
        cin.ignore();
        if(option == "I"){  // 신규가입
            cin >> num >> name >> tel >> x >> y >> disease_name >> charge;
            rbtree.insert(new Node(new Patient(num, name, tel, x, y, disease_name, charge)));
        }
        else if(option=="F"){   // 환자검색
            cin >> num;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << ' ';
                finded->pat->prn();
            }
            else { cout << "Not found" << endl; }
            
        }
        else if (option == "A"){    // 추가진료
            cin >> num >> disease_name >> charge;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << endl;
                // stack 자료구조 기반의 Record_stk에 진료 기록을 저장.
                finded->pat->append_record(disease_name, charge);   
            }
            else{ cout << "Not found" << endl; }
        }
        else if (option == "E"){    // 유행병조사
            cin >> disease_name;
            cout << rbtree.search_disease(disease_name) << endl;
        }
        else{ cout << "Wrong option." << endl; }
    }

    return 0;
}