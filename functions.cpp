#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <thread>
#include <set>
#include "functions.h"


//Separate line into single strings
void SeparateWords(std::string word, std::vector<std::string> &vec){
    std::string tmp;
    for (auto it = 0; it < word.length(); ++it){
        //std::cout << word[ii] << std::endl;
        if((int(word[it])>=32 && int(word[it])!=39 && int(word[it])<=47) || (int(word[it])>=58  && int(word[it])<=64)){
            if (tmp!=""){vec.push_back(tmp);}
            tmp = word[it];
            vec.push_back(tmp);
            tmp.clear();
            continue;
        }
        tmp += word[it];
    }
    vec.push_back(tmp);
}

//Convert string to set
void dict2set(char* file, std::set<std::string> &d){
    std::string ln;

    std::ifstream in(file);
    if(!in){std::cerr << "ERROR DICTIONARY" ; return;}
    while(!in.eof()){
        in >> ln;
        d.insert(ln);
    }
    in.close();
}//Convert the dictionary into string format

//Add special characters to dictionary - Except \'
void AddSpecChar2Dict(std::set<std::string> &d) {
    std::string tmp_i;
    std::string tmp_j;
    int ii= 32, jj = 58;
    while(ii < 48 || jj < 65){
        if(ii<48){
            tmp_i.push_back((char)ii);
            d.insert(tmp_i);
        }
        if(jj<65){
            tmp_j.push_back((char)jj);
            d.insert(tmp_j);
        }
        tmp_i.clear();
        tmp_j.clear();
        ++ii;++jj;
    }

}

/*Cap2Low: convert the word's letter from capital letter to lower case
 * INPUT: std::string -> STRING THAT WILL BE CONVERTED TO LOWER CASE
 * OUTPUT: std::string <- RETURN THE STRING IN LOWER CASE
 */
std::string Cap2Low(std::string wrd){
    std::string tmp;
    tmp=wrd;
    for(char &c: tmp){
        if(int(c)>=65 && int (c)<=90) {//check for capital letters
            c=tolower(c); //convert the char into lower case letter
        }
    }
    return tmp;
}

/* IsNum: Check if the word is a number
 * INPUT: std::string -> STRING THAT WILL ANALYSED IF IS A NUMBER
 * OUTPUT: bool -> RETURNS IF THE WORD IS ALL NUMBERS
 */
bool IsNum (const std::string wrd)
{
    int l=wrd.length();
    std::string tmp = wrd;
    bool num=true;
    for (auto it = tmp.cbegin(); it != tmp.end();++it){ //iterate wrd's chars
        if (int(*it)< 48 || int(*it) > 57 ) //Check if it is not a number
        {
            num = false; //no es num
            break;
        }

    }
    if(wrd[l-1]=='s'){num = true;}
    return num; //si es un num
}

/* CONTRACCIONOK: Search if the word the contraction is correct
* INPUT: std::string -> WORD THAT WILL BE CHECKED FOR CONTRACTION
        * OUTPUT: std::string <- RETURNS THE WORD WITHOUT THE CONTRACTION
*/
std::string ContraccionOK (std::string word)
{
    //Separo pre y post apostrofe
    std::vector <std::string> token = {"",""}; //token: vector that saves the word and contraction
    std::string tmp = word; //tmp: temporary string
    std::stringstream check(tmp);
    std::string contr = "";
    std::string wrd = "";
    std::string intermediate;//intermediate: word or contr
    int pos = 0;
    while (getline(check, intermediate,'\'')){//split words by the \'
        token[pos]=intermediate; //add intermediate to token
        ++pos;
    }
    //asigno palabra y contraccion
    contr= token [1]; //contr: contraction of word
    wrd=token[0]; //wrd: word without contraction

    //Caso 1: apostrofe colocado en pos incorrecta --> PALABRA MAL
    if (contr.length() > 2) std::cout <<"Incorrect: " << tmp << "\n";
        //Caso 2: apostrofe en antepenúltima posicion.
    else if (contr.length() == 2){
        std::vector <std::string> lst_2 = {"re","ll","ve"};
        for (const auto& c: lst_2)
        {
            if (c == contr) {
                tmp.empty();
                tmp=wrd;
                break;
            }
        }
        //si control OK es uno, habría que comprobar que la palabra wrd esta en dic
    }
        //Caso 3: apostrofe en penúltima posicion.
    else if (contr.length() == 1){
        std::vector <std::string> lst_1 = {"m","s","d","t"};
        for (auto c: lst_1)
        {
            if (c == contr) {
                tmp.empty();
                tmp=wrd;
                if(tmp[tmp.length()-1]=='n' && c =="t"){tmp.erase(tmp.length()-1);}
                break;
            }
        }
        //si control OK es uno, habría que comprobar que la palabra wrd esta en dic
    }else if (contr.length()==0){
        tmp.empty();
        tmp=wrd;
    }
    //Caso 4: apostrofe es la ultima palabra. En este caso ir directamente a buscar wrd.
    if(wrd[wrd.length()-1]=='s'){tmp=wrd;}
    token.clear();
    return tmp;
}

/* WordContr: Search if the word has a contraction
 * INPUT: std::string -> STRING THAT WILL ANALYSED FOR SPECIAL CHARACTERS
 * OUTPUT: bool -> RETURNS IF THE WORD IS CONTRACTION
 */
bool WordContr (const std::string word){
    // 0 no está, 1 esta
    bool contr=false;//contr: check if the word has a contraction
    for (auto ct=word.cbegin(); ct!=word.cend();++ct)//loop word's chars
    {
        //std::cout << "ct: " << *ct << "\nASCII: " << int(*ct) << "\n";
        if (int(*ct) == 39) //check if word has a \'
        {
            contr = true; //contraction in word
            return contr;
        }

    }
    return contr;
}

//Read input file
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

