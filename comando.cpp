#include "comando.h"

void Comando::identificacionCMD(Parametros p){
    if(p.Comando=="mkdisk"){
        if(p.Tamano != " " && p.Ruta != " "){ 
            cout << "Ejecutando MKDISK" << endl;
            crearArchivo(p);
        }else{
            cout << "Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "rmdisk"){
        if(p.Ruta != " "){ 
            cout << "Ejecutando RMDISK" << endl;
            system(("rm -i " + p.Ruta).c_str());
        }else{
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "fdisk"){
        if(p.Tamano != " " && p.Ruta != " " && p.Name != " "){ 
            cout << "Ejecutando FDISK" << endl;
            crearParticion(p);
        }else{
            cout << "Error administrando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "mount"){
        if(p.Ruta != " " && p.Name != " "){ 
            cout << "Ejecutando MOUNT" << endl;
        }else{
            cout << "Error montando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "unmount"){
        if(p.Name != " "){ 
            cout << "Ejecutando UNMOUNT" << endl;
        }else{
            cout << "Error desmontando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "mkfs"){
        if(p.Name != " "){ 
            cout << "Ejecutando MKFS" << endl;
        }else{
            cout << "Error formateando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "pause"){
        cout << "Ejecutando Pausa. Presione Enter para continuar." << endl;
        getchar();
    }
}

void Comando::crearParticion(Parametros p){
    FILE *archivo;

    MBR n;
    archivo = fopen(p.Ruta.c_str(),"rb+");
    fseek(archivo,0,SEEK_SET);
    fread(&n,sizeof(MBR),1,archivo);
    fclose(archivo);

    if(p.Delete != " "){
        if(p.Delete == "Full"){
            if(n.part1.name == p.Name){
                n.part1 = Partition();
            }else if(n.part2.name == p.Name){
                n.part2 = Partition();
            }else if(n.part3.name == p.Name){
                n.part3 = Partition();
            }else if(n.part4.name == p.Name){
                n.part4 = Partition();
            }else{
                cout << "La particion " << p.Name << " no existe" << endl;
            }
        }else{
            cout << "Parametro en delete no reconocido." << endl;
        }
    }else if(p.Add != " "){
        // Calculo Tamaño Agregado
        int size_file = 0;
        int tamano = atoi(p.Tamano.c_str());

        // Dimensional
        char dimen = p.Dimensional.at(0);
        if (dimen == 'b' || dimen == 'B'){
            tamano = tamano;
        }else if (dimen == 'k' || dimen == 'K' || dimen == ' '){
            tamano = tamano * 1024;
        }else if (dimen == 'm' || dimen == 'M'){
            tamano = tamano * 1024 * 1024;
        }else{
            cout << "Error -> dimensional no reconocida." << endl;
        }

        if(n.part1.name == p.Name){
            n.part1.tamano += tamano;
        }else if(n.part2.name == p.Name){
            n.part2.tamano += tamano;
        }else if(n.part3.name == p.Name){
            n.part3.tamano += tamano;
        }else if(n.part4.name == p.Name){
            n.part4.tamano += tamano;
        }else{
            cout << "La particion " << p.Name << " no existe" << endl;
        }

    }else{
        Partition nuevaP;
        nuevaP.active = 'V';

        // Calculo Tamaño de Particion
        int size_file = 0;
        int tamano = atoi(p.Tamano.c_str());

        // Dimensional
        char dimen = p.Dimensional.at(0);
        if (dimen == 'b' || dimen == 'B'){
            nuevaP.tamano = tamano;
        }else if (dimen == 'k' || dimen == 'K' || dimen == ' '){
            nuevaP.tamano = tamano * 1024;
        }else if (dimen == 'm' || dimen == 'M'){
            nuevaP.tamano = tamano * 1024 * 1024;
        }else{
            cout << "Error -> dimensional no reconocida." << endl;
        }

        nuevaP.tipo = p.t_particion.at(0);

        // FIT
        string f = p.Ajuste;
        if (f == "BF" || f == "bf"){
            nuevaP.fit = 'B';
        }else if (f == "FF" || f == "ff" || f == " "){
            nuevaP.fit = 'F';
        }else if (f == "WF" || f == "wf"){
            nuevaP.fit = 'W';
        }else{
            cout << "Error -> Ajuste no reconocida." << endl;
        }

        strcpy(nuevaP.name,p.Name.c_str());
    }

    FILE *archivo;
    archivo = fopen(p.Ruta.c_str(),"rb+");
	fseek(archivo,0,SEEK_SET);
	fwrite(&n,sizeof(n),1,archivo);
	fclose(archivo);

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

    // Creando Carpetas
    vector<string> rutas = split_txt(p.Ruta,'/');
    string rr = "";
    for(int i = 0; i< rutas.size()-1; i++){    
        if(i!=0){
            rr = rr + "/" + rutas.at(i);
        }       
    }
    system(("mkdir -pv " + rr).c_str());

    // Escritura de Bloque en Archivo
    int limite = 0;
    FILE *archivo_binario;
    archivo_binario = fopen(p.Ruta.c_str(), "w");
    while (limite != size_file){
        fwrite(&bloque, 1024, 1, archivo_binario);
        limite++;
    }
    fclose(archivo_binario);

    // *********************************** Escribiendo MBR Inicial ***********************
    MBR nuevo;
    nuevo.tamano = size_file*1024;
    time(&nuevo.tiempo);
    nuevo.signature = rand();
    
    // FIT
    string f = p.Ajuste;
    if (f == "BF" || f == "bf"){
        nuevo.fit = 'B';
    }else if (f == "FF" || f == "ff" || f == " "){
        nuevo.fit = 'F';
    }else if (f == "WF" || f == "wf"){
        nuevo.fit = 'W';
    }else{
        cout << "Error -> Ajuste no reconocida." << endl;
    }

    nuevo.part1 = Partition();
    nuevo.part2 = Partition();
    nuevo.part3 = Partition();
    nuevo.part4 = Partition();

    FILE *archivo;
    archivo = fopen(p.Ruta.c_str(),"rb+");
	fseek(archivo,0,SEEK_SET);
	fwrite(&nuevo,sizeof(nuevo),1,archivo);
	fclose(archivo);

}

vector<string> Comando::split_txt(string text, char c){ // Split para separar tipo de comando y parametros
    stringstream text_to_split(text);
    string segment;
    vector<string> splited;

    while(std::getline(text_to_split, segment, c)){
       splited.push_back(segment);
    }
    return splited;
}
