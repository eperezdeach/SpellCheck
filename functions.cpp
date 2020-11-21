
/*Cap2Low: convert the word's letter from capital letter to lower case
 * INPUT: std::string -> STRING THAT WILL BE CONVERTED TO LOWER CASE
 * OUTPUT: std::string <- RETURN THE STRING IN LOWER CASE
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "functions.h"
#include "class.h"


void dict2str(std::string &d){
    std::string ln;

    std::ifstream in("words_alpha.txt");
    if(!in){std::cerr << "ERROR DICTIONARY" ; return;}
    while(!in.eof()){
        in >> ln;
        d += ln + " ";
    }
    in.close();
}//Convert the dictionary into string format

void wrds_line(const std::string ln, std::vector<std::string>& v){
    std::stringstream check(ln);
    std::string word;
    while(getline(check,word,' ')){
        /*bool h = FindHyphen(word);
        if (h){
            std::stringstream hyphen(word);
            std::string split_word;
            while (getline(hyphen,split_word, '-'))
            {
                v.push_back(split_word); //add word to vec
            }
        }
        else*/ v.push_back(word); //add word to vec
    }
    v.emplace_back("<br>\n");
}//Break the line into words

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

/* DelSpecChar: Delete Special Characters
 * INPUT: std::string -> STRING THAT WILL ANALYSED FOR SPECIAL CHARACTERS
 * OUTPUT: std::string <- RETURN THE STRING WITHOUT SPEC. CHAR.
 */
std::string DelSpecChar(std::string wrd) {
    std::string tmp = wrd; //tmp: temporary string
    int len = tmp.length(); //length of tmp
    if(int(tmp[len-1])==39){tmp.erase(len-1);}//delete \' at the end
    for (auto it=tmp.cbegin(); it!=tmp.cend();++it){ //loop tmp chars
        //check if the are special chars
        if((int(*it)>=33 && int(*it)!=39 && int(*it)<=47) || (int(*it)>=58  && int(*it)<=64)){
            tmp.erase(it); //delete special char
            --it;
        }
    }

    //if (tmp.length()==0){tmp=wrd;}//if the word is all spec chars, return wrd
    return tmp;
}

/* IsNum: Check if the word is a number
 * INPUT: std::string -> STRING THAT WILL ANALYSED IF IS A NUMBER
 * OUTPUT: bool -> RETURNS IF THE WORD IS ALL NUMBERS
 */
bool IsNum (const std::string wrd)
{
    int l=wrd.length();
    bool num=true;
    for (auto it = wrd.cbegin(); it != wrd.end();++it){ //iterate wrd's chars
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
    std::vector <std::string> token; //token: vector that saves the word and contraction
    std::string tmp = word; //tmp: temporary string
    std::stringstream check(word);
    std::string intermediate;//intermediate: word or contr
    while (getline(check, intermediate,'\'')){//split words by the \'
        token.push_back(intermediate); //add intermediate to token
    }
    //asigno palabra y contraccion
    std::string contr=token [1]; //contr: contraction of word
    std::string wrd=token[0]; //wrd: word without contraction

    bool contrOK = false;//variable mia interna de control, 0 mal 1 bien
    //Caso 1: apostrofe colocado en pos incorrecta --> PALABRA MAL
    if (contr.length() > 2) std::cout <<"Incorrect \n";
        //Caso 2: apostrofe en antepenúltima posicion.
    else if (contr.length() == 2){
        std::vector <std::string> lst_2 = {"re","ll","ve"};
        for (const auto& c: lst_2)
        {
            if (c == contr) {
                //std::cout <<"Its the contr --> " << c;
                contrOK=true;
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
                //std::cout <<"Its the contr --> " << c;
                contrOK=true;
                tmp.empty();
                tmp=wrd;
                if(tmp[tmp.length()-1]=='n' && c =="t"){tmp.erase(tmp.length()-1);}
                break;
            }
        }
        //si control OK es uno, habría que comprobar que la palabra wrd esta en dic
    }else if (contr.length()==0){
        contrOK=true;
        tmp.empty();
        tmp=wrd;
    }
    //Caso 4: apostrofe es la ultima palabra. En este caso ir directamente a buscar wrd.
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

bool FindHyphen (std::string word)
{
    for (int i = 0; i < word.length() - 1; i ++ )
    {
        if (word[i] == (char) '-')
        {
            return true;
            break;
        }
    }
    return false;
}

/*std::string GuionPre (std::string word) {
    //Separo pre y post guion
    std::vector<std::string> token; //token: vector that saves the word and contraction
    std::string tmp = word; //tmp: temporary string
    std::stringstream check(word);
    std::string intermediate;//intermediate: word or contr
    while (getline(check, intermediate, '\'')) {//split words by the \'
        token.push_back(intermediate); //add intermediate to token
    }
    //asigno palabra y contraccion
    //std::string wrd2 = token[1]; //contr: contraction of word
    std::string wrd1 = token[0]; //wrd: word without contraction
    return wrd1;
}

std::string GuionPost (std::string word) {
    //Separo pre y post guion
    std::vector<std::string> token; //token: vector that saves the word and contraction
    std::string tmp = word; //tmp: temporary string
    std::stringstream check(word);
    std::string intermediate;//intermediate: word or contr
    while (getline(check, intermediate, '\'')) {//split words by the \'
        token.push_back(intermediate); //add intermediate to token
    }
    //asigno palabra y contraccion
    std::string wrd2 = token[1]; //contr: contraction of word
    //std::string wrd1 = token[0]; //wrd: word without contraction
    return wrd2;
} */
