// Header
#include <jogo.h>

// Sistema
#include <stdio.h>  // printf
#include <stdlib.h> // strtol

// Projeto
#include <io.h> // perguntar_y_n, perguntar_num

// IO
//----------------------------------------------//
	char* obter_recurso_str(RecursoNaturalTipo recurso_tipo)
	{
		switch ( recurso_tipo )
		{
			case recurso_natural_wool:   return "W";
			case recurso_natural_brick:  return "B";
			case recurso_natural_lumber: return "L";
			case recurso_natural_grain:  return "G";
			case recurso_natural_iron:   return "I";
			case recurso_natural_desert: return "D";
			
			// Se não existir, erro
			default:
				fprintf(stderr, "Recurso natural tipo invalido: %d", recurso_tipo);
				return NULL;
		}
	}

	char* obter_tipo_str(CelaTipo cela_tipo)
	{
		switch ( cela_tipo )
		{
			case cela_nada:   return "      ";
			case cela_aldeia: return "Aldeia";
			case cela_cidade: return "Cidade";
			
			// Se não existir, erro
			default:
				fprintf(stderr, "Cela tipo invalido: %d", cela_tipo);
				return NULL;
		}
	}

	RecursoNatural obter_recurso(char* str)
	{
		// O recurso natural
		RecursoNatural recurso;
		
		// Ver qual o tipo de recurso natural
		switch ( str[0] )
		{
			case 'W': recurso.tipo = recurso_natural_wool;   break;
			case 'B': recurso.tipo = recurso_natural_brick;  break;
			case 'L': recurso.tipo = recurso_natural_lumber; break;
			case 'G': recurso.tipo = recurso_natural_grain;  break;
			case 'I': recurso.tipo = recurso_natural_iron;   break;
			case 'D': recurso.tipo = recurso_natural_desert; break;
			
			// Se não existir, retornamos um recurso com deserto
			default:
				fprintf(stderr, "Recurso natural invalido: %s", str);
				recurso.tipo = recurso_natural_desert;
		}
		
		// Se é um deserto, por o valor a 0 e retornar
		if ( recurso.tipo == recurso_natural_desert ) { recurso.valor = 0; return recurso; }
		
		// De resto ler o numero e por-lo em valor
		// +1 porque estamos a retirar a letra
		recurso.valor = strtol(str+1, NULL, 10);
		
		// Retornar o recurso
		return recurso;
	}

	void print_jogo(Jogo* jogo)
	{
		// Todas as linhas
		for (int y=0; y<jogo->ydim; y++)
		{
			for (int x=0; x<jogo->xdim; x++) { printf("+---------------");                                                                                               } printf("+\n");
			for (int x=0; x<jogo->xdim; x++) { printf("|      #%2d      ",  y*jogo->xdim + x);                                                                           } printf("|\n");
			for (int x=0; x<jogo->xdim; x++) { printf("|               ");                                                                                               } printf("|\n");
			
			for (int x=0; x<jogo->xdim; x++) { printf("| N: %s (%2d)     ", obter_recurso_str( jogo->celas[y*jogo->xdim + x].N.tipo ), jogo->celas[y*jogo->xdim + x].N.valor); } printf("|\n");
			for (int x=0; x<jogo->xdim; x++) { printf("| S: %s (%2d)     ", obter_recurso_str( jogo->celas[y*jogo->xdim + x].S.tipo ), jogo->celas[y*jogo->xdim + x].S.valor); } printf("|\n");
			for (int x=0; x<jogo->xdim; x++) { printf("| E: %s (%2d)     ", obter_recurso_str( jogo->celas[y*jogo->xdim + x].E.tipo ), jogo->celas[y*jogo->xdim + x].E.valor); } printf("|\n");
			for (int x=0; x<jogo->xdim; x++) { printf("| W: %s (%2d)     ", obter_recurso_str( jogo->celas[y*jogo->xdim + x].W.tipo ), jogo->celas[y*jogo->xdim + x].W.valor); } printf("|\n");
			for (int x=0; x<jogo->xdim; x++) { printf("|               ");                                                                                               } printf("|\n");
			
			for (int x=0; x<jogo->xdim; x++)
			{
				if ( jogo->celas[y*jogo->xdim + x].proprietario == 0 ) { printf("| %s        ", obter_tipo_str( jogo->celas[y*jogo->xdim + x].tipo )  ); }
				else                                                   { printf("| %s     J%d ", obter_tipo_str( jogo->celas[y*jogo->xdim + x].tipo ), jogo->celas[y*jogo->xdim + x].proprietario); }
			} printf("|\n");
		}
		
		// Ultima linha
		for (int x=0; x<jogo->xdim; x++) { printf("+---------------"); } printf("+\n");
	}
	
	void print_cartas(Jogador* jogador)
	{
		printf("+------------+\n");
		printf("| Wool    %2d |\n", jogador->cartas[recurso_natural_wool  ]);
		printf("| Brick   %2d |\n", jogador->cartas[recurso_natural_brick ]);
		printf("| Lumber  %2d |\n", jogador->cartas[recurso_natural_lumber]);
		printf("| Grain   %2d |\n", jogador->cartas[recurso_natural_grain ]);
		printf("| Iron    %2d |\n", jogador->cartas[recurso_natural_iron  ]);
		printf("+------------+\n");
	}
