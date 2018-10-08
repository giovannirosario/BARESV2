#include <iostream>
#include <string>
#include "bares.h"

int main (void) {
	std::string x;
	int result;
	int i = 1;

	while (std::getline (std::cin,x) != 0 ) {					//Enquanto for diferente de EOF 
		std::cout << "Resultado da entrada:" << x << " ";
		expressao entrada(x);									//Cria classe expressao
		result = entrada.infixa_para_posfixa();
		std::cout << " eh:  " << result << "\n";
		i++;
	}

	return 0;
}

