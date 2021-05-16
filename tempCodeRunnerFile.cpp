0; i<tot_comm.size(); i++){
            string tmp = tot_comm.front(); tot_comm.pop();
            for(int j=0; j<tmp.size(); j++){
                if(tmp[j]==' '){
                    q.push(tmp.substr(j, j-iter));
                    iter = j+1;
                }
            }
        }