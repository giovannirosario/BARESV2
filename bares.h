#ifndef _BARES_H_
#define _BARES_H_

struct simbolo{				//struct simbolo
			int num = 0;
			char op = 0;
};

class expressao{
	private:
		std::string infixa;

		int procedencia(char a, char b); 					//Verifica quem é precedente entre 2 operadores
		int operador (char a);								//Verifica se character é operador,operando ou nulo

	public: 
		expressao(std::string a);
		~expressao();

		int infixa_para_posfixa();
};

template < typename data > 
class Pilha {
	private: 
		struct Item {
			data valor;
			struct Item* proximo; 
		};
								
		Item* pre_topo;

	public:
		Item* topo;
		data desempilha ();
		void empilha (data a);

		

		Pilha();
		~Pilha();
};


class Fila {
	private: 
		struct Item{				//struct simbolo, para posfixa
			simbolo valor;
			Item* proximo;
		};
								
		Item* aux;

	public:
		Item* inicio;
		Item* ultimo;
		simbolo tira ();
		void coloca (int a, char b);
		void imprimir_fila();
		int resolver_posfixa (); 			//Resolve expressao posfixa e retorna resultado inteiro

		Fila();
		~Fila();
};


#include "bares.inl"

#endif