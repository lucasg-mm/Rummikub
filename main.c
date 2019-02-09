#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pedrinhas {
    char numero;
    char cor;
    int inteiro;
    struct pedrinhas* prox;

} PEDRA;

//PROTÓTIPOS:
char* inverte_string(char* ins);
char* limpa_string(char* string, int quantos);
void monta_serie(char* ins, PEDRA** tab, int p_um, PEDRA** jogadores);
void exclui_serie(PEDRA** tab, int serie);
int valida_tab(PEDRA** tab, int total_series);
int contabiliza(char* ins);

void cabecalho() {
    system("clear");
    printf("--------------------------------------------------------------------------------");
    printf("            RUMMIKLEBERSON");
    printf("\n--------------------------------------------------------------------------------\n");

    return;
}

void inicializa_jogadores(PEDRA** jogadores, int num_jogadores) { //Cria a lista das mãos dos jogadores.
    int i;

    for(i = 0; i < num_jogadores; i++) {
        jogadores[i] = NULL;
    }

    return;
}

void inicializa_pilha(PEDRA** pilha) {
    *pilha = NULL;  //Pilha inicialmente vazia.

    return;
}

int le_baralho(PEDRA** pilha) { //Lê o baralho do arquivo .txt.
    FILE* arquivo;
    PEDRA* novo;
    int c;
    int flag;
    int num;

    num = 0;
    c = 0;
    flag = 0;

    arquivo = fopen("baralho.txt", "r");

    if(arquivo == NULL) {
        cabecalho();
        printf("##ERRO NA ABERTURA DO ARQUIVO!!");
        exit(1);
    }

    while(flag != EOF) { //Enquanto não atingir o fim do arquivo. (--Cuidado, pois o getc não ignora o '\n').
        fseek(arquivo, c, SEEK_SET);

        novo = (PEDRA*)calloc(1, sizeof(PEDRA));  //Cria um elemento para ser colocado no fim da lista.

        if(novo == NULL) {
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

void embaralha(PEDRA** pilha, int num) { //Passa as pedras para um array; Embaralha o array; Reconstrói a lista na ordem do array.
    PEDRA* percorre;
    PEDRA** aleatorio;
    int i;

    percorre = *pilha;

    aleatorio = (PEDRA**)calloc(num, sizeof(PEDRA*));  //Aloca dinamicamente o array.

    if(aleatorio == NULL) {
        cabecalho();
        printf("##ERRO DE ALOCAÇÃO!!");
        getchar();
        exit(1);
    }

    for(i = 0; i < num; i++) { //Passa as pedras para o array.
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

    for(i = 0; i < num; i++) {
        if(i == (num-1)) {
            aleatorio[i]->prox = NULL;
            break;
        }

        aleatorio[i]->prox = aleatorio[i+1];
    }
    
	free(aleatorio);
    return;
}

void inteiro(PEDRA** pilha) { //Converte char para int nos structs.
    PEDRA* percorre;

    percorre = *pilha;

    while(percorre != NULL) {
        percorre->inteiro = strtol(&percorre->numero, NULL, 16);

        percorre = percorre->prox;
    }  //Obs: vai ficar com lixo de memória no campo dos inteiros quando for coringa. (vira 0)

    return;
}

void deleta_pilha(PEDRA** pilha) { //Deleta apenas o primeiro nó da pilha.
    PEDRA* no;

    no = *pilha;
    *pilha = no->prox;
    free(no);
    return;
}

void distribui(PEDRA** pilha, int num_jogadores, PEDRA** jogadores, int p_um) {
    int i;
    int j;
    PEDRA* percorre;
    PEDRA* novo;

    percorre = *pilha;

    for(i = 0; i < num_jogadores; i++) {
        j = 0;

        while(j < 14) {
            novo = (PEDRA*)calloc(1, sizeof(PEDRA));

            if(novo == NULL) {
                cabecalho();
                printf("\n\n##ERRO DE MEMÓRIA!!");
                getchar();
                exit(1);
            }

            novo->numero = percorre->numero;
            novo->cor = percorre->cor;
            novo->inteiro = percorre->inteiro;

            novo->prox = jogadores[p_um - 1];
            jogadores[p_um - 1] = novo;

            j++;
            percorre = percorre->prox;
            deleta_pilha(pilha);
        }
        if(p_um < num_jogadores) {
            p_um++;
        }
        else {	//	 p_um >= num_jogadores
            p_um = 1;
        }
    }

    return;
}

void imprime_pilha(PEDRA* pilha) { //**Apenas para TESTE**
    cabecalho();

    printf("\n-->PILHA DE PEDRAS\n");

    while(pilha != NULL) {
        printf("NUMERO: %d      COR: %c\n", pilha->inteiro, pilha->cor);
        pilha = pilha->prox;
    }
    return;
}

void imprime_mao(PEDRA** jogadores, int num) { //**Apenas para TESTE**
    int i;
    PEDRA* percorre;

    cabecalho();

    for(i = 0; i < num; i++) {
        printf("\n-->MÃO DO JOGADOR %d\n", i+1);
        percorre = jogadores[i];

        while(percorre != NULL) {
            printf("NUMERO: %d      COR: %c\n", percorre->inteiro, percorre->cor);
            percorre = percorre->prox;
        }
    }

    return;
}

void imprime_idv(PEDRA** jogadores, int num) { //Imprime a mão individual.
    PEDRA* percorre;

    percorre = jogadores[num - 1];

    printf("\n>>SUAS PEDRAS:");

    while(percorre != NULL) {
        printf(" %c%c", percorre->numero, percorre->cor);
        percorre = percorre->prox;
    }
    return;
}

void imprime_tab(PEDRA* tab) { //Imprime o tabuleiro.
    printf("\n>>TABULEIRO:");

    while(tab != NULL) {
        if(tab->numero != 'X') {
            printf(" %c%c", tab->numero, tab->cor);
        }
        else {
            printf("\n");
        }

        tab = tab->prox;
    }

    return;
}

void exclui_mao(char nro, char cr, PEDRA** jogadores, int p_um) {  //#BUG!!
    PEDRA* percorre;
    PEDRA* ant;

    percorre = jogadores[p_um - 1];

    while((percorre->numero != nro) || (percorre->cor != cr)) {
        ant = percorre;
        percorre = percorre->prox;
    }

    if(percorre == jogadores[p_um - 1]) { //Se tiver que remover o primeiro.
        jogadores[p_um - 1] = percorre->prox;
    }
    else { //Se não
        ant->prox = percorre->prox;
    }
    free(percorre);

    return;
}

int existe(PEDRA** jogadores, int p_um, char nro, char cr) { //Verifica se a pedra que o jogador quer baixar existe em sua mão.
    PEDRA* percorre;

    percorre = jogadores[p_um - 1];

    while((percorre != NULL) && ((percorre->numero != nro) || (percorre->cor != cr))) {
        percorre = percorre->prox;
    }

    if(percorre == NULL) {
        return 0;
    }
    else {
        return 1;  //Achou!
    }
}

int existe_tab(PEDRA** tab, char nro, char cr, int serie) { //Procura no tabuleiro
    PEDRA* percorre;
    int contador = 0;

    percorre = *tab;
    percorre = percorre->prox;

    while(contador < serie) {
        percorre = percorre->prox;

        if(percorre->numero == 'X') {
            contador++;
            percorre = percorre->prox;
        }
    }

    while((percorre != NULL) && (percorre->numero != 'X') && ((percorre->numero != nro) || (percorre->cor != cr))) {
        percorre = percorre->prox;
    }

    if((percorre == NULL) || (percorre->numero == 'X')) {
        return 0;
    }
    else {
        return 1;  //Achou!
    }
}

int primeira_jogada(PEDRA** tab, PEDRA** jogadores, int p_um, char* ins, int modo, int* total_series) { 
    int ptos = 0;
    int flag;
    char flag2 = 'X';
    int i = 0;
    int j = 0;
    int vale;  //Diz se a jogada foi válida.
    int grupo = 0;
    int seq = 0;
    int quantos = 0;
    PEDRA* novo;

    while(ins[quantos] != '\n') {
        quantos++;
    }

    while(ins[i] != '\0') {
		if(ins[i] == 'M'){
            flag = existe(jogadores, p_um, ins[i+1], ins[i+2]);

            if(flag == 0) {
                cabecalho();
                printf("\n\n##A jogada não foi valida, pois voce nao tem essas pedras. Tente novamente!\n");
                getchar();
                setbuf(stdin, NULL);

                return 0;  //Se a jogada for inválida. (RETORNA ZERO)
            }
		}

        i = i + 4;
    }

    //Valida a jogada.
    //-->Grupo:

    j = 1;
    i = 1;
    while(ins[i] != '\0') { //Mesmo número, cores diferentes (min 3 / max 4).
        if((ins[j] != '*') && (flag2 == 'X')) {
            flag2 = ins[j];
        }

        if(ins[i] != '*') {
            if(ins[i] == flag2) { //Certifica a parte do "mesmo número".
                grupo = 1;
            }
            else {
                grupo = 0;
                break;
            }
        }

        j = j + 4;
        i = i + 4;
    }

    //Certifica a parte do "cores diferentes".
    if(grupo == 1) {
        for(i = 2; i < quantos; i = i + 4) {
            if(ins[i] != '*') { //Se for curinga, ignora!
                for(j = i + 4; j < quantos; j = j + 4) {
                    if(ins[i] != ins[j]) {
                        grupo = 1;
                    }
                    else {
                        grupo = 0;
                        break;
                    }
                }
                if(ins[i] == ins[j]) {
                    break;
                }
            }
        }
    }

    if((grupo == 1) && ((quantos < 11) || (quantos > 15))) { //Limitação do número de pedras.
        cabecalho();
        printf("\n\n##Essa jogada não foi valida (lembre-se: no mínimo 3 pedras e no máximo 4). Tente novamente!\n");
        getchar();
        setbuf(stdin, NULL);

        return 0;  //Se a jogada for inválida. (RETORNA ZERO)
    }

    //-->Sequência:
    //Certifica se têm cores iguais
    i = 2;
    j = 2;
    flag2 = 'X';
    while(ins[i] != '\0') {
        if((ins[j] != '*') && (flag2 == 'X')) {
            flag2 = ins[j];
        }

        if(ins[i] != '*') {
            if(flag2 == ins[i]) {
                seq = 1;
            }
            else {
                seq = 0;
                break;
            }
        }
        i = i + 4;
        j = j + 4;
    }

    //Certifica a parte da "sequência numérica":

    i = 1;
    if(seq == 1) {
        while(ins[i] != '\0') { //Muda o condicional!!
            if(ins[i] != '*') {
                if((strtol(&ins[i+4], NULL, 16) == strtol(&ins[i], NULL, 16) + 1) || (strtol(&ins[i+8], NULL, 16) == strtol(&ins[i], NULL, 16) + 2) || (ins[i+4] == '\0') || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-4], NULL, 16) + 1) || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-8], NULL, 16) + 2)) {
                    seq = 1;
                }
                else {
                    seq = 0;
                    break;
                }
            }
            i = i + 4;
        }
    }

    if((seq == 1) && (quantos < 11)) { //Limitação do número de pedras.
        cabecalho();
        printf("\n\n##Essa jogada não foi valida (lembre-se: no mínimo 3). Tente novamente!\n");
        getchar();
        setbuf(stdin, NULL);

        return 0;  //Se a jogada for inválida. (RETORNA ZERO)
    }

    //-->Por fim:

    vale = grupo | seq;

    if(vale == 0) {
        cabecalho();
        printf("\n\n##Essa jogada não foi valida. Tente novamente!\n");
        getchar();
        setbuf(stdin, NULL);

        return 0;  //Se a jogada for inválida. (RETORNA ZERO)
    }
	
	if(modo == 1){
        ptos = contabiliza(ins);
        monta_serie(ins, tab, p_um, jogadores);
		(*total_series)++;
        return ptos;
	}

    return 1;
}

