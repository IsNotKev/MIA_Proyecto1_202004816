#include "analizador.h"

vector<string> Analizador::split_txt(string text, char c){ // Split para separar tipo de comando y parametros
    stringstream text_to_split(text);
    string segment;
    vector<string> splited;

    while(std::getline(text_to_split, segment, c)){
       splited.push_back(segment);
    }
    return splited;
}

string Analizador::replace_txt(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void Analizador::analizar(string entrada){
    string entradacmd = entrada;
    // Transformacion de toda la entrada a minusculas
    // transform(entradacmd.begin(), entradacmd.end(), entradacmd.begin(), ::tolower);
    vector<string> cmdentrada = split_txt(entradacmd, ' ');
    vector<string> parametros;
    string comando = "";
    for(int i = 0; i< cmdentrada.size(); i++){
        if(i == 0){
            comando = cmdentrada.at(i);
        }else{
            parametros.push_back(cmdentrada.at(i));
        }
    }
    // Identificacion de comando, parametros y ejecucion
    identificarParametros(comando, parametros);
}

void Analizador::identificarParametros(string comando, vector<string> parametros){
    string param = "";
    transform(comando.begin(), comando.end(), comando.begin(), ::tolower);
    cmd = Comando();
    if(comando == "mkdisk"){                                      // MKDISK
        cmd.param.Comando = "mkdisk";
        // Guardado de parametros en Struct
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                cmd.param.Tamano = param;
            } else if(param.find("-f->") == 0){
                param = replace_txt(param, "-f->", "");
                cmd.param.Ajuste = param;
            } else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                cmd.param.Dimensional = param;
            } else if(param.find("-path->\"") == 0){
                param = replace_txt(param, "-path->", "");

                string ruta = param;
                int cont = i+1;
                while(ruta.at(ruta.size() -1) != '\"'){
                    ruta += " " + parametros.at(cont);
                    cont++;
                }

                i = cont -1;

                ruta = replace_txt(ruta, "\"", "");
                cmd.param.Ruta = ruta;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if(comando == "rmdisk"){                                  // RMDISK
        cmd.param.Comando = "rmdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->\"") == 0){
                param = replace_txt(param, "-path->", "");

                string ruta = param;
                int cont = i+1;
                while(ruta.at(ruta.size() -1) != '\"'){
                    ruta += " " + parametros.at(cont);
                    cont++;
                }

                i = cont -1;

                ruta = replace_txt(ruta, "\"", "");
                cmd.param.Ruta = ruta;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if (comando == "fdisk"){                                 // FDISK
        cmd.param.Comando = "fdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                cmd.param.Tamano = param;
            } else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                cmd.param.Dimensional = param;
            } else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else if(param.find("-t->") == 0){
                param = replace_txt(param, "-t->", "");
                cmd.param.t_particion = param;
            }else if(param.find("-f->") == 0){
                param = replace_txt(param, "-f->", "");
                cmd.param.Ajuste = param;
            }else if(param.find("-delete->") == 0){
                param = replace_txt(param, "-delete->", "");
                cmd.param.Delete = param;
            }else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                cmd.param.Name = param;
            }else if(param.find("-add->") == 0){
                param = replace_txt(param, "-add->", "");
                cmd.param.Add = param;
            }else{
                cout << "    > Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if(comando == "mount"){                                           // MOUNT
        cmd.param.Comando = "mount";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                cmd.param.Name = param;
            }else{
                cout << " > Parametro " << parametros.at(i) << " x" << endl;
            }
        }

        // Ejecucion de metodo
        if(cmd.param.Ruta != " " && cmd.param.Name != " "){
            cout << "   > Ejecutando Mount" << endl;
            MBR nuevo = leermbr(cmd.param.Ruta);
            if(nuevo.tamano > 0){
                DiscoMontado nn;
                nn.mbr = nuevo;
                nn.path = cmd.param.Ruta;
                int cont = 1;
                for(int i = 0; i<discos.size();i++){
                    if(discos.at(i).path == nn.path){
                        cont++;
                    }
                }
                nn.name = cmd.param.Name;

                vector<string> nombres = split_txt(cmd.param.Ruta, '/');
                vector<string> name = split_txt(nombres.at(nombres.size()-1),'.');
                string nombre = name.at(0);
                nn.id = "16";
                nn.id = nn.id + to_string(cont);
                nn.id = nn.id + nombre;
                discos.push_back(nn);
            }    

            cout << "   > Discos Montandos: " << endl;
            for(int j = 0; j< discos.size();j++){
                cout << "       - " << discos.at(j).id << endl;
            }

        }else{
            cout << "   > Error montando partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(comando == "unmount"){                                         // UNMOUNT
        cmd.param.Comando = "unmount";  
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Name = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.Name != " "){
            cout << "   > Ejecutando Unmount" << endl;
            for(int i = 0; i<discos.size();i++){
                if(discos.at(i).id == cmd.param.Name){
                    auto elem_to_remove = discos.begin() + i;
                    discos.erase(elem_to_remove);
                    cout << "   > Disco " << cmd.param.Name << " Desmontado." << endl;
                    break;
                }
            }
            cout << "   > Discos Montandos: " << endl;
            for(int j = 0; j< discos.size();j++){
                cout << "       - " << discos.at(j).id << endl;
            }
        }else{
            cout << "   > Error desmontando partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(comando == "mkfs"){                                         // MKFS
        cmd.param.Comando = "mkfs";  
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Name = param;
            }else if(param.find("-type->") == 0){
                param = replace_txt(param, "-type->", "");
                cmd.param.t_formateo = param;
            }else if(param.find("-fs->") == 0){
                param = replace_txt(param, "-fs->", "");
                cmd.param.Formateo = param;
            }else{
                cout << "Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        bool encontrado = false;
        if(cmd.param.Name != " "){
            for(int i = 0; i<discos.size();i++){
                if(discos.at(i).id == cmd.param.Name){
                    if(cmd.param.t_formateo == "full" || cmd.param.t_formateo == "fast" || cmd.param.t_formateo == " "){
                        if(cmd.param.Formateo == "2fs" || cmd.param.Formateo == " "){
                            discos.at(i).mbr.part1 = Partition();
                            discos.at(i).mbr.part2 = Partition();
                            discos.at(i).mbr.part3 = Partition();
                            discos.at(i).mbr.part4 = Partition();
                            discos.at(i).mbr.fs = '2';
                            cout << "   > Disco " << cmd.param.Name << " Formateado EXT2." << endl;
                            cmd.crearUser(discos.at(i).path);
                            encontrado = true;
                        }else if (cmd.param.Formateo == "3fs"){
                            discos.at(i).mbr.part1 = Partition();
                            discos.at(i).mbr.part2 = Partition();
                            discos.at(i).mbr.part3 = Partition();
                            discos.at(i).mbr.part4 = Partition();
                            discos.at(i).mbr.fs = '3';
                            cout << "   > Disco " << cmd.param.Name << " Formateado EXT3." << endl;
                            cmd.crearUser(discos.at(i).path);
                            encontrado = true;
                        }else{
                            cout << "   > Sistema de archivo no encontrado." << endl;
                        }
                        break;
                    }else{
                        cout << "   > Tipo de formateo no encontrado" << endl;
                    }
                }
            }

            if(!encontrado){
                cout << "   > Disco No Encontrado." << endl;
            }

        }else{
            cout << "   > Error formateando partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(comando == "rep"){                                                 //REPORTE
        cmd.param.Comando = "rep";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                cmd.param.Name = param;
            }else if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            }else if(param.find("-ruta->") == 0){
                param = replace_txt(param, "-ruta->", "");
                cmd.param.ruta = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }

        // Ejecucion de metodo
        bool encontrado = false;
        if(cmd.param.Name != " " && cmd.param.Ruta != " " && cmd.param.id != " "){
            for(int i = 0; i<discos.size();i++){
                if(discos.at(i).id == cmd.param.id){
                    if(cmd.param.Name == "mbr" || cmd.param.Name == "MBR"){
                        cout << "   > Ejecutando Rep" << endl;
                        cout << "   > Generando Reporte MBR" << endl;
                        cmd.reporteMBR(cmd.param,discos.at(i).mbr);
                        encontrado = true;
                        break;
                    }else if (cmd.param.Name == "disk" || cmd.param.Name == "DISK"){
                        cout << "   > Ejecutando Rep" << endl;
                        cout << "   > Generando Reporte Disk" << endl;
                        cmd.reporteDisk(cmd.param,discos.at(i).mbr);
                        encontrado = true;
                        break;
                    }
                                                      
                }
            }
            if(!encontrado){
                cout << "   > Disco " << cmd.param.id <<  " No encontrado" <<endl;
            }
        }else{
            cout << "   > Error en Reporte: Parametros obligatorios no definidos." << endl;
        }
    }else if (comando == "pause"){       
        cmd.param.Comando = "pause";                                     // PAUSE
        cmd.identificacionCMD(cmd.param);
    }else if (comando == "exec"){                                      // Exec
        cmd.param.Comando = "exec";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
             if(param.find("-path->\"") == 0){
                param = replace_txt(param, "-path->", "");

                string ruta = param;
                int cont = i+1;
                while(ruta.at(ruta.size() -1) != '\"'){
                    ruta += " " + parametros.at(cont);
                    cont++;
                }

                i = cont -1;

                ruta = replace_txt(ruta, "\"", "");
                cmd.param.Ruta = ruta;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }

        // Ejecución del Metodo
        if(cmd.param.Ruta != " "){
            exec(cmd.param.Ruta);
        }else{
            cout << "   > Error en Exec: Parametros obligatorios no definidos." << endl;
        }

    }else if (comando == "login"){                                           // LOGIN
        cmd.param.Comando = "login";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-pass->") == 0){
                param = replace_txt(param, "-pass->", "");
                cmd.param.contra = param;
            }else if(param.find("-usr->") == 0){
                param = replace_txt(param, "-usr->", "");
                cmd.param.Name = param;
            }else if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            }else{
                cout << "   > Parametro " << parametros.at(i) << " x" << endl;
            }
        }

        // Ejecución del Metodo
        if(cmd.param.contra != " " && cmd.param.Name != " " && cmd.param.id != " "){
            if(logueado.log == 'F'){
                if(cmd.param.Name == "root" && cmd.param.contra == "123"){
                    cout << "Logueado root" << endl;
                }else{
                    cout << "   > Error en Login: Usuario y/o contraseña incorrecta." << endl;
                }
            }else{
                cout << "   > Error en Login: Existe una sesión activa.";
            }
        }else{
            cout << "   > Error en Login: Parametros obligatorios no definidos." << endl;
        }

    }else if (comando == "logout"){                                           // LOGOUT
        cmd.param.Comando = "logout";
        for(int i=0; i<parametros.size(); i++){
            cout << "   > Parametro " << parametros.at(i) << " x" << endl;
        }

        if(logueado.log == 'V'){
            logueado = Log();
        }else{
            cout << "   > Error en Logout: No hay sesión activa." << endl;
        }

    }else if (comando == "exit"){                                           // EXIT
        cout << "   > Byee." << endl;
    }else if (comando == "" || comando.at(0) == '#'){
        
    }else{
        cout << "   > Comando -" << comando << "- No Reconocido.\n" << endl;
    }
}

void Analizador::exec(string ruta){
    FILE *archivo;
    long medida;
    char *texto;
    if (archivo = fopen(ruta.c_str(), "r")){
        fseek(archivo , 0 , SEEK_END);
        medida = ftell(archivo);
        rewind(archivo);

        texto = (char*) malloc (sizeof(char)*medida);
        fread(texto, medida+1, 1, archivo);

        fclose(archivo);

        vector<string> instrucciones = split_txt(texto,'\n');

        cout << "   > Ejecutando Exec" << endl;

        for(int i = 0; i<instrucciones.size(); i++){
            cout << instrucciones.at(i) << endl;
            analizar(instrucciones.at(i));
        }

    }else{
        cout << "Archivo no existe." << ruta << endl;
    }
}

MBR Analizador::leermbr(string ruta){
    FILE *file;
    MBR ll;
    if (file = fopen(ruta.c_str(), "rb+")){
        fseek(file,0,SEEK_SET);
        fread(&ll,sizeof(MBR),1,file);
        fclose(file);
        return ll;
    }else{
        cout << "Disco Inexistente." << endl;
    }
    return ll;
}