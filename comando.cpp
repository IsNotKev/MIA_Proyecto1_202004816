#include "comando.h"

void Comando::identificacionCMD(Parametros p){
    if(p.Comando=="mkdisk"){ // Se identifica el tipo de comando
        if(p.Tamano != " " && p.Ruta != " "){ // Se validan los parametros para el comando
            cout << "Ejecutando MKDISK" << endl;
            crearArchivo(p);
        }else{
            cout << "Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "rmdisk"){
        if(p.Ruta != " "){ // Se validan los parametros para el comando
            cout << "Ejecutando RMDISK" << endl;
            system(("rm -i " + p.Ruta).c_str());
        }else{
            cout << "Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }
}

void Comando::crearArchivo(Parametros p){
    // Calculo Tamaño del Archivo
    int size_file = 0;
    int tamano = atoi(p.Tamano.c_str());

    // Dimensional
    char dimen = p.Dimensional.at(0);
    if (dimen == 'k' || dimen == 'K'){
        size_file = tamano;
    }else if (dimen == 'm' || dimen == 'M' || dimen == ' '){
        size_file = tamano * 1024;
    }else if (dimen == 'g' || dimen == 'G'){
        size_file = tamano * 1024 * 1024;
    }else{
        cout << "Error -> dimensional no reconocida." << endl;
    }

    // Preparacion Bloque
    char bloque[1024];
    for (int i = 0; i < 1024; i++){
        bloque[i] = '\0';
    }

    // Escritura de Bloque en Archivo
    int limite = 0;
    FILE *archivo_binario;
    cout << p.Ruta << endl;
    archivo_binario = fopen(p.Ruta.c_str(), "w");
    while (limite != size_file){
        fwrite(&bloque, 1024, 1, archivo_binario);
        limite++;
    }
    fclose(archivo_binario);

    // *********************************** Pendiente usar el fit ***********************
}