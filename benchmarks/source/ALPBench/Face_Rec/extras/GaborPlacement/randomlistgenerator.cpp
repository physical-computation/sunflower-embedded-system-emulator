#include <iostream.h>
#include <vector>
#include <stdlib.h>
#include <string>

main(){
    std::string s;
    std::vector<std::string> group;

    while(cin >> s){
        group.push_back(s);
    }

    for(int i = 0; i < 1000000; i++){
        int id1 = random()%group.size();
        int id2 = random()%group.size();
        
        string tmp = group[id1];
        group[id1] = group[id2];
        group[id2] = tmp;
    }
    
    for(int i = 0; i < group.size(); i++){
        cout << group[i] << endl;
    }

}