//----------------------------------------------//

// Ver condições
//----------------------------------------------//
	bool jogador_consegue_comprar_aldeia(Jogador* jogador)
	{
		return  (jogador->cartas[ recurso_natural_brick  ] >= 1) &&
				(jogador->cartas[ recurso_natural_lumber ] >= 1) &&
				(jogador->cartas[ recurso_natural_grain  ] >= 1) &&
				(jogador->cartas[ recurso_natural_wool   ] >= 1);
	}
	
	bool jogador_consegue_comprar_cidade(Jogador* jogador)
	{
		return  (jogador->cartas[ recurso_natural_grain ] >= 2) &&
				(jogador->cartas[ recurso_natural_iron  ] >= 3);
	}
	
	bool existem_celas_adjacentes_de_jogador(Jogo* jogo, int x, int y, int jogador_num)
	{
		if ( x > 0            && jogo->celas[ jogo->xdim * (y+0) + (x-1) ].proprietario == jogador_num) { return true; }
		if ( x < jogo->xdim-1 && jogo->celas[ jogo->xdim * (y+0) + (x+1) ].proprietario == jogador_num) { return true; }
		if ( y > 0            && jogo->celas[ jogo->xdim * (y-1) + (x+0) ].proprietario == jogador_num) { return true; }
		if ( y < jogo->ydim-1 && jogo->celas[ jogo->xdim * (y+1) + (x+0) ].proprietario == jogador_num) { return true; }
		
		return false;
	}
	
	bool existem_celas_para_comprar_adjacentes(Jogo* jogo, int jogador_num)
	{
		for (int n=0; n<jogo->xdim*jogo->ydim; n++)
		{
			// Se a cela for do jogador, e existirem celas adjacentes do jogador 0 (vazio)
			if ( jogo->celas[n].proprietario == jogador_num &&
				 existem_celas_adjacentes_de_jogador(jogo, n % jogo->xdim, n / jogo->ydim, 0))
			{
				return true;
			}
		}
		
		// Se chegamos aqui, então não existem
		return false;
	}
	
	bool jogador_tem_aldeias(Jogo* jogo, int jogador_num)
	{
		for (int n=0; n<jogo->xdim*jogo->ydim; n++)
		{
			// Se uma cela for uma aldeia e o proprietário for o jogador
			if ( jogo->celas[n].tipo == cela_aldeia && jogo->celas[n].proprietario == jogador_num )
			{
				return true;
			}
		}
		
		// Se chegamos aqui, não tem
		return false;
	}
	
	bool jogador_tem_recurso_pelo_menos(Jogador* jogador, int num)
	{
		for (int n=recurso_natural_wool; n<recurso_natural_desert; n++)
		{
			if ( jogador->cartas[n] >= num ) { return true; }
		}
		
		// Se chegamos aqui, não tem
		return false;
	}
//----------------------------------------------//

