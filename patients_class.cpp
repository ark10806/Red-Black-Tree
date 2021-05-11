#include <iostream>
#include <queue>
using namespace std;

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

int main(){
    queue<string> command_line;
    string command;
    getline(cin, command);
    Parser(command_line, command);

    Patient pat(command_line);
    pat.prn();

    return 0;
}