// Header
#include <leitor_ini.h>

// Sistema
#include <ctype.h>   // isspace
#include <string.h>  // strchr, strcmp
#include <stdlib.h>  // free
#include <stdio.h>   // printf
#include <stdbool.h> // true, false

Jogo obter_jogo(Ficheiro f)
{
	// O jogo
	Jogo jogo;
	
	// Por xdim e ydim para -1 e celas para NULL até os encontarmos
	jogo.xdim = -1;
	jogo.ydim = -1;
	jogo.celas = NULL;
	
	// Iniciar os jogadores
	jogo.jogador1.cartas[recurso_natural_wool  ] = 0;
	jogo.jogador1.cartas[recurso_natural_brick ] = 0;
	jogo.jogador1.cartas[recurso_natural_lumber] = 0;
	jogo.jogador1.cartas[recurso_natural_grain ] = 0;
	jogo.jogador1.cartas[recurso_natural_iron  ] = 0;
	jogo.jogador1.pontos                         = 0;
	
	jogo.jogador2.cartas[recurso_natural_wool  ] = 0;
	jogo.jogador2.cartas[recurso_natural_brick ] = 0;
	jogo.jogador2.cartas[recurso_natural_lumber] = 0;
	jogo.jogador2.cartas[recurso_natural_grain ] = 0;
	jogo.jogador2.cartas[recurso_natural_iron  ] = 0;
	jogo.jogador2.pontos                         = 0;
	
	// A cela que estamos a trabalhar agora
	Cela* cur_cela = jogo.celas;
	
	// Ler todo o .ini
	for (size_t linha_num=0; linha_num<f.num_linhas; linha_num++)
	{
		// A linha
		char* linha = f.linhas[linha_num];
		
		// Ignorar todos os espaços até '\n'
		while ( linha[0] != '\0' && linha[0] != '\n' && isspace(linha[0]) ) { linha++; }
		
		// Se a linha está vazia, é um comentário ou é o ultimo character, continuar
		if ( linha[0] == '\n' || linha[0] == ';' || linha[0] == '\0' ) { continue; }
		
		// O nome da propriedade e o valor
		char* propriedade_nome;
		char* propriedade_valor;
		
		// Ler tudo até o '=' e por em 'propriedade_nome'
		// E depois ler tudo de '=' até '\n' e por em 'propriedade_valor'
		{
			// Posição de o '='
			char* igual_pos = strchr(linha, '=');
			
			// Se não existir, reportar erro
			if ( igual_pos == NULL )
			{
				fprintf(stderr, "Erro, nenhum '=' foi encontrado na linha %lu\n", linha_num+1);
				jogo.celas = NULL;
				jogo.xdim = 0;
				jogo.ydim = 0;
				return jogo;
			}
			
			// Allocar o nome e escrever tudo
			size_t propriedade_nome_tam = (size_t)( igual_pos - linha );
			propriedade_nome = (char*)malloc( ( propriedade_nome_tam + 1 ) * sizeof(char) );
			propriedade_nome[ propriedade_nome_tam ] = '\0';
			for (size_t n=0; n<propriedade_nome_tam; n++) { propriedade_nome[n] = linha[n]; }
			
			// Posição do fim da linha, se strchr for NULL, então isto é o fim do ficheiro
			char* final_pos = strchr(igual_pos, '\n');
			if ( final_pos == NULL ) { final_pos = f.conteudo + f.tamanho; }
			
			// Allocar o valor e escrever tudo
			size_t propriedade_valor_tam = (size_t)( final_pos - (igual_pos+1) );
			propriedade_valor = (char*)malloc( ( propriedade_valor_tam + 1 ) * sizeof(char) );
			propriedade_valor[ propriedade_valor_tam ] = '\0';
			for (size_t n=0; n<propriedade_valor_tam; n++) { propriedade_valor[n] = (igual_pos+1)[n]; }
			
			
			// Se a propriedade for xdim ou ydim, escrever
			if ( strcmp( propriedade_nome, "xdim" ) == 0 ) { jogo.xdim = strtol( propriedade_valor , NULL, 10 ); }
			if ( strcmp( propriedade_nome, "ydim" ) == 0 ) { jogo.ydim = strtol( propriedade_valor , NULL, 10 ); }
			
			// Se encontramos xdim e ydim e a grid de celas é NULL, criar-la
			if ( (jogo.xdim) != -1 && (jogo.ydim) != -1 && (jogo.celas) == NULL )
			{
				// Se xdim or ydim for negativo, avisar
				if ( jogo.xdim < 0 || jogo.ydim < 0 )
				{
					fprintf(stderr, "As dimensões não podem ser negativas.\n");
					jogo.celas = NULL;
					jogo.xdim = 0;
					jogo.ydim = 0;
					return jogo;
				}
				
				jogo.celas = (Cela*)malloc( ( (jogo.xdim) * (jogo.ydim) ) * sizeof(Cela) );
				
				cur_cela = jogo.celas;
			}
			
			// Se a grid já existe
			if ( (jogo.celas) != NULL )
			{
				// O que já foi escrito agora
				static bool escrito_N = false;
				static bool escrito_S = false;
				static bool escrito_E = false;
				static bool escrito_W = false;
				
				// O que tem de ser escrito agora
				bool escrever_N =                      (strcmp( propriedade_nome, "N" ) == 0);
				bool escrever_S = escrever_N ? false : (strcmp( propriedade_nome, "S" ) == 0);
				bool escrever_E = escrever_S ? false : (strcmp( propriedade_nome, "E" ) == 0);
				bool escrever_W = escrever_E ? false : (strcmp( propriedade_nome, "W" ) == 0);
				
				// Se vamos escrever alguma coisa e cur_cela está fora das celas, avisar
				if ( (escrever_N || escrever_S || escrever_E || escrever_W) && cur_cela - (jogo.celas) >= (jogo.xdim) * (jogo.ydim) )
				{
					fprintf(stderr, "Demasidas celas foram dadas.\n");
					libertar_jogo( &jogo );
					return jogo;
				}
				
				// Se encontrarmos um "N", "S", "E" ou "W", conseguimos uma cela e escrevemo-la
				     if ( escrever_N ) { cur_cela->N = obter_recurso( propriedade_valor ); escrito_N = true; }
				else if ( escrever_S ) { cur_cela->S = obter_recurso( propriedade_valor ); escrito_S = true; }
				else if ( escrever_E ) { cur_cela->E = obter_recurso( propriedade_valor ); escrito_E = true; }
				else if ( escrever_W ) { cur_cela->W = obter_recurso( propriedade_valor ); escrito_W = true; }
				
				// Se já escrevemos tudo, então vamos para a proxima cela
				if ( escrito_N == true && escrito_S == true && escrito_E == true && escrito_W == true )
				{
					// Acabar de escrever a cela
					cur_cela->proprietario = 0;
					cur_cela->tipo = cela_nada;
					
					// Ir para a proximo
					cur_cela++;
					escrito_N = false;
					escrito_S = false;
					escrito_E = false;
					escrito_W = false;
				}
			}
		}
		
		// Libertar tudo
		free( (void*)propriedade_nome  );
		free( (void*)propriedade_valor );
	}
	
	// Se cur_cela não estiver no fim, um erro occoreu
	if ( cur_cela != (jogo.celas) + (jogo.xdim) * (jogo.ydim) )
	{
		fprintf(stderr, "Não foram dadas celas suficientes para o tamanho.\n");
		fprintf(stderr, "Foram dadas %lu, mas %d eram esperadas.\n", cur_cela - (jogo.celas), (jogo.xdim) * (jogo.ydim));
		libertar_jogo( &jogo );
		return jogo;
	}
	
	// Retornar o jogo
	return jogo;
}


void libertar_jogo(Jogo* jogo)
{
	// Libertar as celas
	free( jogo->celas );
	jogo->celas = NULL;
	jogo->xdim = 0;
	jogo->ydim = 0;
}
