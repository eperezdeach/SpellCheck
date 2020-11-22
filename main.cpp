#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <set>
#include "functions.cpp"

using namespace std::chrono_literals;

std::set<std::string> dict = {"<br>\n"}; //dict: Global variable that contains the dictionary in string format
std::mutex mut;
std::mutex _files;
std::condition_variable data_cond;

void Read(const char* file,std::vector<std::string> &ln_v,std::vector<std::string> &wrds_v){
    std::ifstream in(file);
    std::string line;
    bool found;
    //std::cout << "Opening file...\n";

    std::unique_lock<std::mutex> lock(mut);
    while(getline(in,line)){
        wrds_v.clear();
        SeparateWords(line, wrds_v);
        bool f, c;
        std::string tmp;//tmp: temporal string
        std::string tmp_line;//tmp: temporal
        for (auto w: wrds_v){
            found = dict.find(w) != dict.end();
            if(!found) {
                tmp=w;//copy w in tmp
                found=IsNum(tmp);
                tmp=Cap2Low(tmp);//convert to lower cases
                c=WordContr(tmp);
                if (c){
                    tmp=ContraccionOK(tmp);
                }
                if(!found){found = dict.find(tmp)!= dict.end();}
            }
            if(!found){
                w = "<a style=\"color:red\">" + w + "</a>";
            }
            tmp_line+=w;
        }
        if (tmp_line!=line){
            line = tmp_line;
        }
        ln_v.push_back(line);
        tmp_line.clear();
        tmp.clear();
        data_cond.notify_one();
    }
}

void Write(std::vector<std::string> &v){

    std::unique_lock<std::mutex> lock(mut);
    data_cond.wait(lock);
    std::this_thread::sleep_for(10ms);
    std::ofstream out("output.html");
    std::cout << "Writing file...\n";
    out << "<html>\n";
    out << "\t<body>\n";


    for (auto w:v){
        out << w << "<br>\n";
    }
    out << "\t</body>\n";
    out << "</html>\n";
    out.close();
}

int main(int argc, char* argv[]) {
    if(argc < 4){std::cerr << "ERROR: NOT ENOUGH ARGUMENTS!\n";return -1;}
    auto start = std::chrono::steady_clock::now();
    std::vector<std::string> doc_lines;
    std::vector<std::string> line_break;
    std::size_t found;
    dict2str(argv[1], dict);
    AddSpecChar2Dict(dict);
    AddContractions2Dict(dict);

    {

        std::thread t_read(Read,argv[3], ref(doc_lines), ref(line_break));
        std::thread t_write(Write, ref(doc_lines));

        t_read.join();
        t_write.join();
    }


    auto stop = std::chrono::steady_clock::now();
    auto diff = stop-start;
    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << "\n";

    return 0;
}