int aglutina(PEDRA** tab, char nro, char cr, int serie, PEDRA** jogadores, int p_um, int* total_series) {
    char ins1[70];
    char ins2[70];
    int i;
    int j;

    limpa_string(ins1, 70);
    limpa_string(ins2, 70);

    i = 0;

    PEDRA* percorre;
    int contador = 0;

    percorre = *tab;
    percorre = percorre->prox;

    while(contador < serie) {
        percorre = percorre->prox;

        if(percorre->numero == 'X') {
            contador++;
            percorre = percorre->prox;
        }
    }

    while((percorre != NULL) && (percorre->numero != 'X') && ((percorre->numero != nro) || (percorre->cor != cr))) {
        ins1[i] = 'X';
        ins1[i+1] = percorre->numero;
        ins1[i+2] = percorre->cor;
        ins1[i+3] = ' ';
        i = i+4;

        percorre = percorre->prox;

        if((percorre == NULL) || (percorre->numero == 'X') || ((percorre->numero == nro) && (percorre->cor == cr))) {
            ins1[i-1] = '\n';
        }
    }

    inverte_string(ins1);

    i = 0;
    percorre = percorre->prox;
    while((percorre != NULL) && (percorre->numero != 'X')) {
        ins2[i] = 'X';
        ins2[i+1] = percorre->numero;
        ins2[i+2] = percorre->cor;
        ins2[i+3] = ' ';
        i = i+4;

        percorre = percorre->prox;

        if((percorre == NULL) || (percorre->numero == 'X')) {
            ins2[i-1] = '\n';
        }
    }

    inverte_string(ins2);

    i = 0;
    while(ins1[i] != '\n') {
        i++;
    }

    ins1[i] = ' ';
    ins1[i+1] = '\0';

    strcat(ins1, ins2);   //Combinar strings. (acabar com o '\n')

    monta_serie(ins1, tab, p_um, jogadores);
	(*total_series)++;

    return 1;
}

