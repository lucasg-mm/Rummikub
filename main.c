#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pedrinhas{
	char numero;
	char cor;
	struct pedrinhas* prox;
	
}PEDRA;

void cabecalho(){
	system("clear");
	printf("--------------------------------------------------------------------------------");
	printf("            RUMMIKLEBERSON");
	printf("\n--------------------------------------------------------------------------------\n");
	
	return;
}

void inicializa_jogadores(PEDRA** jogadores, int num_jogadores){  //Cria a lista das mãos dos jogadores.
	int i;
	
	for(i = 0; i < num_jogadores; i++){
		jogadores[i] = NULL;
	}
	
	return;
}

void inicializa_pilha(PEDRA** pilha){
	*pilha = NULL;  //Pilha inicialmente vazia.
	
	return;
}

int le_baralho(PEDRA** pilha){  //Lê o baralho do arquivo .txt.
	FILE* arquivo;
	PEDRA* novo;
	int c;
	int flag;
	int num;
	
	num = 0;
	c = 0;
	flag = 0;
	
	arquivo = fopen("baralho.txt", "r");
	
	if(arquivo == NULL){
		cabecalho();
		printf("##ERRO NA ABERTURA DO ARQUIVO!!");
		exit(1);
	}
	
	while(flag != EOF){  //Enquanto não atingir o fim do arquivo. (--Cuidado, pois o getc não ignora o '\n').
		fseek(arquivo, c, SEEK_SET);
		
		novo = (PEDRA*)calloc(1, sizeof(PEDRA));  //Cria um elemento para ser colocado no fim da lista.
		
		if(novo == NULL){
			cabecalho();
			printf("##ERRO DE MEMÓRIA!!");
			exit(1);
		}
		
		fscanf(arquivo, "%c%c ", &(novo->numero), &(novo->cor));  //Preenche os elementos. (~~O fscanf já consome os whitespace QUANDO LÊ STRING~~) (--Isso não funciona quando o especificador é %c--) (~~quando lê o meio de uma linha, não move o ponteiro do arquivo~~).			
		c = ftell(arquivo);
		flag = getc(arquivo);
		
		novo->prox = *pilha;  //Insere o elemento no começo da lista.
		*pilha = novo;
		num++;
	}
	
	fclose(arquivo);
	
	return num;
}

void embaralha(PEDRA** pilha, int num){  //Passa as pedras para um array; Embaralha o array; Reconstrói a lista na ordem do array.
	PEDRA* percorre;
	PEDRA** aleatorio;
	int i;
	
	percorre = *pilha;
	
	aleatorio = (PEDRA**)calloc(num, sizeof(PEDRA*));  //Aloca dinamicamente o array.
	
	if(aleatorio == NULL){
		cabecalho();
		printf("##ERRO DE ALOCAÇÃO!!");
		getchar();
		exit(1);
	}
	
	for(i = 0; i < num; i++){  //Passa as pedras para o array.
		aleatorio[i] = percorre;
		percorre = percorre->prox;
	}
	
	srand((unsigned)time(NULL));  //Embaralha o array. (MÉTODO BEN PFAFF).
    for (i = 0; i < num - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (num - i) + 1);
        PEDRA* t = aleatorio[j];
        aleatorio[j] = aleatorio[i];
        aleatorio[i] = t;
	}	
	
	*pilha = aleatorio[0];  //Reconectando o array.
	
	for(i = 0; i < num; i++){
		if(i == (num-1)){
			aleatorio[i]->prox = NULL;
			break;
		}
		
		aleatorio[i]->prox = aleatorio[i+1];
	}
	
	return;
}

int main(void){		
//Declaração de variáveis------------------------------------------------	
	int num_jogadores;  //Guarda o número de jogadores que estão participando.
	int i;
	int num;  //Guarda o número de pedras na pilha.
	PEDRA** jogadores;  //Trata-se de um vetor de ponteiros. Esse vetor guarda listas encadeadas que corres pondem as mãos de cada jogador.
	PEDRA* pilha;  //Aponta para o primeiro elemento da pilha de pedras.
	
//Atribuições e funções iniciais------------------------------------------------------------
	inicializa_pilha(&pilha);	
	
//Splash-screen----------------------------------------------------------
	cabecalho();
	printf("\n\n~~Pressione <ENTER> para continuar~~");
	getchar();
		
//Informações iniciais---------------------------------------------------
	cabecalho();
	printf("\n\n>>Insira quantos jogadores participarão (MIN  2 / MAX 4):");
	scanf("%d", &num_jogadores);
	
//Cria a mão dos jogadores-----------------------------------------------	
	jogadores = (PEDRA**)calloc(num_jogadores, sizeof(PEDRA*));
	
	if(jogadores == NULL){
		cabecalho();
		printf("##ERRO DE ALOCAÇÃO!!");
		getchar();
		exit(1);
	}
	
	inicializa_jogadores(jogadores, num_jogadores);  //Inicializa as mãos dos jogadores. 
	
	num = le_baralho(&pilha);  //Função para ler baralho em texto. 
	
	embaralha(&pilha, num);  //Função para embaralhar.
	
//-----------------------------------------------------------------------
//Cada jogador pega uma pedra do monte. Aquele que pegar a pedra com valor mais alto fica determinado como o primeiro a jogar.
    //~~FUNÇÃO PARA DETERMINAR QUEM É O PRIMEIRO. 
	
//-----------------------------------------------------------------------
//14 peças são distribuídas para cada jogador.
	//~~FUNÇÃO PARA TRANSFERIR PEDRA DA PILHA PARA AS MÃOS (A PILHA RESULTANTE É O SHOPPING).

//-----------------------------------------------------------------------
//Cada jogador deve jogar uma ou mais séries de no mínimo 30 pontos. Se não puder fazer isso, ele deve recolher uma pedra e passar a sua vez.

//-----------------------------------------------------------------------
//O jogo (loop).

//-----------------------------------------------------------------------
//Dá free nas listas e exibe mensagem de vitória (FIM).	
	
//-----------------------------------------------------------------------
	
return 0;	
}