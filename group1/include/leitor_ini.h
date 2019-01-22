#ifndef projeto2_leitor_ini_H
#define projeto2_leitor_ini_H

// Projeto
#include "ficheiro.h" // Ficheiro
#include "jogo.h"     // Jogo

// Funções
//----------------------------------------------//
	// Obtem um jogo de um ficheiro contendo um ini
	// libertar_jogo deve ser chamado no fim de vida do jogo
	Jogo obter_jogo(Ficheiro f);
	
	// Liberta tudo alocado no jogo
	void libertar_jogo(Jogo* jogo);
//----------------------------------------------//

#endif
