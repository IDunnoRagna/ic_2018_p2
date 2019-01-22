#ifndef projeto2_jogo_H
#define projeto2_jogo_H


// Systema
#include <stdbool.h> // bool, true, false


// Enums
//----------------------------------------------//
	// Enum que representa cada tipo de recurso natural
	typedef enum RecursoNaturalTipo
	{
		recurso_natural_wool   = 0,
		recurso_natural_brick  = 1,
		recurso_natural_lumber = 2,
		recurso_natural_grain  = 3,
		recurso_natural_iron   = 4,
		recurso_natural_desert = 5
	} RecursoNaturalTipo;
	
	// Tipo de cela
	typedef enum CelaTipo
	{
		cela_nada,
		cela_aldeia,
		cela_cidade
	} CelaTipo;
//----------------------------------------------//

// Structs
//----------------------------------------------//
	// Struct que tem um recurso natural e um valor para ele
	// Este valor será 0 quando tipo == desert
	typedef struct RecursoNatural
	{
		RecursoNaturalTipo tipo;
		int valor;
	} RecursoNatural;
	
	// Struct que representa cada cela no mapa
	typedef struct Cela
	{
		// Os recursos naturais
		RecursoNatural N;
		RecursoNatural S;
		RecursoNatural E;
		RecursoNatural W;
		
		// A quem pertence, 1 = J1, 2 = J2, 0 = Ninguem
		int proprietario;
		
		// Tipo de cela
		CelaTipo tipo;
	} Cela;
	
	
	typedef struct Jogador
	{
		// Cartas do jogador
		int cartas[5];
		
		// Quantos pontos o jogador tem
		int pontos;
	} Jogador;
	
	// Struct que representa o estado do jogo
	typedef struct Jogo
	{
		// Todas as celas
		Cela* celas;
		
		// Dimensões das celas
		int xdim;
		int ydim;
		
		// Os dois jogadores
		Jogador jogador1;
		Jogador jogador2;
	} Jogo;
//----------------------------------------------//

// Funções
//----------------------------------------------//
	// IO
	//----------------------------------------------//
		// Retorna um char* que representa um tipo de recurso natural
		char* obter_recurso_str(RecursoNaturalTipo recurso_tipo);
		
		// Retorna um char* que representa um tipo de cela
		char* obter_tipo_str(CelaTipo cela_tipo);
		
		// Retorna um recurso natural dado uma linha que o representa
		// Por examplo "L11" -> { lumber, 11 }
		RecursoNatural obter_recurso(char* str);
		
		// Imprime o estado do jogo
		void print_jogo(Jogo* jogo);
		
		// Imprime as cartas do jogador
		void print_cartas(Jogador* jogador);
	//----------------------------------------------//
	
	// Ver condições
	//----------------------------------------------//
		// Ve se um jogador consegue comprar uma aldeia
		bool jogador_consegue_comprar_aldeia(Jogador* jogador);
		
		// Ve se um jogador consegue comprar uma cidade
		bool jogador_consegue_comprar_cidade(Jogador* jogador);
		
		// Ve se existem celas de um certo jogador adjacentes a uma cela
		bool existem_celas_adjacentes_de_jogador(Jogo* jogo, int x, int y, int jogador_num);
		
		// Ve se um jogador tem celas livres para comprar
		bool existem_celas_para_comprar_adjacentes(Jogo* jogo, int jogador_num);
		
		// Ve se um jogador tem aldeias
		bool jogador_tem_aldeias(Jogo* jogo, int jogador_num);
		
		// Ve se um jogador tem algum recurso pelo menos a num
		bool jogador_tem_recurso_pelo_menos(Jogador* jogador, int num);
	//----------------------------------------------//
	
	// Mudar estado do jogo
	//----------------------------------------------//
		// Processa um turno do jogador
		void processar_turno_jogador(Jogador* jogador, int jogador_num, Jogo* jogo);
	//----------------------------------------------//
	
//----------------------------------------------//




#endif // projeto2_jogo_H