int contabiliza(char* ins) {
    int ptos = 0;
    int i;
    int quantos;

    quantos = 0;
    i=0;

    while(ins[quantos] != '\n') {
        quantos++;
    }

    while(ins[i] != '\0') {
        if(ins[i] == 'M') {
            i++;
            if(ins[i] != '*') {
                ptos = ptos + strtol(&ins[i], NULL, 16);
            }
            else {
                if(i == 0) {
                    //Coringa no começo
                    if(strtol(&ins[i+4], NULL, 16) == (strtol(&ins[i+8], NULL, 16) + 1)) { //Uma sequência.
                        ptos = ptos + strtol(&ins[i+4], NULL, 16) - 1;
                    }
                    if(strtol(&ins[i+4], NULL, 16) == strtol(&ins[i+8], NULL, 16)) { //Um grupo.
                        ptos = ptos + strtol(&ins[i+4], NULL, 16);
                    }
                }
                if(i == (quantos - 2)) {	//Coringa no fim
                    if(strtol(&ins[i-4], NULL, 16) == (strtol(&ins[i-8], NULL, 16) + 1)) { //Uma sequencia
                        ptos = ptos + strtol(&ins[i-4], NULL, 16) + 1;
                    }

                    if(strtol(&ins[i-4], NULL, 16) == strtol(&ins[i-8], NULL, 16)) { //Um grupo
                        ptos = ptos + strtol(&ins[i-4], NULL, 16);
                    }
                }
                if((i != quantos) && (i != 0)) { //Coringa no meio
                    if(strtol(&ins[i+4], NULL, 16) == (strtol(&ins[i-4], NULL, 16) + 2)) {
                        ptos = ptos + strtol(&ins[i-4], NULL, 16) + 1;
                    }

                    if(strtol(&ins[i-4], NULL, 16) == strtol(&ins[i+4], NULL, 16)) {
                        ptos = ptos + strtol(&ins[i-4], NULL, 16);
                    }
                }
            }
            i--;
        }
        i = i + 4;
    }

    return ptos;
}

