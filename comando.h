#ifndef COMANDO_H
#define COMANDO_H

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct{
    string Comando = " ";
    string Tamano = " ";
    string Ajuste = " ";
    string Dimensional = " ";
    string Ruta = " ";
    string t_particion = " ";
    string Delete = " ";
    string Add = " ";
    string Name = " ";
    string Formateo = " ";
    string t_formateo = " ";
}Parametros;

typedef struct{
    char active = 'F';
    char tipo;
    char fit;
    int start;
    int tamano;
    char name[16];
}Partition;

typedef struct{
    int tamano;
    time_t tiempo;
    int signature;
    char fit;
    Partition part1;
    Partition part2;
    Partition part3;
    Partition part4;
}MBR;

typedef struct{
    char status;
    char fit;
    int start;
    int size;
    int next;
    char name[16];
}EBR;

class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(Parametros p);
        vector<string> split_txt(string entrada, char c);
        void crearParticion(Parametros p);
        //void escribir(string id, string nombre, string tel, string dir, string x);
        //void vertodo();
        //void verX(string x);
        //void eliminarX(string x);
        //void mostrar_struct(Ejemplo ejm);
};

#endif // COMANDO_H