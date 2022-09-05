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
            } else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else{
                cout << "Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if(comando == "rmdisk"){                                  // RMDISK
        cmd.param.Comando = "rmdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Ruta = param;
            }else{
                cout << "Parametro " << parametros.at(i) << " x" << endl;
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
                cout << "Parametro " << parametros.at(i) << " x" << endl;
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
                cout << "Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if(comando == "unmount"){                                         // UNMOUNT
        cmd.param.Comando = "unmount";  
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                cmd.param.Name = param;
            }else{
                cout << "Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if(comando == "mkfs"){                                         // MKFS
        cmd.param.Comando = "mkfs";  
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id->") == 0){
                param = replace_txt(param, "-id->", "");
                cmd.param.Name = param;
            }if(param.find("-type->") == 0){
                param = replace_txt(param, "-type->", "");
                cmd.param.t_formateo = param;
            }if(param.find("-fs->") == 0){
                param = replace_txt(param, "-fs->", "");
                cmd.param.Formateo = param;
            }else{
                cout << "Parametro " << parametros.at(i) << " x" << endl;
            }
        }
        // Ejecucion de metodo
        cmd.identificacionCMD(cmd.param);
    }else if (comando == "pause"){       
        cmd.param.Comando = "pause";                                     // PAUSE
        cmd.identificacionCMD(cmd.param);
    }else if (comando == "exit"){                                           // EXIT
        cout << "Byee." << endl;
    }else{
        cout << "Comando -" << comando << "- No Reconocido.\n" << endl;
    }
}