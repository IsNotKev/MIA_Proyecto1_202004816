#include <iostream>
#include <sstream>

using namespace std;
using std::stoi;

int main(){

	string instruccion = "";
	string lectura;
	string ex;

	while(instruccion.compare("exit") != 0){
		
		cin >> instruccion;
		stringstream input(instruccion); 

		while (getline(input, lectura, ' ')){
        	cout << "Un valor: " << lectura << endl;
    	}
		cout << instruccion << "\n" << endl;
	}

	return 0;
}
