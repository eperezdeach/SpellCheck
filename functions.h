

#ifndef SPELLCHECKFINAL_FUNCTIONS_H
#define SPELLCHECKFINAL_FUNCTIONS_H
#endif //SPELLCHECKFINAL_FUNCTIONS_H

std::string Cap2Low(std::string wrd);
std::string DelSpecChar(std::string wrd);
bool IsNum (const std::string wrd);
std::string ContraccionOK (std::string word);
bool WordContr (const std::string word);
void dict2str(std::string &d);
void wrds_line(const std::string ln, std::vector<std::string>& v);
bool FindHyphen (std::string word);

