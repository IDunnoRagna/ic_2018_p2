// Sistema
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>  // printf, fprintf, stderr, stdin, fgets, scanf
#include <ctype.h>  // isspace

// Projeto
#include <ficheiro.h>   // abrir_ficheiro, fechar_ficheiro
#include <jogo.h>       // Jogo, processar_turno_jogador
#include <leitor_ini.h> // obter_jogo, libertar_jogos
#include <io.h>         // perguntar_y_n, perguntar_num

int main(int argc, char** argv)
{
	// Verificar que temos exatamente 1 argumento
	// 2 porque o nome to executável é sempre o primeiro
	if ( argc != 2 )
	{
		// Retornar falha
		fprintf(stderr, "Usagem: ./main {*.ini}\n");
		return EXIT_FAILURE;
	}
	
	Ficheiro f = abrir_ficheiro( argv[1] );
	
	// Obter o jogo
	Jogo jogo = obter_jogo(f);
	
	// Se houve um erro, terminar o programa
	if ( jogo.celas == NULL )
	{
		fprintf(stderr, "Occoreu um erro\n");
		return EXIT_FAILURE;
	}
	
	// Escolher a posição dos jogadores
	print_jogo(&jogo);
	
	while ( true )
	{
		// Ver a aldeia inicial
		int aldeia = perguntar_num("Jogador 1, escolha a sua aldeia inicial:");
		
		// Ver errors
		if ( aldeia < 0 )                              { printf("Numero não pode ser negativo.\n"); continue; }
		if ( aldeia >= jogo.xdim * jogo.ydim )         { printf("Numero não pode ser maior que o numero de celas.\n"); continue; }
		
		// Por a cela em nome do jogador
		jogo.celas[ aldeia ].proprietario = 1;
		jogo.celas[ aldeia ].tipo         = cela_aldeia;
		
		// Dar o ponto
		jogo.jogador1.pontos++;
		
		// Break
		break;
	}
	
	while ( true )
	{
		// Ver a aldeia inicial
		int aldeia = perguntar_num("Jogador 2, escolha a sua aldeia inicial:");
		
		// Ver erros
		if ( aldeia < 0 )                              { printf("Numero não pode ser negativo.\n"); continue; }
		if ( aldeia >= jogo.xdim * jogo.ydim )         { printf("Numero não pode ser maior que o numero de celas.\n"); continue; }
		if ( jogo.celas[ aldeia ].proprietario == 1 )  { printf("Não pode escolher a mesma cela que jogador 1.\n"); continue; }
		
		// Por a cela em nome do jogador
		jogo.celas[ aldeia ].proprietario = 2;
		jogo.celas[ aldeia ].tipo         = cela_aldeia;
		
		// Dar o ponto
		jogo.jogador2.pontos++;
		
		// Break quando tivermos o numero
		break;
	}
	
	// Quem ganhou
	int jogador_num_ganhou = -1;
	
	// Loop primaria
	while ( true )
	{
		// Print o jogo no inicio de cada jogada
		print_jogo(&jogo);
		
		// Dizer quantos pontos tem e processar o seu turno
		printf("Jogador 1, tem %d pontos\n", jogo.jogador1.pontos);
		processar_turno_jogador( &jogo.jogador1, 1, &jogo);
		
		// Verificar se o jogador 1 ganhou
		if ( jogo.jogador1.pontos >= 6 ) { jogador_num_ganhou = 1; break; }
		
		// Print o jogo depois do jogador 1 jogar
		print_jogo(&jogo);
		
		// Dizer quantos pontos tem e processar o seu turno
		printf("Jogador 2, tem %d pontos\n", jogo.jogador2.pontos);
		processar_turno_jogador( &jogo.jogador2, 2, &jogo);
		
		// Verificar se o jogador 2 ganhou
		if ( jogo.jogador2.pontos >= 6 ) { jogador_num_ganhou = 2; break; }
	}
	
	if ( jogador_num_ganhou != -1 )
	{
		printf("Parabêns, jogador %1d, ganhou!\n", jogador_num_ganhou);
	}
	
	// Libertar tudo
	libertar_jogo( &jogo );
	fechar_ficheiro( &f );
	
	// retornar successo
	return EXIT_SUCCESS;
}
