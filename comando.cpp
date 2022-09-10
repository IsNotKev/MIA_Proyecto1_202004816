#include "comando.h"

void Comando::identificacionCMD(Parametros p){
    if(p.Comando=="mkdisk"){
        if(p.Tamano != " " && p.Ruta != " "){ 
            cout << "   > Ejecutando MKDISK" << endl;
            crearArchivo(p);
        }else{
            cout << "   > Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "rmdisk"){
        if(p.Ruta != " "){ 
            cout << "   > Ejecutando RMDISK" << endl;
            system(("rm -i " + p.Ruta).c_str());
        }else{
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "fdisk"){
        if(p.Tamano != " " && p.Ruta != " " && p.Name != " "){ 
            cout << "   > Ejecutando FDISK" << endl;
            crearParticion(p);
        }else{
            cout << "Error administrando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "mount"){
        if(p.Ruta != " " && p.Name != " "){ 
            cout << "   > Ejecutando MOUNT" << endl;
        }else{
            cout << "Error montando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "unmount"){
        if(p.Name != " "){ 
            cout << "   > Ejecutando UNMOUNT" << endl;
        }else{
            cout << "Error desmontando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "mkfs"){
        if(p.Name != " "){ 
            cout << "   > Ejecutando MKFS" << endl;
        }else{
            cout << "Error formateando Partición: Parametros obligatorios no definidos." << endl;
        }
    }else if(p.Comando == "pause"){
        cout << "Ejecutando Pausa. Presione Enter para continuar." << endl;
        getchar();
    }
}


void Comando::crearUser(string ruta){

    string text("1, G, root\n1, U, root, root, 123\n");

    vector<string> rutas = split_txt(ruta,'/');
    string rr = "";
    for(int i = 1; i<rutas.size()-2; i++){    
        rr = rr + "/" + rutas.at(i);    
    }
    rr += "/users.txt";
    //Creando Dot
    string filename2(rr);
    fstream outfile;
    cout << rr << endl;
    outfile.open(filename2, std::ios_base::out);
    if (!outfile.is_open()) {
        cout << "failed to open " << filename2 << '\n';
    } else {
        outfile.write(text.data(), text.size());
        outfile.close();
    }
}

void Comando::reporteDisk(Parametros p, MBR mbr){
    vector<Partition> particiones;
    particiones.push_back(mbr.part1);
    particiones.push_back(mbr.part2);
    particiones.push_back(mbr.part3);
    particiones.push_back(mbr.part4);

    string text("digraph G {\n     node [shape=record];\n    A[label =<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\"><TR><TD BGCOLOR=\"yellow\">MBR</TD>\n");

    int inicio = sizeof(mbr) + 1;

    for(int i = 0; i< 4 ; i++){
        if(particiones.at(i).active == 'V'){

            if(inicio!=particiones.at(i).start){
                float t = particiones.at(i).start;
                t = t-inicio;
                float tt = mbr.tamano;
                float percent = t/tt;
                percent = percent * 100;           
                text += "<TD>Libre<br/> "+to_string(percent)+"%</TD>\n";
            }

            if(particiones.at(i).tipo == 'P'){    
                float t = particiones.at(i).tamano;
                float tt = mbr.tamano;
                float percent = t/tt;
                percent = percent * 100;           
                text += "<TD>Primaria<br/> "+to_string(percent)+"%</TD>\n";
            }else if (particiones.at(i).tipo == 'E'){
                float t = particiones.at(i).tamano;
                float tt = mbr.tamano;
                float percent = t/tt;
                percent = percent * 100;
                text += "<TD><TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\"><TR><TD COLSPAN=\"40\">Extendida "+to_string(percent)+"%</TD></TR><TR>\n";
                for(int j = 0 ; j<20;j++){
                    if(particiones.at(i).logicas[j].status == 'V'){
                        float t = particiones.at(i).logicas[j].size;
                        float tt = mbr.tamano;
                        float percent = t/tt;
                        percent = percent * 100;
                        text += "<TD>EBR</TD>\n";
                        text += "<TD>Lógica<br/> "+to_string(percent)+"%</TD>\n";
                    }                  
                }              
                text += "</TR></TABLE></TD>\n";
            }
            inicio = particiones.at(i).start+particiones.at(i).tamano+1;
        }else if(i==3){
            float t = mbr.tamano;
            t = t-inicio;
            float tt = mbr.tamano;
            float percent = t/tt;
            percent = percent * 100;           
            text += "<TD>Libre<br/> "+to_string(percent)+"%</TD>\n";
        }
        
    }


    text += "</TR></TABLE>>];\n}";

    // Creando Carpetas
    vector<string> rutas = split_txt(p.Ruta,'/');
    string rr = "";
    for(int i = 0; i< rutas.size()-1; i++){    
        if(i!=0){
            rr = rr + "/" + rutas.at(i);
        }       
    }
    system(("mkdir -pv " + rr).c_str());

    //Creando Dot
    string filename2("mbr.dot");
    fstream outfile;

    outfile.open(filename2, std::ios_base::out);
    if (!outfile.is_open()) {
        //cout << "failed to open " << filename2 << '\n';
    } else {
        outfile.write(text.data(), text.size());
        outfile.close();
        //Graphviz y abrir
        system(("dot -Tpng mbr.dot -o " + p.Ruta).c_str());
        system(("eog " + p.Ruta).c_str());
    }

    cout << "\nPresione Enter Para Continuar." << endl;
	getchar();
}

void Comando::reporteMBR(Parametros p, MBR mbr){
    string text("digraph G {\n     node [shape=record];\n    A[label =<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\"><TR><TD BGCOLOR=\"purple\">REPORTE DE MBR</TD><TD BGCOLOR=\"purple\">               </TD></TR>\n");
    
    string f = ctime(&mbr.tiempo);

    text += "<TR><TD>mbr_tamano</TD><TD>"+to_string(mbr.tamano)+"</TD></TR>\n";
    text += "<TR><TD>mbr_fecha_creacion</TD><TD>"+f+"</TD></TR>\n";
    text += "<TR><TD>mbr_disk_signature</TD><TD>"+to_string(mbr.signature)+"</TD></TR>\n";


    int cant = 0;
    string textra = "";

    vector<Partition> particiones;
    particiones.push_back(mbr.part1);
    particiones.push_back(mbr.part2);
    particiones.push_back(mbr.part3);
    particiones.push_back(mbr.part4);

    for(int i = 0;i<4;i++){
        textra = "";
        text += "<TR><TD BGCOLOR=\"purple\">Particion</TD><TD BGCOLOR=\"purple\">               </TD></TR>\n";
        f = particiones.at(i).active;
        text += "<TR><TD>part_status</TD><TD>"+f+"</TD></TR>\n";

        if(f=="F"){
            text += "<TR><TD>part_type</TD><TD> </TD></TR>\n";
            text += "<TR><TD>part_fit</TD><TD> </TD></TR>\n";
            text += "<TR><TD>part_start</TD><TD> </TD></TR>\n";
            text += "<TR><TD>part_size</TD><TD> </TD></TR>\n";
            text += "<TR><TD>part_name</TD><TD> </TD></TR>\n";
        }else{
            f = particiones.at(i).tipo;
            if(f == "E"){
                for(int j = 0;j<20;j++){
                    if(particiones.at(i).logicas[j].status == 'V'){
                        textra += "<TR><TD BGCOLOR=\"yellow\">Particion Logica</TD><TD BGCOLOR=\"yellow\">               </TD></TR>\n";
                        textra += "<TR><TD>part_status</TD><TD> V </TD></TR>\n";
                        if(particiones.at(i).logicas[j].next == -1){
                            textra += "<TR><TD>part_next</TD><TD>"+to_string(particiones.at(i).logicas[j].next)+"</TD></TR>\n";
                        }else{
                            textra += "<TR><TD>part_next</TD><TD>"+to_string(particiones.at(i).logicas[j].next + particiones.at(i).start)+"</TD></TR>\n";
                        }
                        
                        f = particiones.at(i).logicas[j].fit;
                        textra += "<TR><TD>part_fit</TD><TD>"+f+"</TD></TR>\n";
                        textra += "<TR><TD>part_start</TD><TD>"+to_string(particiones.at(i).logicas[j].start + particiones.at(i).start)+"</TD></TR>\n";
                        textra += "<TR><TD>part_size</TD><TD>"+to_string(particiones.at(i).logicas[j].size)+"</TD></TR>\n";
                        f = particiones.at(i).logicas[j].name;
                        textra += "<TR><TD>part_name</TD><TD>"+f+"</TD></TR>\n";
                    }                   
                }
            }
            f = particiones.at(i).tipo;
            text += "<TR><TD>part_type</TD><TD>"+f+"</TD></TR>\n";
            f = particiones.at(i).fit;
            text += "<TR><TD>part_fit</TD><TD>"+f+"</TD></TR>\n";
            text += "<TR><TD>part_start</TD><TD>"+to_string(particiones.at(i).start)+"</TD></TR>\n";
            text += "<TR><TD>part_size</TD><TD>"+to_string(particiones.at(i).tamano)+"</TD></TR>\n";
            f = particiones.at(i).name;
            text += "<TR><TD>part_name</TD><TD>"+f+"</TD></TR>\n";
            text += textra;
        }     
    }
    
    text += "</TABLE>>];\n}";

    // Creando Carpetas
    vector<string> rutas = split_txt(p.Ruta,'/');
    string rr = "";
    for(int i = 0; i< rutas.size()-1; i++){    
        if(i!=0){
            rr = rr + "/" + rutas.at(i);
        }       
    }
    system(("mkdir -pv " + rr).c_str());

    //Creando Dot
    string filename2("mbr.dot");
    fstream outfile;

    outfile.open(filename2, std::ios_base::out);
    if (!outfile.is_open()) {
        //cout << "failed to open " << filename2 << '\n';
    } else {
        outfile.write(text.data(), text.size());
        outfile.close();
        //Graphviz y abrir
        system(("dot -Tpng mbr.dot -o " + p.Ruta).c_str());
        system(("eog " + p.Ruta).c_str());
    }

    cout << "\nPresione Enter Para Continuar." << endl;
	getchar();
}

void Comando::crearParticion(Parametros p){
    FILE *archivo;
    MBR n;
    archivo = fopen(p.Ruta.c_str(),"rb+");
    fseek(archivo,0,SEEK_SET);
    fread(&n,sizeof(MBR),1,archivo);
    fclose(archivo);

    if(p.Delete != " "){
        if(p.Delete == "full" || p.Delete == "fast"){
            vector<Partition> particiones;
            particiones.push_back(n.part1);
            particiones.push_back(n.part2);
            particiones.push_back(n.part3);
            particiones.push_back(n.part4);

            bool encontrado = false;

            for(int i = 0;i <4;i++){
                if(particiones.at(i).name == p.Name){
                    particiones.at(i) = Partition();
                    encontrado = true;
                    break;
                }
            }
            
            if(!encontrado){
                for(int i = 0;i <4;i++){
                    if(particiones.at(i).tipo == 'E'){
                        for(int j = 0; j<20 ; j++){
                            if(particiones.at(i).logicas[j].name == p.Name){
                                particiones.at(i).logicas[j] = EBR();
                                encontrado = true;
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            if(!encontrado){
                cout << "   > Particion " << p.Name << " no encontrada." << endl;
            }

            n.part1 = particiones.at(0);
            n.part2 = particiones.at(1);
            n.part3 = particiones.at(2);
            n.part4 = particiones.at(3);
        }else{
            cout << "Parametro en delete no reconocido." << endl;
        }
    }else if(p.Add != " "){
        // Calculo Tamaño Agregado
        int size_file = 0;
        int tamano = atoi(p.Add.c_str());

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

        vector<Partition> particiones;
        particiones.push_back(n.part1);
        particiones.push_back(n.part2);
        particiones.push_back(n.part3);
        particiones.push_back(n.part4);

        bool encontrado = false;

        for(int i = 0; i<4; i++){
            if(particiones.at(i).active == 'V'){
                if(particiones.at(i).name == p.Name){
                    encontrado = true;
                    if(i==3){
                        int tt = particiones.at(i).tamano+tamano+particiones.at(i).start;
                        if (tt<=n.tamano && particiones.at(i).tamano+tamano>=0){
                            particiones.at(i).tamano += tamano;
                            break;
                        }else{
                            cout << "   > No se puede agregar más espacio" << endl;
                        }
                    }else{
                        int fin = 0;
                        for(int j = i+1; j<4 ; j++){
                            if(particiones.at(j).active == 'V'){
                                fin = particiones.at(j).start;
                                break;
                            }else if (j==3){
                                fin = n.tamano;
                                break;
                            }                            
                        }
                        int tt = particiones.at(i).tamano;
                        tt += tamano;
                        tt += particiones.at(i).start;
                        cout << tt << "-" << fin << endl;
                        if(tt<=fin && particiones.at(i).tamano + tamano >= 0){
                            particiones.at(i).tamano += tamano;
                            break;
                        }else{
                            cout << "   > No se puede agregar más espacio" << endl;
                        }
                    }
                }
            }
        }

        if(!encontrado){
            for(int i = 0; i<4 ; i++){
                if(particiones.at(i).active == 'V'){
                    if(particiones.at(i).tipo == 'E'){
                        for(int j = 0 ; j<20 ; j++){
                            if(particiones.at(i).logicas[j].status == 'V'){
                                if (particiones.at(i).logicas[j].name == p.Name){
                                    
                                }                               
                            }
                        }
                        break;
                    }
                }
            }
        }


        n.part1 = particiones.at(0);
        n.part2 = particiones.at(1);
        n.part3 = particiones.at(2);
        n.part4 = particiones.at(3);

    }else{
        Partition nuevaP;
        nuevaP.active = 'V';

        // Calculo Tamaño de Particion
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


        char tt = p.t_particion.at(0);

        if(tt == 'E' || tt == 'L' || tt == 'P'){
            nuevaP.tipo = tt;
        }else if(tt == ' '){
            nuevaP.tipo = 'P';
        }else{
            cout << "Error ->  Tipo de particion no reconocido" << endl;
        }

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

        if(nuevaP.tipo == 'L'){
            EBR nuevoEbr;
            nuevoEbr.status = 'V';
            nuevoEbr.fit = nuevaP.tipo;
            nuevoEbr.size = nuevaP.tamano;
            nuevoEbr.next = -1;
            strcpy(nuevoEbr.name,p.Name.c_str());

            if(n.part1.tipo == 'E'){
                for(int i = 0; i<20;i++){
                    if(n.part1.logicas[i].status == 'F'){
                        if(i>0){
                            int ultimo = i-1;
                            nuevoEbr.start = n.part1.logicas[ultimo].start + n.part1.logicas[ultimo].size +1;
                            if(nuevoEbr.start + nuevoEbr.size <= n.part1.tamano){
                                n.part1.logicas[ultimo].next = n.part1.logicas[ultimo].start + n.part1.logicas[ultimo].size+1;
                                n.part1.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }else{
                            nuevoEbr.start = 0;
                            if(nuevoEbr.size <= n.part1.tamano){
                                n.part1.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }
                        break;
                    }
                }           

            }else if (n.part2.tipo == 'E'){
                for(int i = 0; i<20;i++){
                    if(n.part2.logicas[i].status == 'F'){
                        if(i>0){
                            int ultimo = i-1;
                            nuevoEbr.start = n.part2.logicas[ultimo].start + n.part2.logicas[ultimo].size + 1;
                            if(nuevoEbr.start + nuevoEbr.size <= n.part2.tamano){
                                n.part2.logicas[ultimo].next = n.part2.logicas[ultimo].start + n.part2.logicas[ultimo].size+1;
                                n.part2.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }else{
                            nuevoEbr.start = 0;
                            if(nuevoEbr.size <= n.part2.tamano){
                                n.part2.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }
                        break;
                    }
                }
            }else if (n.part3.tipo == 'E'){
                for(int i = 0; i<20;i++){
                    if(n.part3.logicas[i].status == 'F'){
                        if(i>0){
                            int ultimo = i-1;
                            nuevoEbr.start = n.part3.logicas[ultimo].start + n.part3.logicas[ultimo].size +1;
                            if(nuevoEbr.start + nuevoEbr.size <= n.part3.tamano){
                                n.part3.logicas[ultimo].next = n.part3.logicas[ultimo].start + n.part3.logicas[ultimo].size+1;
                                n.part3.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }else{
                            nuevoEbr.start = 0;
                            if(nuevoEbr.size <= n.part3.tamano){
                                n.part3.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }
                        break;
                    }
                }
            }else if (n.part4.tipo == 'E'){
                for(int i = 0; i<20;i++){
                    if(n.part4.logicas[i].status == 'F'){
                        if(i>0){
                            int ultimo = i-1;
                            nuevoEbr.start = n.part4.logicas[ultimo].start + n.part4.logicas[ultimo].size +1;
                            if(nuevoEbr.start + nuevoEbr.size <= n.part4.tamano){
                                n.part4.logicas[ultimo].next = n.part4.logicas[ultimo].start + n.part4.logicas[ultimo].size+1;
                                n.part4.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }else{
                            nuevoEbr.start = 0;
                            if(nuevoEbr.size <= n.part4.tamano){
                                n.part4.logicas[i] = nuevoEbr;
                            }else{
                                cout << "No cabe nueva particion logica" << endl;
                            }
                        }
                        break;
                    }
                }
            }else{
                cout << "Error -> No existe particion extendida" << endl;
            }
            
        }else{
            vector<Partition> particiones;
            particiones.push_back(n.part1);
            particiones.push_back(n.part2);
            particiones.push_back(n.part3);
            particiones.push_back(n.part4);

            if(n.part1.active == 'F' && n.part2.active == 'F' && n.part3.active == 'F' && n.part4.active == 'F'){
                if(sizeof(n)+nuevaP.tamano <= n.tamano){
                    nuevaP.start = sizeof(n) + 1;
                    n.part1 = nuevaP;
                }else{
                    cout << "No cabe " << nuevaP.name << endl;
                }
                
            }else if(n.part1.active == 'V' && n.part2.active == 'V' && n.part3.active == 'V' && n.part4.active == 'V'){
                cout << "Error -> No se puede crear mas de 4 particiones" << endl;
            }else if((nuevaP.tipo == 'E' && n.part1.tipo == 'E') || (nuevaP.tipo == 'E' && n.part2.tipo == 'E')||(nuevaP.tipo == 'E' && n.part3.tipo == 'E') || (nuevaP.tipo == 'E' && n.part4.tipo == 'E')){
                cout << "Error -> No se puede crear mas de 1 particion extendida" << endl;
            }else if(n.fit == 'B'){
                vector<vector<int>> espacios = encontrarEspacio(sizeof(n),n.tamano,nuevaP.tamano,particiones);
                vector<int> ubicacion;
                ubicacion.push_back(0);
                ubicacion.push_back(0);

                
                
                if(espacios.size() > 0){
                    //Eligiendo mejor espacio
                    for(int i = 0; i<espacios.size(); i++){
                        if((ubicacion.at(1)-ubicacion.at(0))>((espacios.at(i).at(1)-espacios.at(i).at(0)))){
                            ubicacion = espacios.at(i);
                        }
                    }

                    if(ubicacion.at(1) == n.tamano && particiones.at(3).active == 'V'){
                        vector<Partition> aux;
                        int cont = 0;
                        while (cont<4){
                            if(particiones.at(cont).active == 'V'){
                                aux.push_back(particiones.at(cont));
                            }
                            cont++;
                        }
                        aux.push_back(nuevaP);
                        particiones = aux;
                    }
                    int inicio = sizeof(n);               
                    for(int i = 0 ; i < 4; i++){
                        
                        if(inicio == ubicacion.at(0) && particiones.at(i).active == 'F'){
                            nuevaP.start = inicio + 1;
                            particiones.at(i) = nuevaP;
                            break;
                        }else if (particiones.at(i).active == 'V'){
                            if(ubicacion.at(1) == particiones.at(i).start){
                                nuevaP.start = inicio + 1;
                                cout << "Llego a insertar" << endl;
                                particiones = insertarParticion(i,particiones,nuevaP);
                                break;
                            }
                            inicio = particiones.at(i).start + particiones.at(i).tamano;                       
                        }
                        
                    }

                    n.part1 = particiones.at(0);
                    n.part2 = particiones.at(1);
                    n.part3 = particiones.at(2);
                    n.part4 = particiones.at(3);
                }else{
                    cout << "No cabe " << nuevaP.name << endl;
                }

            }else if(n.fit == 'F'){
                vector<vector<int>> espacios = encontrarEspacio(sizeof(n),n.tamano,nuevaP.tamano,particiones);
                //Eligiendo primer espacio
                if(espacios.size() > 0){
                    vector<int> ubicacion = espacios.at(0);

                    if(ubicacion.at(1) == n.tamano && particiones.at(3).active == 'V'){
                        vector<Partition> aux;
                        int cont = 0;
                        while (cont<4){
                            if(particiones.at(cont).active == 'V'){
                                aux.push_back(particiones.at(cont));
                            }
                            cont++;
                        }
                        aux.push_back(nuevaP);
                        particiones = aux;
                    }
                    int inicio = sizeof(n);               
                    for(int i = 0 ; i < 4; i++){
                        
                        if(inicio == ubicacion.at(0) && particiones.at(i).active == 'F'){
                            nuevaP.start = inicio + 1;
                            particiones.at(i) = nuevaP;
                            break;
                        }else if (particiones.at(i).active == 'V'){
                            if(ubicacion.at(1) == particiones.at(i).start){
                                nuevaP.start = inicio + 1;
                                cout << "Llego a insertar" << endl;
                                particiones = insertarParticion(i,particiones,nuevaP);
                                break;
                            }
                            inicio = particiones.at(i).start + particiones.at(i).tamano;                       
                        }
                        
                    }

                    n.part1 = particiones.at(0);
                    n.part2 = particiones.at(1);
                    n.part3 = particiones.at(2);
                    n.part4 = particiones.at(3);
                }else{
                    cout << "No cabe " << nuevaP.name << endl;
                }
                
            }else if(n.fit == 'W'){
                vector<vector<int>> espacios = encontrarEspacio(sizeof(n),n.tamano,nuevaP.tamano,particiones);
                vector<int> ubicacion;
                ubicacion.push_back(0);
                ubicacion.push_back(0);           

                if(espacios.size() > 0){
                    //Eligiendo peor espacio
                    for(int i = 0; i<espacios.size(); i++){
                        if((ubicacion.at(1)-ubicacion.at(0))<((espacios.at(i).at(1)-espacios.at(i).at(0)))){
                            ubicacion = espacios.at(i);
                        }
                    }

                    if(ubicacion.at(1) == n.tamano && particiones.at(3).active == 'V'){
                        vector<Partition> aux;
                        int cont = 0;
                        while (cont<4){
                            if(particiones.at(cont).active == 'V'){
                                aux.push_back(particiones.at(cont));
                            }
                            cont++;
                        }
                        aux.push_back(nuevaP);
                        particiones = aux;
                    }
                    int inicio = sizeof(n);               
                    for(int i = 0 ; i < 4; i++){
                        
                        if(inicio == ubicacion.at(0) && particiones.at(i).active == 'F'){
                            nuevaP.start = inicio + 1;
                            particiones.at(i) = nuevaP;
                            break;
                        }else if (particiones.at(i).active == 'V'){
                            if(ubicacion.at(1) == particiones.at(i).start){
                                nuevaP.start = inicio + 1;
                                cout << "Llego a insertar" << endl;
                                particiones = insertarParticion(i,particiones,nuevaP);
                                break;
                            }
                            inicio = particiones.at(i).start + particiones.at(i).tamano;                       
                        }
                        
                    }

                    n.part1 = particiones.at(0);
                    n.part2 = particiones.at(1);
                    n.part3 = particiones.at(2);
                    n.part4 = particiones.at(3);
                }else{
                    cout << "No cabe " << nuevaP.name << endl;
                }
            }
        }

        

    }

    //Escribiendo MBR nuevo

    FILE *aa;
    aa = fopen(p.Ruta.c_str(),"rb+");
	fseek(aa,0,SEEK_SET);
	fwrite(&n,sizeof(n),1,aa);
	fclose(aa);

}

vector<Partition> Comando::insertarParticion(int num, vector<Partition> particiones, Partition nueva){
    vector<Partition> ordenado;

    int cont = 0;

    while (ordenado.size() < 4){
        if(particiones.at(cont).active == 'F' && cont==num){
            ordenado.push_back(nueva);
        }else if(particiones.at(cont).active == 'V' && cont==num){
            ordenado.push_back(nueva);
            ordenado.push_back(particiones.at(cont));
        }else{
            ordenado.push_back(particiones.at(cont));
        }
        cont++;
    }


    return ordenado;
}

vector<vector<int>> Comando::encontrarEspacio(int inicio, int tamano_total, int tamano, vector<Partition> particiones){
    vector<vector<int>> espacios;
    int initTemp = inicio;
    for(int i = 0;i<4;i++){
        vector<int> ss;
        if(particiones.at(i).active == 'V'){
            if(particiones.at(i).start-initTemp >= tamano && particiones.at(i).start-initTemp > 1){
                ss.push_back(initTemp);
                ss.push_back(particiones.at(i).start);
                espacios.push_back(ss);              
            }
            initTemp = particiones.at(i).start + particiones.at(i).tamano; 
        }
    }
    vector<int> ss;
    if(tamano_total-initTemp >= tamano){
        ss.push_back(initTemp);
        ss.push_back(tamano_total);
        espacios.push_back(ss);
    }

    return espacios;
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
    nuevo.signature = rand()%100;
    
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
