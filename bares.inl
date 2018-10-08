#include <iostream>
#include <string>
#include <math.h>
#include "bares.h"

expressao::expressao (std::string a) {	infixa = a; }				//Construtor

expressao::~expressao () {}

int expressao::infixa_para_posfixa () {
	int i,j,k;      // i = Contador da fila infixa; j = contador da fila posfixa; k = contador da pilha;
	i = 0;
	j = 0;
	k = 0;
	int flag = 0;								//Flag para saber se vai entrar na Pilha
	char Symb;									//Simbolo(Token) Atual
	char SymbAnterior = 0;						//Simbolo(Token) Anterior
	
	Fila posfixa; 								//Fila de Simbolos

	Pilha <char> operadores;					//Pilha de operadores

 	int flagEspaco = 0;							//Avisa que o ultimo foi um espaco ou tab;

	while (infixa[i] != 0) {					//Enquanto nao chegar ao final da entrada
		flag = 0;		
		flagEspaco = 0;							

		Symb = infixa[i];						//Symb recebe, em ordem, simbolos da entrada

		while (Symb == 32 || Symb == 9) {		//Ignora Tabs e espacos avancando na fila infixa
			i++;
			Symb = infixa[i];
			flagEspaco = 1;						//mostra que ultimo foi espaço ou tab
		}

		//Aviso de erro 5
		if (flagEspaco == 1 && operador(Symb) == 0 && operador(SymbAnterior) == 0) {					
			std::cout << "Erro 5 -> Falta operador \n";
			return 0;
		}

		//Aviso de Erro 2
		if (operador(Symb) == 1 && SymbAnterior != 0 && operador(SymbAnterior) == 1){
			if(SymbAnterior != '(' && SymbAnterior != ')' && Symb != '(' && Symb != '-') {				
				std::cout << "Erro 2 -> Falta operando \n";
				return 0;
			}
		}

		//Aviso de Erro 3
		if(operador(Symb) == -1 && (SymbAnterior == 0 || operador(SymbAnterior) == 1)) {				
			std::cout << "Erro 3 -> Operando invalido \n";
			return 0;
		}

		//Aviso de Erro 4
		if(operador(Symb) == -1 && (SymbAnterior == 0 || operador(SymbAnterior) == 0)) {				 
				std::cout << "Erro 4 -> Operador invalido \n";
			return 0;
		}

		//Se for primeiro operando 
		if (operador(Symb) == 0 && (operador(SymbAnterior) == 1 || SymbAnterior == 0)) {				
				posfixa.coloca(Symb - '0', 0);			//Coloca o INTEIRO direto na saida 
				j++;								//incrementa a saida		
			flag = 1;								//Pula a etapa de Empilhamento
		}

		//Se for segundo operando em seguida
		if (operador(Symb) == 0 && (operador(SymbAnterior) == 0 && SymbAnterior != 0)) {  
			if (Symb != 32 && Symb != 9){
				posfixa.ultimo->valor.num  = posfixa.ultimo->valor.num*10 + (Symb - '0');  		//Multiplica por 10 e coloca segundo digito
				if (posfixa.ultimo->valor.num > 32767 || posfixa.ultimo->valor.num < -32767){			//Aviso de Erro 1
					std::cout << "Erro 1 -> Constante numerica invalida \n";		
					return 0;
				}
			}
			flag = 1;
		}

		if (Symb == ')') {						//Se for ')' o processo a ser feito eh desimpilhar ate chegar no '('
			char aux;
			aux = operadores.desempilha();
			flag = 1;							//Nao entra na pilha
			while (aux != '(' && k > 0) {				
					posfixa.coloca(0,aux);
					aux = operadores.desempilha();
					k--;
					j++;
			}	

			if (aux == '(') {
				k--;
			}
			
			else { //Aviso de Erro 6
				std::cout << "Erro 6 -> Fechamento de escopo invalido \n";	
				return 0;	
			}
		}	

		if (Symb == '-' ) {											//Verifica se eh '-' unario 
			if (SymbAnterior == 0 || operador(SymbAnterior) == 1)	//Se for o primeiro simbolo, ou o simbolo anterior for operador
				Symb = '#';											//Significa que eh '-' unitario, usa o '#' para representa-lo 
		}															//(para nao confundir com subtracao)

		if (flag == 0) {											// Se deve entrar na lista
			while (k != 0 && procedencia(operadores.topo->valor,Symb) <= 0 && operadores.topo->valor != '(') {	//Se topo deve sair da pilha:
				char aux = operadores.topo->valor;
				if (procedencia(aux,Symb) <= 0 && aux != '(') {		//Somente se topo for menor que Symb (ter ordem primaria)
					if (aux != '#' || Symb != '#') {
						aux = operadores.desempilha();
						posfixa.coloca(0,aux);											//Posfixa recebe Topo da Pilha
						j++;											
						k--;
					}													
				}
			}
		
  			
			operadores.empilha(Symb);													//Coloca novo simbolo no topo da pilha
			k++;																		//Contador pilha
		}		

		SymbAnterior = Symb;															//Avanca na fila infixa
		i++;
	}
																						//Quando termina a fila infixa
	while (k > 0) {																		//Termina de colocar os operadores
		char aux;
		aux = operadores.desempilha();
		if (aux != '(') {
			posfixa.coloca(0,aux);														//Desempilhando ate o final
		}
		else { //Aviso de erro 7
			std::cout << "Erro 7 -> Escopo aberto \n";									//Se tiver algum '(' na fila
			return 0;
		}

		k--;
		j++;																			
	}

	// posfixa.imprimir_fila();

	return posfixa.resolver_posfixa();
}

