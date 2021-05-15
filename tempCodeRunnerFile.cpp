ector<string> test;

    // command = "I 1005691 Mary 01012345678 1123 90 Pneumonia 50000&I 1024129 Dorothy 01014832345 3453 6660 Diabetes 10000&I 1009711 Frank 01090123141 5453 5678 Fracture 10000&I 1008353 Athur 01065461752 23 2365 Measles 10000&I 1012317 Anna 01048713158 111 2234 Flu 100000&I 1014748 Edward 01097123455 3245 1234 Bruise 10000&I 1011062 Nancy 01078954184 766 445 Fracture 10000&I 1028522 Henry 01015648964 4346 6567 Fracture 10000&F 1005691&F 1003200&I 1014748 Susan 01093223455 322 124 Fracture 10000&E Fracture&E Headache&A 1008000 Pneumonia 30000&A 1011062 Pneumonia 30000&E Fracture&";
    // int iter = 0;
    // for(unsigned int i=0; i<command.size(); i++){
    //     if(command[i] == '&'){
    //         test.push_back(command.substr(iter, i-iter));
    //         iter = i+1;
    //     }
    // }