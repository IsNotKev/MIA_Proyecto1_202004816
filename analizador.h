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

typedef struct{
    string path;
    string name;
    string id;
    MBR mbr;
}DiscoMontado;

typedef struct{
    char log = 'F';
    string us;
    string disco;
}Log;

class Analizador{
    public:
        Comando cmd;
        Log logueado;
        vector<DiscoMontado> discos;
        MBR leermbr(string ruta);
        vector<string> split_txt(string entrada, char c);
        string replace_txt(string str, const string& from, const string& to);
        void analizar(string entrada);
        void exec(string ruta);
        void identificarParametros(string comando,vector<string> parametros);
};

#endif // ANALIZADOR_H