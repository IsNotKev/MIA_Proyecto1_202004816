#include <iostream>
#include <sstream>

using namespace std;
using std::stoi;

#include "analizador.cpp"

int main(){
	Analizador analisis;

	string entradacmd = "";

	while(entradacmd != "exit"){
        cout << "[MIA]@Proyecto1:~$  ";
        getline(cin,entradacmd);
        analisis.analizar(entradacmd);
    }
    return 0;

	return 0;
}