void monta_serie(char* ins, PEDRA** tab, int p_um, PEDRA** jogadores) {
    int i;
    PEDRA* novo;

    i = 1;
    while(ins[i] != '\0') {
        if(ins[i-1] == 'M') { //Exclui somente os da mão.
            exclui_mao(ins[i], ins[i+1], jogadores, p_um);  //~FUNÇÃO PARA EXCLUIR DA MÃO.
        }

        //Manipula a lista do tabuleiro.

        novo = (PEDRA*)calloc(1, sizeof(PEDRA));
        if(novo == NULL) {
            cabecalho();
            printf("##ERRO DE ALOCAÇÃO!!");
            getchar();
            exit(1);
        }

        novo->inteiro = strtol(&ins[i], NULL, 16);
        novo->numero = ins[i];
        novo->cor = ins[i+1];

        novo->prox = *tab;
        *tab = novo;

        if(ins[i+3] == '\0') {
            novo = (PEDRA*)calloc(1, sizeof(PEDRA));
            if(novo == NULL) {
                cabecalho();
                printf("##ERRO DE ALOCAÇÃO!!");
                getchar();
                exit(1);
            }

            novo->numero = 'X';
            novo->cor = 'X';

            novo->prox = *tab;
            *tab = novo;
        }

        i = i + 4;
    }
    return;
}

void exclui_serie(PEDRA** tab, int serie) { //Exclui uma serie por completo.
    PEDRA* percorre;
    PEDRA* percorre2;
    PEDRA* percorre3;
    PEDRA* percorre4;
    int contador = 0;
    int flag = 0;

    percorre = *tab;
    percorre2 = *tab;

    if(contador != serie) {
        percorre = percorre->prox;
        percorre2 = percorre2->prox;

        while(contador < serie) { //E se eu desejo alterar a primeira série? (e a última?)
            percorre = percorre->prox;

            if(percorre->prox->numero == 'X') {
                contador++;
            }
        }
        flag = 1;
    }
    //"Percorre" agora aponta para o ultimo elemento antes da serie a ser excluida.

    contador = 0;

    while(contador < (serie+1)) { //Não se tem certeza se essa "série + 1" existe...
        percorre2 = percorre2->prox;

        if(percorre2 == NULL) {
            break;
        }

        if(percorre2->numero == 'X') {
            contador++;
        }
    }

    if(flag == 1) {
        percorre4 = percorre->prox;
    } else {
        percorre4 = percorre;
    }

    percorre3 = percorre4;

    while(percorre4 != percorre2) {
        percorre3 = percorre4;
        if(percorre4 != NULL) {
            percorre4 = percorre4->prox; //Alteração aqui!
        }		
        free(percorre3);
    }
    if(flag == 1) {
        percorre->prox = percorre2;
    } else {
        *tab = percorre2;
    }

    return;
}

int ret_ext(PEDRA** tab, int modo, int serie, PEDRA** jogadores, int p_um, int* total_series) {  //Retira da extremidade
    char ins[70];
    int i;
	int j;

    limpa_string(ins, 70);

    PEDRA* percorre;
    int contador = 0;

    percorre = *tab;
    percorre = percorre->prox;

    while(contador < serie) {
        percorre = percorre->prox;

        if(percorre->numero == 'X') {
            contador++;
            percorre = percorre->prox;
        }
    }
    
	i = 0;
	
    switch(modo) {
    case 0:  //Retira a ultima pedra.
        percorre = percorre->prox;

        while((percorre != NULL) && (percorre->numero != 'X')) {
            ins[i] = 'X';
            ins[i+1] = percorre->numero;
            ins[i+2] = percorre->cor;
            ins[i+3] = ' ';
            i = i+4;

            percorre = percorre->prox;

            if((percorre == NULL) || (percorre->numero == 'X')) {
                ins[i-1] = '\n';
            }
        }

        break;

    case 1:  //Retira a última pedra.
        while((percorre != NULL) && (percorre->numero != 'X')) {
            ins[i] = 'X';
            ins[i+1] = percorre->numero;
            ins[i+2] = percorre->cor;
            ins[i+3] = ' ';
            i = i+4;
			
			if((percorre->prox->prox == NULL) || (percorre->prox->prox->numero == 'X')){
				ins[i-1] = '\n';
				break;
			}            
			
            percorre = percorre->prox;

            if((percorre == NULL) || (percorre->numero == 'X')) {
                ins[i-1] = '\n';
            }
        }
			
        break;

    default:

        exit(1);
    }
	inverte_string(ins);

    monta_serie(ins, tab, p_um, jogadores);
	(*total_series)++;

    return 1;
}

char* limpa_string(char* string, int quantos){
	int i;
	
	for(i = 0;  i < quantos; i++){
		string[i] = '\0';
	}
	
	return string;
}

