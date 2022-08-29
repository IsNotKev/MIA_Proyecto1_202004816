#ifndef COMANDO_H
#define COMANDO_H

#include <iostream>

using namespace std;

typedef struct{
    string Comando = " ";
    string Tamano = " ";
    string Ajuste = " ";
    string Dimensional = " ";
    string Ruta = " ";
}Parametros;

typedef struct{
    char active;
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

class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(Parametros p);
        //void escribir(string id, string nombre, string tel, string dir, string x);
        //void vertodo();
        //void verX(string x);
        //void eliminarX(string x);
        //void mostrar_struct(Ejemplo ejm);
};

#endif // COMANDO_H