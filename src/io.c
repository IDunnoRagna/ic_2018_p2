// Header
#include <io.h>

// Sistema
#include <stdio.h>  // printf, fgets
#include <stdlib.h> // strtol

bool perguntar_y_n(const char* str)
{
	while ( true )
	{
		printf("%s (y/n) ", str);
		
		// A resposta
		char resposta[32] = {0};
		while ( !fgets(resposta, 32, stdin) ) {}
		
		// Ver erros
			 if ( resposta[0] == 'y' || resposta[0] == 'Y' ) { return true; }
		else if ( resposta[0] == 'n' || resposta[0] == 'N' ) { return false; }
		else    { printf("Inválido, tente outra vez.\n"); continue; }
	}
}

int perguntar_num(const char* str)
{
	while ( true )
	{
		printf("%s #", str);
		
		// A resposta
		char resposta[32] = {0};
		while ( !fgets(resposta, 32, stdin) ) {}
		
		// Tentar extrair um numero
		char* num_erro = NULL;
		int num = strtol(resposta, &num_erro, 10);
		
		// Ver erros
		if ( num_erro == NULL || *num_erro != '\n' || resposta[0] == '\n') { printf("Inválido, tente outra vez.\n"); continue; }
		
		// Retornar o numero
		return num;
	}
}
