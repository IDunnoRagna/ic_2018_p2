#ifndef projeto2_ficheiro_H
#define projeto2_ficheiro_H

// Systema
#include <stddef.h> // size_t


// Structs
//----------------------------------------------//
	// Contem varias propriadades de um ficheiro depois de lido
	typedef struct Ficheiro
	{
		// O conteúdo do ficheiro
		char* conteudo;
		
		// O tamanho to ficheiro
		size_t tamanho;
		
		// Todas as linhas do conteudo
		char** linhas;
		
		// O numero de linhas
		size_t num_linhas;
	} Ficheiro;
//----------------------------------------------//


// Funções
//----------------------------------------------//
	// Abre um ficheiro e retorna uma struct ficheiro com varias properiadades do ficheiro
	Ficheiro abrir_ficheiro(const char* nome);

	// Feixa um ficheiro
	void fechar_ficheiro(Ficheiro* ficheiro);
//----------------------------------------------//

#endif // projeto2_ficheiro_H