// Mudar estado do jogo
//----------------------------------------------//
	void processar_carta_jogador(Jogador* jogador, int soma_dados, RecursoNatural recurso, CelaTipo cela_tipo)
	{
		// Não precisamos de ver se é deserto porque esses tem 0
		// Maximo de 99
		if ( recurso.valor == soma_dados && jogador->cartas[ recurso.tipo ] < 99 )
		{
				 if ( cela_tipo == cela_aldeia ) { jogador->cartas[ recurso.tipo ] += 1; }
			else if ( cela_tipo == cela_cidade ) { jogador->cartas[ recurso.tipo ] += 2; }
		}
	}

	void processar_cartas_jogador(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Lançar os dois dados
		int dado1 = (rand() % 6)+1;
		int dado2 = (rand() % 6)+1;
		
		printf("Jogador %d, a soma dos teus dados é %d\n", jogador_num, dado1+dado2);
		
		for (int n=0; n<jogo->xdim*jogo->ydim; n++)
		{
			if ( jogo->celas[n].proprietario == jogador_num )
			{
				processar_carta_jogador(jogador, dado1+dado2, jogo->celas[n].N, jogo->celas[n].tipo);
				processar_carta_jogador(jogador, dado1+dado2, jogo->celas[n].S, jogo->celas[n].tipo);
				processar_carta_jogador(jogador, dado1+dado2, jogo->celas[n].E, jogo->celas[n].tipo);
				processar_carta_jogador(jogador, dado1+dado2, jogo->celas[n].W, jogo->celas[n].tipo);
			}
		}
	}
	
	void processar_comprar_celas(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Se o jogador não consegue comprar uma aldeia
		if ( !jogador_consegue_comprar_aldeia(jogador) ) { printf("Jogador %1d, precissa de B1, L1, G1 e W1 para comprar uma aldeia.\n", jogador_num); return; }
		
		// Se não existem celas adjacentes para o jogador comprar
		if ( !existem_celas_para_comprar_adjacentes(jogo, jogador_num) ) { printf("Jogador %1d, não existem celas adjacentes para comprar.\n", jogador_num); return; }
		
		// A linha para perguntar ao jogador
		char quer_comprar_num_pergunta[68] = {0};
		snprintf(quer_comprar_num_pergunta, 68, "Jogador %1d, escolha a cela que quere comprar (-1 para não comprar):", jogador_num);
		
		// Loop para perguntar ao jogador
		while ( true )
		{
			// Perguntar a cela
			int cela_num = perguntar_num(quer_comprar_num_pergunta);
			
			// Ver erros
			if (  cela_num == -1 )                                                                                       { break; }
			if (  cela_num < 0 )                                                                                         { printf("Numero não pode ser negativo e não -1.\n"); continue; }
			if (  cela_num >= jogo->xdim * jogo->ydim )                                                                  { printf("Numero não pode ser maior que o numero de celas.\n"); continue; }
			if (  jogo->celas[ cela_num ].proprietario != 0 )                                                            { printf("Essa cela já está ocupada.\n"); continue; }
			if ( !existem_celas_adjacentes_de_jogador(jogo, cela_num % jogo->xdim, cela_num / jogo->ydim, jogador_num) ) { printf("Essa cela não é adjacente a nehuma das suas.\n"); continue; }
			
			// Tirar-lhe os materiais
			jogador->cartas[ recurso_natural_brick  ]--;
			jogador->cartas[ recurso_natural_lumber ]--;
			jogador->cartas[ recurso_natural_grain  ]--;
			jogador->cartas[ recurso_natural_wool   ]--;
			
			// Por a cela em nome do jogador
			jogo->celas[ cela_num ].proprietario = jogador_num;
			jogo->celas[ cela_num ].tipo         = cela_aldeia;
			
			// Dar o ponto
			jogador->pontos++;
			
			// Break quando tivermos o numero
			break;
		} // Ver qual comprar
	}
	
	void processar_transformar_celas(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Se o jogador não tem recursos suficients
		if ( !jogador_consegue_comprar_cidade(jogador) ) { printf("Jogador %1d, preceisa de G2 e I3 para transformar uma aldeia.\n", jogador_num); return; }
		
		// Se o jogador não tiver nenhuma aldeia para transformar
		if ( !jogador_tem_aldeias(jogo, jogador_num) ) { printf("Jogador %1d, não tem nenhuma aldeia para transformar.\n", jogador_num); return; }
		
		// A linha para perguntar ao jogador
		char quer_transformar_pergunta[70] = {0};
		snprintf(quer_transformar_pergunta, 70, "Jogador %1d, escolha o que quer transformar (-1 para não transformar):", jogador_num);
		
		// Loop para perguntar
		while ( true )
		{
			// Perguntar a cela
			int cela_num = perguntar_num( quer_transformar_pergunta );
			
			// Ver erros
			if ( cela_num == -1 )                                      { break; }
			if ( cela_num < 0 )                                        { printf("Numero não pode ser negativo e não -1.\n"); continue; }
			if ( cela_num >= jogo->xdim * jogo->ydim )                 { printf("Numero não pode ser maior que o numero de celas.\n"); continue; }
			if ( jogo->celas[ cela_num ].tipo != cela_aldeia )         { printf("Essa cela não é uma aldeia.\n"); continue; }
			if ( jogo->celas[ cela_num ].proprietario != jogador_num ) { printf("Essa cela não é sua.\n"); continue; }
			
			// Tirar-lhe os materiais
			jogador->cartas[ recurso_natural_grain ] -= 2;
			jogador->cartas[ recurso_natural_iron  ] -= 3;
			
			// Transformar a cela
			jogo->celas[ cela_num ].tipo = cela_cidade;
			
			// Dar o ponto
			// Só ++ e não += 2 porque ele já têm um da aldeia
			jogador->pontos++;
			
			// Break quando tivermos o numero
			break;
		} // Ver qual transformar
	}
	
	void processar_troca_recursos_por_recursos(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Se o jogador não tiver pelo menos 4 de algo
		if ( !jogador_tem_recurso_pelo_menos(jogador, 4) ) { printf("Jogador %1d, precisa de pelo menos 4 de algum recurso para trocar recursos por outros com o banco.\n", jogador_num); return; }
		
		// A linha para perguntar ao jogador o recurso a dar
		char quer_trocar_recursos_por_recursos_recurso_dar_pergunta[120] = {0};
		snprintf(quer_trocar_recursos_por_recursos_recurso_dar_pergunta, 120, 
		"0 - Wool  \n"
		"1 - Brick \n"
		"2 - Lumber\n"
		"3 - Grain \n"
		"4 - Iron  \n"
		"Jogador %1d, escolha o que quer trocar (-1 para não trocar):", jogador_num);
		
		// Se quer continuar a trocar
		bool quer_trocar_recursos_por_recursos = true;
		
		// O recurso a dar e receber
		int recurso_dar;
		int recurso_receber;
		
		// Loop para perguntar qual o recurso a dar
		while ( quer_trocar_recursos_por_recursos )
		{
			// Perguntar o recurso
			recurso_dar = perguntar_num( quer_trocar_recursos_por_recursos_recurso_dar_pergunta );
			
			// Ver erros
			if ( recurso_dar == -1 )                  { quer_trocar_recursos_por_recursos = false; break; }
			if ( recurso_dar < 0 )                    { printf("Numero não pode ser negativo e não -1.\n"); continue; }
			if ( recurso_dar > 4 )                    { printf("Numero não pode ser maior que 4.\n"); continue; }
			if ( jogador->cartas[ recurso_dar ] < 4 ) { printf("Precisa de pelo menos 4 do recurso para trocar.\n"); continue; }
			
			// Break quando tivermos o numero
			break;
		}
		
		// A linha para perguntar ao jogador o recurso a receber
		char quer_trocar_recursos_por_recursos_recurso_receber_pergunta[69] = {0};
		snprintf(quer_trocar_recursos_por_recursos_recurso_receber_pergunta, 69, "Jogador %1d, escolha o que quer receber em troco (-1 para não trocar)", jogador_num);
		
		// Loop para perguntar o recurso a receber
		while ( quer_trocar_recursos_por_recursos )
		{
			// Perguntar o recurso
			recurso_receber = perguntar_num( quer_trocar_recursos_por_recursos_recurso_receber_pergunta );
			
			// Ver erros
			if ( recurso_receber == -1 )          { quer_trocar_recursos_por_recursos = false; break; }
			if ( recurso_receber < 0 )            { printf("Numero não pode ser negativo e não -1.\n"); continue; }
			if ( recurso_receber > 4 )            { printf("Numero não pode ser maior que 4.\n"); continue; }
			if ( recurso_receber == recurso_dar ) { printf("Não pode trocar um recurso pelo mesmo.\n"); continue; }
			
			// Break quando tivermos o numero
			break;
		} // Ver qual quer receber
		
		// Executar a troca se ainda quiser trocar
		if ( quer_trocar_recursos_por_recursos )
		{
			jogador->cartas[ recurso_dar     ] -= 4;
			jogador->cartas[ recurso_receber ] += 1;
		}
	}
	
	void processar_troca_recursos_por_pontos(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Se o jogador não tem pelo menos 10 de um recurso
		if ( !jogador_tem_recurso_pelo_menos(jogador, 10) ) { printf("Jogador %1d, precisa de pelo menos 10 de algum recurso para trocar recursos por pontos com o banco.\n", jogador_num); return; }
		
		// A linha a perguntar ao jogador
		char quer_trocar_recursos_por_pontos_recurso_pergunta[120] = {0};
		snprintf(quer_trocar_recursos_por_pontos_recurso_pergunta, 120, 
		"0 - Wool  \n"
		"1 - Brick \n"
		"2 - Lumber\n"
		"3 - Grain \n"
		"4 - Iron  \n"
		"Jogador %1d, escolha o que quer trocar (-1 para não trocar):", jogador_num);
		
		// Loop para perguntar
		while ( true )
		{
			// Perguntar o recurso
			int recurso = perguntar_num( quer_trocar_recursos_por_pontos_recurso_pergunta );
			
			// Ver erros
			if ( recurso == -1 )                   { break; }
			if ( recurso < 0 )                     { printf("Numero não pode ser negativo e não -1.\n"); continue; }
			if ( recurso > 4 )                     { printf("Numero não pode ser maior que 4.\n"); continue; }
			if ( jogador->cartas[ recurso ] < 10 ) { printf("Precisa de pelo menos 4 do recurso para trocar.\n"); continue; }
			
			// Executar a troca
			jogador->cartas[ recurso ] -= 10;
			jogador->pontos++;
			
			// Break quando tivermos o numero
			break;
		}
	}
	
	void processar_turno_jogador(Jogador* jogador, int jogador_num, Jogo* jogo)
	{
		// Processar Dados e cartas
		processar_cartas_jogador(jogador, jogador_num, jogo);
		
		// A linha para perguntar ao jogador
		char quer_fazer_num_pergunta[154] = {0};
		snprintf(quer_fazer_num_pergunta, 154, 
		"0 - Comprar aldeias\n"
		"1 - Transformar aldeias\n"
		"2 - Trocar recursos com o banco\n"
		"3 - Trocar pontos com o banco\n"
		"4 - Passar o turno\n"
		"Jogador %1d, o que quer fazer?", jogador_num);
		
		// O que quer fazer
		int quer_fazer = -1;
		
		// Loop para perguntar
		while ( quer_fazer != 4 )
		{
			// Mostrar as cartas
			printf("As suas cartas, jogador %d:\n", jogador_num);
			print_cartas(jogador);
			
			// Perguntar
			quer_fazer = perguntar_num(quer_fazer_num_pergunta);
			
			// Ver errors
			if ( quer_fazer < 0 ) { printf("Numero não pode ser negativo.\n");    continue; }
			if ( quer_fazer > 4 ) { printf("Numero não pode ser maior que 4.\n"); continue; }
			
			
			// Fazer ações
			     if ( quer_fazer == 0 ) { processar_comprar_celas              (jogador, jogador_num, jogo); continue; }
			else if ( quer_fazer == 1 ) { processar_transformar_celas          (jogador, jogador_num, jogo); continue; }
			else if ( quer_fazer == 2 ) { processar_troca_recursos_por_recursos(jogador, jogador_num, jogo); continue; }
			else if ( quer_fazer == 3 ) { processar_troca_recursos_por_pontos  (jogador, jogador_num, jogo); continue; }
			else if ( quer_fazer == 4 ) { break; }
		}
	}
//----------------------------------------------//
