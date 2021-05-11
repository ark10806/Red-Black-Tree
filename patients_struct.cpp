#include <iostream>
#include <queue>
#include <string>

#include <vector>
using namespace std;

struct Address{
    int x;
    int y;
};

struct Records{
    string disease;
    unsigned int charge;
};

struct Patients{
    unsigned int num;
    string name;
    string tel;
    Address addr;
    Records record;
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

    vector<Patients> vec;
    Patients pats;
    int field_cnt = 0;
    while(!command_line.empty()){
        switch (field_cnt){
            case 0:
                pats.num = command_line.front();
                break;
            case 1:
                pats.name = command_line.front();
                break;
            case 2:
                pats.tel = command_line.front();
                break;
            case 3:
                Address addr;
                addr.x = command_line.front();
                command_line.pop();
                addr.y = command_line.front();
                break;
            case 4:
                Records rec;
                rec.disease = command_line.front();
                command_line.pop()
                rec.charge = command_line.front();
                break;
            default:
                command_line.pop();
                field_cnt++;
                break;
        }
        // cout << command_line.front() << endl;
        // command_line.pop();
    }

    return 0;
}