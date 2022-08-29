#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>

#include "comando.cpp"

using namespace std;

class Analizador{
    public:
        Comando cmd;
        vector<string> split_txt(string entrada, char c);
        string replace_txt(string str, const string& from, const string& to);
        void analizar(string entrada);
        void identificarParametros(string comando,vector<string> parametros);
};

#endif // ANALIZADOR_H