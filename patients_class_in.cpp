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
    int get_num(){
        return num;
    }
    stack<Record*>* get_Record(){
        return &Record_stk;
    }
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
            cout << "par: " << par->key << endl;
            cout << "me: " << node->key << endl;
            node->parent = par;
            doubleRed(node);
        }
        cout << get_depth(node) << ' ' << 1 << endl;
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
    void preorder_traversal(Node* node, string disease_name, int& num_disease){
        if(node==NULL) return;
        preorder_traversal(node->left, disease_name, num_disease);
        if(node->pat->get_Record()->top()->disease == disease_name)
            num_disease++;
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
            if(uncle == NULL){
                cout << "RES1" << endl;
                Restructure(node);
            }
            else if(uncle->color == BLK){
                cout << "RES2" << endl;
                Restructure(node);
            }
            else{
                cout << "REC" << endl;
                Recolor(node, uncle);
            }
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
    // string command;
    
    string option;
    int num;
    string name;
    string tel;
    int x;
    int y;
    string disease_name;
    int charge;

    string str = "I 1778361 Edward 01081595876 1408 8758 Fracture 70000&I 1381869 Nancy 01085851167 1616 8167 Burn 30000&I 1548516 Frank 01040559897 1599 2552 Burn 100000&I 1570637 Frank 01019001862 5197 8157 Burn 40000&I 1519868 Susan 01097233696 6943 6326 Insomnia 20000&I 1268218 Frank 01038123263 5818 7159 Flu 20000&I 1750503 William 01014105246 2602 9528 Pneumonia 100000&I 1675039 Susan 01030769665 909 9842 Diabetes 30000&I 1437763 Edward 01065127422 57 4043 Fracture 60000&I 1963267 Frank 01003119658 5605 9488 Burn 100000&I 1488881 Nancy 01021801601 9391 6702 Headache 50000&I 1825382 Henry 01096870110 9188 6504 Fracture 50000&I 1635388 Dorothy 01074981059 406 7939 Burn 10000&I 1244910 Frank 01089010231 6903 3681 Burn 40000&I 1461319 Dorothy 01052690808 9946 1811 Fracture 70000&I 1595580 Athur 01028032209 7619 1403 Pneumonia 30000&I 1828813 Nancy 01098051447 1099 794 Headache 30000&I 1934964 Susan 01011358793 348 3383 Bruise 10000&I 1863520 Dorothy 01066967295 8101 772 Measles 10000&I 1446375 Edward 01037397188 8743 3862 Flu 100000&I 1898142 Nancy 01069407479 6635 6168 Measles 60000&I 1018398 Edward 01001758685 1040 1676 Headache 40000&I 1490585 Athur 01013075341 8984 6787 Insomnia 90000&I 1333106 Frank 01008314556 5123 6572 Measles 10000&I 1709158 Athur 01024043495 7634 6044 Measles 40000&I 1429566 Dorothy 01003445440 6141 5889 Burn 50000&I 1337173 Dorothy 01088098827 2701 2892 Cancer 80000&I 1773000 Nancy 01034274748 878 2439 Measles 10000&I 1602676 Frank 01083542176 1423 6263 Measles 20000&I 1558000 William 01086380140 1432 6158 Burn 30000&I 1883891 Henry 01036062908 2370 7125 Flu 30000&I 1139410 Anna 01012346043 240 1606 Flu 70000&I 1313472 Dorothy 01019618559 3026 9691 Cancer 50000&I 1977106 Dorothy 01093639702 487 6376 Measles 60000&I 1154549 Edward 01019744053 8243 6105 Pneumonia 20000&I 1141842 William 01094571762 9897 756 Burn 30000&I 1941682 Athur 01055552970 3013 5124 Fracture 50000&I 1530818 Dorothy 01048990019 2246 5890 Cancer 90000&I 1334767 Mary 01013144715 5080 2614 Bruise 80000&I 1865921 Athur 01044306975 4627 8293 Burn 50000&I 1567090 William 01005068647 1501 6344 Bruise 70000&I 1391703 Mary 01068834587 9994 5048 Insomnia 80000&I 1462254 Nancy 01076696791 8988 1637 Cancer 10000&I 1253606 Dorothy 01024707075 2521 5637 Measles 20000&I 1333361 Frank 01001259194 1539 5932 Pneumonia 60000&I 1639714 Dorothy 01035110865 2452 2313 Cancer 70000&I 1274474 Mary 01051307330 9198 3050 Headache 40000&I 1574776 William 01058718675 5287 2227 Fracture 50000&I 1847094 Anna 01014115474 5183 4422 Burn 60000&I 1819046 William 01011034089 6778 8388 Fracture 90000&I 1101037 Nancy 01094125752 1612 4782 Burn 90000&I 1454453 Anna 01089511416 2260 8937 Cancer 10000&I 1423659 Dorothy 01015120413 7478 2170 Pneumonia 10000&I 1621461 Anna 01033000511 8002 422 Headache 60000&I 1956357 Frank 01075066103 2797 4361 Measles 10000&I 1490499 Henry 01038376346 9581 6272 Insomnia 90000&I 1901738 Dorothy 01055433961 6277 655 Pneumonia 90000&I 1296077 Dorothy 01059731134 256 5921 Burn 30000&I 1043623 Anna 01055102709 2527 1266 Flu 10000&I 1575367 Nancy 01005250331 2740 9428 Bruise 30000&I 1820547 Henry 01075015948 4188 9377 Cancer 90000&I 1192339 Anna 01011058627 4930 4191 Burn 60000&I 1018022 Frank 01099752244 1217 1995 Cancer 60000&I 1149125 William 01071641047 2856 883 Burn 30000&I 1665872 Frank 01094872534 7199 1836 Cancer 90000&I 1278798 Edward 01001258573 4954 2776 Burn 90000&I 1925523 Nancy 01058618587 2051 3657 Cancer 10000&I 1300791 Nancy 01074833435 4726 8778 Insomnia 30000&I 1736444 Dorothy 01064573190 5178 8439 Insomnia 70000&I 1035015 Athur 01047395257 4187 113 Pneumonia 80000&I 1365814 Anna 01063854479 1371 1857 Insomnia 30000&I 1695153 Anna 01031644251 9952 1282 Flu 10000&I 1042064 Susan 01013683828 1587 3055 Cancer 40000&I 1764895 Susan 01065635565 4504 2950 Flu 90000&I 1504557 Edward 01059834459 9914 6985 Cancer 30000&I 1939060 Susan 01017427737 7020 8871 Measles 70000&I 1489919 Henry 01062014110 2070 2758 Fracture 20000&I 1995153 Edward 01083869255 7401 5274 Cancer 50000&I 1537250 Athur 01089707520 2911 190 Measles 50000&I 1703760 Nancy 01029364384 2721 6602 Diabetes 20000&I 1839987 Mary 01010135054 8155 8716 Diabetes 70000&I 1141994 Dorothy 01052653892 4255 7293 Fracture 10000&I 1217075 Henry 01046266607 1018 8882 Insomnia 80000&I 1136383 Frank 01001280314 1230 7672 Pneumonia 50000&I 1653336 Mary 01039099337 2827 8668 Pneumonia 30000&I 1610310 Nancy 01047777307 8201 4622 Burn 50000&I 1188892 Susan 01091207817 725 5016 Measles 40000&I 1532096 Susan 01040240970 4187 5259 Bruise 20000&I 1193768 Athur 01080747537 9892 8505 Flu 100000&I 1270220 Dorothy 01029753071 3463 3890 Fracture 70000&I 1855849 Dorothy 01064800591 1278 6786 Bruise 10000&I 1701809 Nancy 01084100598 4691 8577 Measles 10000&I 1740024 Frank 01039331503 1754 7402 Measles 90000&I 1102636 Dorothy 01090201306 2336 1701 Bruise 90000&I 1705882 Susan 01086606479 9833 1898 Diabetes 60000&I 1519600 Frank 01082643623 3633 7195 Fracture 30000&I 1973890 William 01032315414 576 1523 Flu 70000&I 1483288 Mary 01026744256 2255 847 Insomnia 80000&I 1373270 William 01066211359 3269 4575 Burn 100000&I 1928500 Edward 01005805373 7274 7863 Diabetes 40000&";

    string comm_line;
    queue<string> tot_comm;
    int iter = 0;
    int cnt = 0;
    for(int i=0; i<str.size(); i++){
        if(str[i] == '&'){
            cnt++;
            string cuttd = str.substr(iter, i-iter);
            // cout << cuttd << endl;
            tot_comm.push(cuttd);
            iter = i+1;
        }
    }
    cout << cnt << endl;
    cout << "tot_comm.size() " << tot_comm.size() << endl;
    cnt = 0;

    queue<string> q;
    iter = 0;
    for (int i = 0; i < tot_comm.size(); i++){
        string tmp = tot_comm.front(); tot_comm.pop();
        cnt++;
        cout << i+1 << "'" << tmp << "'" << endl;
        iter = 0;
        for (int j = 0; j < tmp.size(); j++){
            if (tmp[j] == ' '){
                // string kk = tmp.substr(iter, j-iter);
                // cout << "''" << kk << "''" << endl;
                q.push(tmp.substr(iter, j - iter));
                iter = j + 1;
            }
        }
        // string kk = tmp.substr(iter, tmp.size()-iter);
        // cout << "^" << kk << "^" << endl;
        q.push(tmp.substr(iter, tmp.size() - iter - 1));
    }
    cout << cnt << endl;

    // int loop;
    // cin >> loop;
    cnt = 0;
    while(true){
        string nth;

        
        // cin >> nth;
        getline(cin, nth);
        // cout << q.front();
        option = q.front(); q.pop();
        num = stoi(q.front()); q.pop();
        name = q.front(); q.pop();
        tel = q.front(); q.pop();
        x = stoi(q.front()); q.pop();
        y = stoi(q.front()); q.pop();
        disease_name = q.front(); q.pop();
        charge = stoi(q.front()); q.pop();
        cnt++;
        cout << cnt << "'" << option << ' ' << num << ' ' << name << ' ' << tel << ' ' << x << ' ' << y << ' ' << disease_name << ' ' << charge << "'" << endl;

        if(option == "I"){
            // cin >> num >> name >> tel >> x >> y >> disease_name >> charge;
            // cout << num+1;
            // cout << "\n\t\t\t: ";
            rbtree.insert(new Node(new Patient(num, name, tel, x, y, disease_name, charge)));
        }
        else if(option=="F"){
            cin >> num;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << ' ';
                finded->pat->prn();
            }
            else
                cout << "Not found" << endl;
            
        }
        else if (option == "A"){
            cin >> num >> disease_name >> charge;
            Node* finded = rbtree.find_loc(num);
            if(finded->key == num){
                cout << rbtree.get_depth(finded) << endl;
                finded->pat->append_record(disease_name, charge);
            }
            else
                cout << "Not found" << endl;
        }
        else if (option == "E"){
            cin >> disease_name;
            cout << rbtree.search_disease(disease_name) << endl;
        }
        else{
            cout << "Wrong option." << endl;
        }
    }

    return 0;
}