void Fila::imprimir_fila () {
	Item* busca = inicio;
	while (busca != NULL) {
		if (busca->valor.num != 0)
			std::cout << busca->valor.num;
		else if (busca->valor.op) 
			std::cout << busca->valor.op;

		busca = busca->proximo;
	}
}

int expressao::procedencia(char a, char b) {						//Verifica quem é precedente entre 2 operadores
		int x=5,y=5;
		if (a == '(' || a == ')')
			x = 1;
		else if (a == '#')
			x = 2;
		else if (a == '^')
			x = 3;
		else if (a == '*' || a == '/' || a == '%')
			x = 4;
		else if (a == '+' || a == '-')
			x = 5;
		else 
			x = 0;

		if (b == '(' || b == ')')
			y = 1;
		else 
			if (b == '#')
				y = 2;
		else 
			if (b == '^')
				y = 3;
		else 
			if (b == '*' || b== '/' || b == '%')
				y = 4;
		else 
			if (b == '+' || b == '-')
				y = 5;
		else  
			x = 0; 

		return x-y;					//
}

int expressao::operador (char a) {														//Verifica se eh operador ou operando ou invalido
	if (a == '(' || a == ')' || a == '#' || a == '^' || a == '*' || a == '/' || a == '%' || a == '+' || a == '-')
		return 1;
	else if (a >= '0' && a <= '9') 
		return 0;
	else 
		return -1;									
}

int Fila::resolver_posfixa () {
		simbolo x;
		Pilha <int> resolve;
		int resultado = 0;
		char operador = 0;
		int operandoA = 0;
		int operandoB = 0;

		x = tira();
		while (x.num != 0 || x.op != 0) {      //Ate chegar no final
			resultado = 0;
			if (x.num != 0) {
				operandoA = x.num;
				resolve.empilha(operandoA);
			}

			operador = x.op;
			if (operador != 0 && operador != '#') {
				operandoB = resolve.desempilha();
				operandoA = resolve.desempilha();
				

				if (operador == '+') {
					if ((operandoA + operandoB) < -32767 || (operandoA + operandoB) > 32767) {
						std::cout << "Erro 9 -> Resultado ultrapassa limites de representacao \n";
						return 0;
					}
					resultado = operandoA + operandoB;
					resolve.empilha(resultado);
				}

				if (operador == '-') {
					if ((operandoA - operandoB) < -32767 || (operandoA - operandoB) > 32767) {
						std::cout << "Erro 9 -> Resultado ultrapassa limites de representacao \n";
						return 0;
					}
					resultado = operandoA - operandoB;
					resolve.empilha(resultado);
				}

				if (operador == '*') {
					if ((operandoA * operandoB) < -32767 || (operandoA * operandoB) > 32767) {
						std::cout << "Erro 9 -> Resultado ultrapassa limites de representacao \n";
						return 0;
					}
					resultado = (operandoA*operandoB);
					resolve.empilha(resultado);
				}

				if (operador == '/') {
					if(operandoB == 0){
						std::cout << "Erro 8 -> Divisao por zero \n";
						return 0; 
					}

					resultado = operandoA / operandoB;
					resolve.empilha(resultado);
				}

				if (operador == '%') {
					resultado = operandoA % operandoB;
					resolve.empilha(resultado);
				}

				if (operador == '^') {
					if (pow(operandoA,operandoB) < -32767 || pow(operandoA,operandoB) > 32767) {
						std::cout << "Erro 9 -> Resultado ultrapassa limites de representacao \n";
						return 0;
					}
					resultado = pow(operandoA,operandoB);
					resolve.empilha(resultado);
				}
			}

				else if (operador == '#') {
					operandoA = resolve.desempilha();
					resultado = (operandoA * -1);
					resolve.empilha(resultado);
					operador = x.op;
				}
			
		x = tira();	
		}				//
	resultado = resolve.desempilha();
	return resultado;
}

template < typename data >
Pilha < data > ::Pilha () {
	topo = NULL;
	pre_topo = NULL;
}

template <typename data>
Pilha < data > ::~Pilha () {}

template <typename data>
void Pilha < data > ::empilha (data a) {
		Item* novo = new Item;
		novo->valor = a;
		novo->proximo = topo;
		topo = novo;
}

template <typename data>
data Pilha < data > ::desempilha () {
		if (topo == NULL) {
			return 0;
		}

		data a;
		a = topo->valor;
		pre_topo = topo->proximo;
		delete (topo);
		topo = pre_topo;	
		return a;
}


Fila::Fila () {
	inicio = NULL;
	aux = NULL;
}

Fila::~Fila () {}

void Fila::coloca (int a, char b) {
	if (inicio == NULL) { //Se a Fila estiver vazia, cria o primeiro elemento
		Item* novo = new Item;
		novo->valor.num = a;
		novo->valor.op = b;
		novo->proximo = NULL;
		inicio = novo;
		ultimo = novo;
	}

	else {
		Item* novo = new Item;
		novo->valor.num = a;
		novo->valor.op = b;
		novo->proximo = NULL;
		
		aux = inicio;
		while (aux->proximo != NULL)
			aux = aux->proximo;

		aux->proximo = novo;
		ultimo = novo;
	}
}

simbolo Fila::tira () {
		simbolo x;
		if (inicio == NULL) {
			simbolo y;
			y.num = 0;
			y.op = 0;
			return y;
		}


		x = inicio->valor;
		aux = inicio->proximo;
		delete (inicio);
		inicio = aux;
		return x;
}
