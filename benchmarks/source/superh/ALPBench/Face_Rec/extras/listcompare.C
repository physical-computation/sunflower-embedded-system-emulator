#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <assert.h>

#define IN_LIST_ONE 1
#define IN_LIST_TWO 2
#define IN_BOTH 3

using namespace std;

int main(int argc, char** argv){
    if( argc < 3 ){ cout << "Error: two image lists required" << endl; }
    map<string,int> image;
    map<string,int> subject;
    ifstream list1(argv[1]);
    ifstream list2(argv[2]);
    string tmp;

    int cnt1 = 0, cnt2 = 0, cntb = 0;
    int sbj1 = 0, sbj2 = 0, sbjb = 0;
    int cnt1total = 0, cnt2total = 0;

    assert(list1);
    assert(list2);

    while(list1 >> tmp){
        image[tmp] = image[tmp] | IN_LIST_ONE;

        string sbj(tmp,0,5);
        //cout << sbj << endl;
        subject[sbj] = subject[sbj] | IN_LIST_ONE;
        
        cnt1total++;
    }

    while(list2 >> tmp){
        image[tmp] = image[tmp] | IN_LIST_TWO;
        string sbj(tmp,0,5);
        subject[sbj] = subject[sbj] | IN_LIST_TWO;
        cnt2total++;
    }

    ofstream l1("list1only.list");
    ofstream l2("list2only.list");
    ofstream both("both.list");
    for(map<string,int>::iterator each = image.begin(); each != image.end(); each++){
        if(each->second == 1){
            l1 << each->first << endl;
            cnt1++;
        } else if(each->second == 2){
            l2 << each->first << endl;
            cnt2++;
        } else if(each->second == 3){
            both << each->first << endl;
            cntb++;
        } else {
            cout << "Error: unknown problem " << each->first << endl;
            exit(1);
        }
    }
    for(map<string,int>::iterator each = subject.begin(); each != subject.end(); each++){
        if(each->second == 1){
            sbj1++;
        } else if(each->second == 2){
            sbj2++;
        } else if(each->second == 3){
            sbjb++;
        } else {
            cout << "Error: unknown problem " << each->first << endl;
            exit(1);
        }
    }

    cout << "Number of images only in first list: " << cnt1 << " of " << cnt1total << endl;
    cout << "Number of images only in second list: " << cnt2 << " of " << cnt2total << endl;
    cout << "Number of images in both lists: " << cntb << endl;
    cout << "Number of subjects only in first list: " << sbj1 << " of " << sbj1+sbjb << endl;
    cout << "Number of subjects only in second list: " << sbj2 << " of " << sbj2+sbjb << endl;
    cout << "Number of subjects in both lists: " << sbjb << endl;
    
    return 0;

}