char* inverte_string(char* ins){
	int i;
	int j;
	int quantos;
	char aux[70];
	
	limpa_string(aux, 70);
	
    if(ins[0] != '\0') {
         i = 0;
         quantos = 0;
         while(ins[i] != '\n') {
             quantos++;
             i++;
         }

         for(i = 0; i < quantos; i++) {
             aux[i] = ins[i];
         }

         j = quantos;  //CONTINUAR AQUI
         for(i = 0; i < quantos; i = i + 4) {
             ins[i] = aux[j-3];
             ins[i+1] = aux[j-2];
             ins[i+2] = aux[j-1];
             if(i != 0) {
                 ins[i+3] = aux[j];
             }
             else {
                 ins[i+3] = ' ';
             }

             j = j - 4;
         }

         ins[i-1] = '\n';
     }
	
	return ins;
}

int divide(PEDRA** tab, char nro, char cr, int serie, PEDRA** jogadores, int p_um, int* total_series){
	char ins1[50];
	char ins2[50];
	PEDRA* percorre;
    int contador = 0;
	int flag = 0;
	int i = 0;

    percorre = *tab;
    percorre = percorre->prox;
	
	limpa_string(ins1, 50);
	limpa_string(ins2, 50);

    while(contador < serie) {
        percorre = percorre->prox;

        if(percorre->numero == 'X') {
            contador++;
            percorre = percorre->prox;
        }
    }	
	
	while((percorre != NULL) && (percorre->numero != 'X')){
		if(flag == 0){  //primeira metade
            ins1[i] = 'X';
            ins1[i+1] = percorre->numero;
            ins1[i+2] = percorre->cor;
            ins1[i+3] = ' ';
            i = i+4;			
			
			if((percorre->cor == cr) && (percorre->numero == nro)){
				ins1[i-1] = '\n';
				
				i = 0;
				flag = 1;
				ins2[i] = 'M';
                ins2[i+1] = percorre->numero;
                ins2[i+2] = percorre->cor;
                ins2[i+3] = ' ';
                i = i+4;
			}
			
			percorre = percorre->prox;
		}
		else{  //segunda metade
            ins2[i] = 'X';
            ins2[i+1] = percorre->numero;
            ins2[i+2] = percorre->cor;
            ins2[i+3] = ' ';
            i = i+4;
			
			percorre = percorre->prox;
			
            if((percorre == NULL) || (percorre->numero == 'X')) {
                ins2[i-1] = '\n';
            }			
		}
	}
	
	inverte_string(ins1);
	inverte_string(ins2);
	
	monta_serie(ins1, tab, p_um, jogadores);
	monta_serie(ins2, tab, p_um, jogadores);
	*total_series = (*total_series + 2);
	
	return 1;
}

