// Header
#include <ficheiro.h>

// Systema
#include <stdlib.h> // malloc
#include <stdio.h>  // FILE, fopen, fclose, fseek, ftell

Ficheiro abrir_ficheiro(const char* nome)
{
	// O ficheiro para retornar
	Ficheiro ficheiro;
	
	// Processar o ficheiro
	{
		// O ficheiro no disco
		FILE* file = fopen(nome, "r");
		
		// Se não conseguimos abrir, retornar um ficheiro vazio
		if ( !file )
		{
			ficheiro.conteudo   = NULL;
			ficheiro.tamanho    = 0;
			ficheiro.linhas     = NULL;
			ficheiro.num_linhas = 0;
			return ficheiro;
		}
		
		// Ler o ficheiro todo
		{
			// Obter o tamanho
			fseek(file, 0L, SEEK_END);
			ficheiro.tamanho = ftell(file);
			fseek(file, 0L, SEEK_SET);
			
			// Alocar os conteudos
			// Notar que não é preciso usar calloc, porque iremos escrever o ficheiro todo
			// +1 para o '\0'
			ficheiro.conteudo = (char*)malloc( (ficheiro.tamanho + 1) * sizeof(char));
			ficheiro.conteudo[ ficheiro.tamanho ] = '\0';
			
			// Ler o conteudo
			fread( (void*)ficheiro.conteudo, sizeof(char), ficheiro.tamanho, file);
		}
		
		
		// Processar as linhas
		{
			// Contar as linhas
			ficheiro.num_linhas = 1;
			for (size_t n=0; n<ficheiro.tamanho; n++)
			{
				if ( ficheiro.conteudo[n] == '\n' ) { ficheiro.num_linhas++; }
			}
			
			// Allocar as linhas
			ficheiro.linhas = (char**)malloc( (ficheiro.num_linhas + 1) * sizeof(char*) );
			ficheiro.linhas[ ficheiro.num_linhas ] = NULL;
			
			// A primeira linha é sempre conteudo+0
			ficheiro.linhas[0] = ficheiro.conteudo;
			
			// Se houver mais que 1 linha, adicionar linhas adicionais
			if ( ficheiro.num_linhas > 1 )
			{
				// Processar as linhas
				// Começamos na segunda linha porque a primeira já fizemos
				char** linha_cur = ficheiro.linhas+1;
				for (size_t n=0; n<ficheiro.tamanho; n++)
				{
					if ( ficheiro.conteudo[n] == '\n' ) { *linha_cur = ficheiro.conteudo+n+1; linha_cur++; }
				}
			}
		}
		
		
		// Fechar o ficheiro
		fclose(file);
	}
	
	// Retornar o ficheiro
	return ficheiro;
}

void fechar_ficheiro(Ficheiro* ficheiro)
{
	// Libertar conteudo e linhas se existirem
	if ( ficheiro->conteudo != NULL ) { free( (void*)ficheiro->conteudo ); }
	if ( ficheiro->linhas   != NULL ) { free( (void*)ficheiro->linhas   ); }
	
	// E por tudo a NULL e 0
	ficheiro->conteudo   = NULL;
	ficheiro->tamanho    = 0;
	ficheiro->linhas     = NULL;
	ficheiro->num_linhas = 0;
}
