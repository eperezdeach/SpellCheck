//
// Created by Emilio Pérez De Acha on 11/22/20.
//

#ifndef SPELLCHECK_THREAD_FUNCTIONS_H
#define SPELLCHECK_THREAD_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <set>
#include <thread>
#include <sstream>
#include <fstream>
#include <string>


using namespace std::chrono_literals;

std::set<std::string> dict = {"<br>\n"}; //dict: Global variable that contains the dictionary in string format
std::mutex mut;
std::mutex _files;
std::condition_variable data_cond;


void SeparateWords(std::string word, std::vector<std::string> &vec);
void dict2set(char* file, std::set<std::string> &d);
void AddSpecChar2Dict(std::set<std::string> &d);
std::string Cap2Low(std::string wrd);
bool IsNum (const std::string wrd);
std::string ContraccionOK (std::string word);
bool WordContr (const std::string word);
void Read(const char* file,std::vector<std::string> &ln_v,std::vector<std::string> &wrds_v);
void Write(std::vector<std::string> &v);


#endif //SPELLCHECK_THREAD_FUNCTIONS_H