int jogada(PEDRA** tab, PEDRA** jogadores, int p_um, int* baixou, int* total_series, int* validou) { //Retorna os pontos da jogada.
    int opcao;
    int ptos;
    int opcao2;
    int i;
    int serie_dest;  //Só importa na hora de conectar as series
    char ins[70];
    char uni[10];
    int modo;
	int flag = 0; 

    cabecalho();
    imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
    imprime_tab(*tab);
    printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
    printf("(1)Adicionar pedras a uma serie (da mao)\n");  //FUNCIONA!!
    printf("(2)Criar uma nova serie (da mao)\n");  //FUNCIONA!!
    printf("(3)Remover uma de um grupo e usa-la para formar uma serie\n");  //FUNCIONA!!
    printf("(4)Remover uma pedra de uma sequencia e usa-la para formar uma serie\n");  //FUNCIONA!!
	printf("(5)Dividir uma sequencia\n");  //FUNCIONA!!
	printf("(6)Adicionar pedras (de outra serie)\n");  //FUNCIONA!!

    scanf("%d", &opcao);

    switch(opcao) {
    case 1:
        limpa_string(ins, 70);

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);
        printf("\n\n>>Em qual serie deseja inserir?\n");  //Informação necessária para manipular os ponteiros
        scanf("%d", &serie_dest);
        setbuf(stdin, NULL);

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);
        printf("\n\n>>Atualize a serie:\n");
        fgets(ins, 70, stdin);

        exclui_serie(tab, serie_dest);  //Excluindo a serie:
		(*total_series)--;	

        break;
    case 2:
        limpa_string(ins, 70);

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);

        setbuf(stdin, NULL);

        printf("\n\n>>Quais pedras deseja baixar?\n");
        fgets(ins, 70, stdin);	

        break;
    case 3:
		limpa_string(ins, 70);
		limpa_string(uni, 10);

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);

        printf("\n\n>>De qual serie deseja extrair a pedra?\n");
        scanf("%d", &serie_dest);

        setbuf(stdin, NULL);

        printf("\n\n>>Qual pedra deseja extrair?\n");
        fgets(uni, 10, stdin);

        aglutina(tab, uni[0], uni[1], serie_dest, jogadores, p_um, total_series);

        exclui_serie(tab, (serie_dest + 1));  //Excluindo a serie
		(*total_series)--;	

        printf("\n\n>>Escreva a nova serie (inclua a pedra extraida)\n");
        fgets(ins, 70, stdin);

        break;

    case 4:
		limpa_string(ins, 70);

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);

        printf("\n\n>>De qual serie deseja extrair a pedra?\n");
        scanf("%d", &serie_dest);

        setbuf(stdin, NULL);

        printf("\n\n>>Deseja retirar a primeira pedra(1) ou a ultima(0)?\n");
        scanf("%d", &modo);

        ret_ext(tab, modo, serie_dest, jogadores, p_um, total_series);

        exclui_serie(tab, (serie_dest + 1));  //Excluindo a serie
		(*total_series)--;	
			
		setbuf(stdin, NULL);	

        printf("\n\n>>Escreva a nova serie (inclua a pedra extraida)\n");
        fgets(ins, 70, stdin);

        break;
			
	case 5:
        limpa_string(ins, 70);
		limpa_string(uni, 10);	

        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);

        printf("\n\n>>Qual serie deseja dividir?\n");
        scanf("%d", &serie_dest);

        setbuf(stdin, NULL);

        printf("\n\n>>A partir de qual pedra deseja dividir?\n");
        fgets(uni, 10, stdin);
			
		divide(tab, uni[0], uni[1], serie_dest, jogadores, p_um, total_series);
			
		exclui_serie(tab, (serie_dest + 2));  //Excluindo a serie
		(*total_series)--;
		flag = 1;
		
		*baixou = 1;	
			
		break;
			
	case 6:
        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        imprime_tab(*tab);
        printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
        printf("(1)Remover uma quarta pedra de um grupo e usa-la para formar uma serie\n");
        printf("(2)Remover uma pedra de uma sequencia e usa-la para formar uma serie\n"); 
		setbuf(stdin, NULL);	
		scanf("%d", &opcao2);
		
		switch(opcao2){
		case 1:		
		    limpa_string(ins, 70);
		    limpa_string(uni, 10);		

            cabecalho();
            imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
            imprime_tab(*tab);

            printf("\n\n>>De qual serie deseja extrair a pedra?\n");
            scanf("%d", &serie_dest);

            setbuf(stdin, NULL);

            printf("\n\n>>Qual pedra deseja extrair?\n");
            fgets(uni, 10, stdin);

            aglutina(tab, uni[0], uni[1], serie_dest, jogadores, p_um, total_series);

            exclui_serie(tab, (serie_dest + 1));  //Excluindo a serie
			(*total_series)--;	
			setbuf(stdin, NULL);	
			
			break;	
		case 2:
			limpa_string(ins, 70); 

            cabecalho();
            imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
            imprime_tab(*tab);

            printf("\n\n>>De qual serie deseja extrair a pedra?\n");
            scanf("%d", &serie_dest);

            setbuf(stdin, NULL);

            printf("\n\n>>Deseja retirar a primeira pedra(1) ou a ultima(0)?\n");
            scanf("%d", &modo);

            ret_ext(tab, modo, serie_dest, jogadores, p_um, total_series);

            exclui_serie(tab, (serie_dest + 1));  //Excluindo a serie
			(*total_series)--;	
		    setbuf(stdin, NULL);	
				
			break;
		default:
			exit(1);
				
			break;	
		}				
			
		printf("\n\n>>Em qual serie deseja inserir?\n");
		scanf("%d", &serie_dest);
		exclui_serie(tab, (serie_dest + 1));
		(*total_series)--;	
		 
        getchar();
		printf("\n\n>>Reescreva a serie (inclua a pedra extraida)\n");
		setbuf(stdin, NULL);	
        fgets(ins, 70, stdin);
		
		break;	

    default:
        exit(1);

        break;
    }
    
	if(flag != 1){
		i = 0;
		while(ins[i] != '\0'){
			if(ins[i] == 'M'){
				*baixou = 1;
				break;
			}
			i = i + 4;
		}
		
        ptos = contabiliza(ins);  //Contabiliza os pontos da jogada.
        monta_serie(ins, tab, p_um, jogadores);
		(*total_series)++;
	}
	else{
		ptos = contabiliza(uni);
	}
	
	if(valida_tab(tab, *total_series) == 0){
		*validou = 0;
	}
	else{
		*validou = 1;
	}

    return ptos;
}

void libera_jogadores(PEDRA** jogadores, int num_p){
	int i;
	PEDRA* aux;
	
	for(i = 0; i < num_p; i++){
        if(jogadores[i] != NULL){
		    while(jogadores[i] != NULL){
			    aux = jogadores[i];
			    jogadores[i] = jogadores[i]->prox;
			    free(aux);
		    }
	    }		
	}
	
	return;
}

void libera_pilha_tab(PEDRA** p_t){
	if(*p_t != NULL){
		PEDRA* aux;
		
		while(*p_t != NULL){
			aux = *p_t;
			*p_t = (*p_t)->prox;
			free(aux);
		}
	}
	
	return;
}

