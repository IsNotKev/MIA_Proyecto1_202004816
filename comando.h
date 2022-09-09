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
#include <cmath>

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
    string id = " ";
    string ruta = " ";
}Parametros;

typedef struct{
    char status = 'F';
    char fit;
    int start;
    int size;
    int next;
    char name[16];
}EBR;

typedef struct{
    char active = 'F';
    char tipo;
    char fit;
    int start;
    int tamano = 0;
    char name[16];
    EBR logicas[20];
}Partition;

typedef struct{
    int tamano = 0;
    time_t tiempo;
    int signature;
    char fit;
    Partition part1;
    Partition part2;
    Partition part3;
    Partition part4;
    char name[20];
}MBR;

class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(Parametros p);
        vector<string> split_txt(string entrada, char c);
        void crearParticion(Parametros p);
        vector<vector<int>> encontrarEspacio(int inicio, int tamano_total, int tamano, vector<Partition> particiones);
        vector<Partition> insertarParticion(int num, vector<Partition> particiones, Partition nueva);
        void reporteMBR(Parametros p, MBR mbr);
        void reporteDisk(Parametros p, MBR mbr);
        //void escribir(string id, string nombre, string tel, string dir, string x);
        //void vertodo();
        //void verX(string x);
        //void eliminarX(string x);
        //void mostrar_struct(Ejemplo ejm);
};

#endif // COMANDO_H