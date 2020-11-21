
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "functions.h"
#include "class.h"

std::string dict; //dict: Global variable that contains the dictionary in string format

int main(int argc, char * argv[]) {

    if(argc < 3){std::cerr << "ERROR: NOT ENOUGH ARGUMENTS!\n";return -1;}
    auto start = std::chrono::steady_clock::now();
    dict2str(dict);


    std::string line; //line: line to be read of the input text
    std::vector<std::string> vec;//vec: vector that will contain the words of line
    std::size_t found;
    WriteHTML html; //html: Class that will be called to write the HTML
    html.beginHTML();// start html

    std::string in = "test_1 copy.txt";//text to be read
    std::ifstream input(in);
    if(!input) { std::cerr << "ERROR WITH INPUT FILE: " << in; return -1; }

    while(getline(input, line)){//
        //thread t:
        std::thread t(wrds_line,std::ref(line),std::ref(vec));
        t.join();
    }

    bool f,c;//f: check is word is correct/ c: check if word has contraction/ g check for guion
    for (auto w:vec){//loop vec
        found = dict.find(w);
        std::string tmp,tmp1;//tmp: temporal string
        if(found!=std::string::npos){
            f=true; //found
        } else {
            bool h =FindHyphen(w);
            if (h)
            {
                
            }
            else {
                f = false;
                tmp = w;//copy w in tmp
                tmp = Cap2Low(tmp);//convert to lower cases
                tmp = DelSpecChar(tmp);//delete special characters
                if (tmp.length() == 0) { f = true; }
                found = dict.find(tmp);
                if (found != std::string::npos) { f = true; }
                else {
                    c = WordContr(tmp); //search for contraction
                    if (c) {
                        tmp = ContraccionOK(tmp);//return tmp without contrac.
                        found = dict.find(tmp);
                        if (found != std::string::npos) { f = true; }
                    } else {
                        f = IsNum(tmp);
                    }
                }
            }
        }

        html.checkWord(f,w);//write html
    }

    html.endHTML();//end html
    auto stop = std::chrono::steady_clock::now();
    auto diff = stop-start;
    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << "\n";

    return 0;
}