int valida_tab(PEDRA** tab, int total_series){
	char ins[70];
	int i;
	int j;
	int contador;
	int serie;
	PEDRA* percorre;
	
	j = 0;	
	serie = 0;
	
	while(j < total_series){
	    limpa_string(ins, 70);
		contador = 0;
		percorre = *tab;
        percorre = percorre->prox;
		
        while(contador < serie) {
            percorre = percorre->prox;

            if(percorre->numero == 'X') {
                contador++;
                percorre = percorre->prox;
            }
        }
        i = 0;
        while((percorre != NULL) && (percorre->numero != 'X')) {
            ins[i] = 'X';
            ins[i+1] = percorre->numero;
            ins[i+2] = percorre->cor;
            ins[i+3] = ' ';
            i = i+4;

            percorre = percorre->prox;

            if((percorre == NULL) || (percorre->numero == 'X')) {
                 ins[i-1] = '\n';
            }
        }
		
		inverte_string(ins);
		if(primeira_jogada(tab, NULL, 0, ins, 0, NULL) == 0){
			return 0;  //Verificação falhou
		}
		
		serie++;
		j++;
	}
		
	return 1;
}

int main(void) {
//Declaração de variáveis------------------------------------------------
    int num_jogadores;  //Guarda o número de jogadores que estão participando.
    int i;
    int j;
	int baixou;
    int k;
    int valida;
	int loop;
    int opcao;  //Para usar em menus.
    int opcao2;
    int num;  //Guarda o número de pedras na pilha.
    int p_um;  //Indica qual jogador será o primeiro. (volátil)
    int primeiro;  //Guarda o primeiro valor da variável acima (constante)
    char ins[70];  //Guarda as instruções da jogada.
    PEDRA** jogadores;  //Trata-se de um vetor de ponteiros. Esse vetor guarda listas encadeadas que corres pondem as mãos de cada jogador.
    PEDRA* pilha;  //Aponta para o primeiro elemento da pilha de pedras.
    PEDRA* tab;  //Aponta para o primeiro elemento da pilha que corresponde ao tabuleiro.
    PEDRA* novo;
    int* ptos;  //Guarda os pontos de cada jogador.
    int ganhou = 0;
    int maior = 0;
	int validou = 1;
	int total_series = 0;

//Atribuições e funções iniciais------------------------------------------------------------
    inicializa_pilha(&pilha);
    inicializa_pilha(&tab);
    limpa_string(ins, 70);

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
    ptos = (int*)calloc(num_jogadores, sizeof(int));

    if(jogadores == NULL) {
        cabecalho();
        printf("##ERRO DE ALOCAÇÃO!!");
        getchar();
        exit(1);
    }

    inicializa_jogadores(jogadores, num_jogadores);  //Inicializa as mãos dos jogadores.

    num = le_baralho(&pilha);  //Função para ler baralho em texto.

    embaralha(&pilha, num);  //Função para embaralhar.

    inteiro(&pilha);  //Converte os números em hex na struct para inteiros.

//Cada jogador pega uma pedra do monte-----------------------------------
    switch(num_jogadores) { //(--FALTA INCLUIR CASO DO CORINGA--)
    case 2:
        if(pilha->inteiro >= pilha->prox->inteiro) { //Considera-se que o jogador um é o primeiro a retirar uma carta do baralho.
            p_um = 1;
        }
        else {
            p_um = 2;
        }

        break;

    case 3:
        if((pilha->inteiro >= pilha->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->inteiro)) {
            p_um = 1;
        }

        if((pilha->prox->inteiro > pilha->inteiro ) && (pilha->prox->inteiro > pilha->prox->prox->inteiro)) {
            p_um = 2;
        }

        if((pilha->prox->prox->inteiro > pilha->inteiro ) && (pilha->prox->prox->inteiro > pilha->prox->inteiro)) {
            p_um = 3;
        }

        break;

    case 4:
        if((pilha->inteiro >= pilha->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->inteiro) && (pilha->inteiro >= pilha->prox->prox->prox->inteiro)) {
            p_um = 1;
        }

        if((pilha->prox->inteiro > pilha->inteiro ) && (pilha->prox->inteiro > pilha->prox->prox->inteiro) && (pilha->prox->inteiro > pilha->prox->prox->prox->inteiro)) {
            p_um = 2;
        }

        if((pilha->prox->prox->inteiro > pilha->inteiro ) && (pilha->prox->prox->inteiro > pilha->prox->inteiro) && (pilha->prox->prox->inteiro > pilha->prox->prox->prox->inteiro)) {
            p_um = 3;
        }

        if((pilha->prox->prox->prox->inteiro > pilha->inteiro) && (pilha->prox->prox->prox->inteiro > pilha->prox->inteiro) && (pilha->prox->prox->prox->inteiro > pilha->prox->prox->inteiro)) {
            p_um = 4;
        }

        break;

    default:
        cabecalho();
        printf("\n\n##QUANTIDADE DE JOGADORES INVÁLIDA!!");
        getchar();
        exit(1);
    }

    cabecalho();
    printf("\n\n>>O JOGADOR %d começa!", p_um);
    getchar();
    getchar();  //Dois getchar (?)
    primeiro = p_um;

    embaralha(&pilha, num);  //Função para embaralhar (de novo).

//14 peças são distribuídas para cada jogador----------------------------

    distribui(&pilha, num_jogadores, jogadores, p_um);

//Cada jogador deve jogar uma ou mais séries de no mínimo 30 pontos.-----
//Se não puder fazer isso, ele deve recolher uma pedra e passar a sua vez.

    for(i = 0; i < num_jogadores; i++) {
        cabecalho();
        imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
        printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
        printf("(1)Baixar pedras\n");
        printf("(2)Comprar pedra\n");
        scanf("%d", &opcao);

        valida = 0;

        switch(opcao) {
        case 1:
            opcao2 = 0;

            while(opcao2 == 0) {
                while(valida == 0) {
                    cabecalho();
                    imprime_idv(jogadores, p_um);
                    imprime_tab(tab);
                    setbuf(stdin, NULL);
                    printf("\n\n>>JOGADOR %d, escolha as pedras que deseja baixar:\n", p_um);

                    setbuf(stdin, NULL);
                    fgets(ins, 70, stdin);  //Preenche a string de instruções.

                    valida = primeira_jogada(&tab, jogadores, p_um, ins, 1, &total_series);
                    ptos[p_um - 1] = ptos[p_um - 1] + valida;

                    limpa_string(ins, 70);
                }

                valida = 0;
                imprime_tab(tab);

                printf("\n\n>>Deseja terminar a jogada?\n");
                printf("(1)Sim\n");
                printf("(2)Nao\n");
                scanf("%d", &opcao2);

                if(opcao2 == 1) {
                    opcao2 = 1;

                    if(ptos[p_um - 1] < 30) { //A primeira jogada tem que somar 30.
                        cabecalho();
                        printf("\n\n##A primeira jogada precisa somar 30 pontos. Continue jogando!\n");
                        setbuf(stdin, NULL);
                        getchar();
                        setbuf(stdin, NULL);

                        opcao2 = 0;
                    }
                }
                else {
                    opcao2 = 0;
                }

            }

            break;
        case 2: //Compra e passa a vez.
            novo = (PEDRA*)calloc(1, sizeof(PEDRA));

            if(novo == NULL) {
                cabecalho();
                printf("\n##ERRO DE ALOCACAO!\n");
                getchar();
                exit(1);
            }

            novo->numero = pilha->numero;
            novo->cor =  pilha->cor;
            novo->inteiro =	 pilha->inteiro;

            novo->prox = jogadores[p_um - 1];
            jogadores[p_um - 1] = novo;

            deleta_pilha(&pilha);

            break;
        default:  //Se digitou a opção errada.
            exit(1);

            break;
        }


        if(p_um < num_jogadores) {
            p_um++;
        }
        else {	//	 p_um >= num_jogadores
            p_um = 1;
        }
    }

//O jogo (loop)----------------------------------------------------------
    while(ganhou != 1) { //Enquanto não tiver ganhado!
        p_um = primeiro;

        for(i = 0; i < num_jogadores; i++) {
            cabecalho();
            imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
            imprime_tab(tab);
            printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
            printf("(1)Jogar\n");
            printf("(2)Comprar pedra\n");
            scanf("%d", &opcao);

            switch(opcao) {
            case 1:
				loop = 1;
				baixou = 0;	
				while(loop == 1){	
                    ptos[p_um - 1] = jogada(&tab, jogadores, p_um, &baixou, &total_series, &validou) + ptos[p_um - 1];  //JOGAR
					
					if(validou == 0){
						printf("\n\n>>Tabuleiro invalido! Continue manipulando...\n");
						getchar();
					}
					
					if((baixou != 0) && (validou != 0)){
					    printf("\n\n>>Deseja encerrar?\n");
					    printf("(0)Sim\n");
					    printf("(1)Nao\n");
					    setbuf(stdin, NULL);
					    scanf("%d", &loop);
					}
			    }

                break;

            case 2:
                novo = (PEDRA*)calloc(1, sizeof(PEDRA));

                if(novo == NULL) {
                    cabecalho();
                    printf("\n##ERRO DE ALOCACAO!\n");
                    getchar();
                    exit(1);
                }

                novo->numero = pilha->numero;
                novo->cor =  pilha->cor;
                novo->inteiro =	 pilha->inteiro;

                novo->prox = jogadores[p_um - 1];
                jogadores[p_um - 1] = novo;

                deleta_pilha(&pilha);
                break;

            default:
                exit(1);

                break;
            }

            if((jogadores[p_um - 1] == NULL) || (pilha == NULL)) {
                if(pilha == NULL) {
                    //Ganha aquele com mais pontos.

                    for(j = 0; j < num_jogadores; j++) {
                        for(k = 0; k < num_jogadores; k++) {
                            if(ptos[j] >= ptos[k]) { //Complexidade quadrática.
                                maior++;
                            }
                        }
                        if(maior == 4) {
                            p_um = j;
                        }
                    }
                }

                ganhou = 1;
                break;
            }
            if(p_um < num_jogadores) {
                p_um++;
            }
            else {
                p_um = 1;
            }
        }
    } 
//-----------------------------------------------------------------------
//Exibe mensagem de vitória (FIM).
    cabecalho();
    printf("\n\n>>O JOGADOR %d venceu!\n", p_um);
    setbuf(stdin, NULL);
    getchar();

//-----------------------------------------------------------------------
//Dá free nas listas.
    libera_jogadores(jogadores, num_jogadores);
	libera_pilha_tab(&tab);
	libera_pilha_tab(&pilha);
	free(jogadores);
	free(ptos);
	
    return 0;
}