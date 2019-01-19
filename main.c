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

int existe_tab(PEDRA** tab, char nro, char cr, int serie){  //Procura no tabuleiro
	PEDRA* percorre;
	int contador = 0;
	
	percorre = *tab;
    percorre = percorre->prox;
	
	while(contador < serie){
		percorre = percorre->prox;
		
		if(percorre->numero == 'X'){
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

int primeira_jogada(PEDRA** tab, PEDRA** jogadores, int p_um, char* ins) { //#####NOTA DO P.: Separar a valida em uma função!
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
        flag = existe(jogadores, p_um, ins[i], ins[i+1]);

        if(flag == 0) {
            cabecalho();
            printf("\n\n##A jogada não foi valida, pois voce nao tem essas pedras. Tente novamente!\n");
            getchar();
            setbuf(stdin, NULL);

            return 0;  //Se a jogada for inválida. (RETORNA ZERO)
        }

        i = i + 3;
    }

    //Valida a jogada.
    //-->Grupo:

    j = 0; 
    i = 0;
    while(ins[i] != '\0') { //Mesmo número, cores diferentes (min 3 / max 4).
        if((ins[j] != '*') && (flag2 == 'X')){
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

        j = j + 3;
        i = i + 3;
    }

    //Certifica a parte do "cores diferentes".
    if(grupo == 1) {
        for(i = 1; i < quantos; i = i + 3) {
            if(ins[i] != '*') { //Se for curinga, ignora!
                for(j = i + 3; j < quantos; j = j + 3) {
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

    if((grupo == 1) && ((quantos < 8) || (quantos > 11))) { //Limitação do número de pedras.
        cabecalho();
        printf("\n\n##Essa jogada não foi valida (lembre-se: no mínimo 3 pedras e no máximo 4). Tente novamente!\n");
        getchar();
        setbuf(stdin, NULL);

        return 0;  //Se a jogada for inválida. (RETORNA ZERO)
    }

    //-->Sequência:
    //Certifica se têm cores iguais
    i = 1;
    j = 1;
    flag2 = 'X';
    while(ins[i] != '\0') {
        if((ins[j] != '*') && (flag2 == 'X')){
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
        i = i + 3;
        j = j + 3;
    }

    //Certifica a parte da "sequência numérica":

    i = 0;
    if(seq == 1) {
        while(ins[i] != '\0') { //Muda o condicional!!
            if(ins[i] != '*') {
                if((strtol(&ins[i+3], NULL, 16) == strtol(&ins[i], NULL, 16) + 1) || (strtol(&ins[i+6], NULL, 16) == strtol(&ins[i], NULL, 16) + 2) || (ins[i+3] == '\0') || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-3], NULL, 16) + 1) || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-6], NULL, 16) + 2)) {
                    seq = 1;
                }
                else {
                    seq = 0;
                    break;
                }
            }
            i = i + 3;
        }
    }

    if((seq == 1) && (quantos < 8)) { //Limitação do número de pedras.
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

    //-->Verifica a soma dos pontos (30 pontos no máximo):
    i=0;
    while(ins[i] != '\0') {
        if(ins[i] != '*') {
            ptos = ptos + strtol(&ins[i], NULL, 16);
        }
        else {
            if(i == 0) {
                //Coringa no começo
                if(strtol(&ins[i+3], NULL, 16) == (strtol(&ins[i+6], NULL, 16) + 1)) { //Uma sequência.
                    ptos = ptos + strtol(&ins[i+3], NULL, 16) - 1;
                }
                if(strtol(&ins[i+3], NULL, 16) == strtol(&ins[i+6], NULL, 16)) { //Um grupo.
                    ptos = ptos + strtol(&ins[i+3], NULL, 16);
                }
            }
            if(i == (quantos - 2)) {	//Coringa no fim
                if(strtol(&ins[i-3], NULL, 16) == (strtol(&ins[i-6], NULL, 16) + 1)) { //Uma sequencia
                    ptos = ptos + strtol(&ins[i-3], NULL, 16) + 1;
                }

                if(strtol(&ins[i-3], NULL, 16) == strtol(&ins[i-6], NULL, 16)) { //Um grupo
                    ptos = ptos + strtol(&ins[i-3], NULL, 16);
                }
            }
            if((i != quantos) && (i != 0)) { //Coringa no meio
                if(strtol(&ins[i+3], NULL, 16) == (strtol(&ins[i-3], NULL, 16) + 2)) {
                    ptos = ptos + strtol(&ins[i-3], NULL, 16) + 1;
                }

                if(strtol(&ins[i-3], NULL, 16) == strtol(&ins[i+3], NULL, 16)) {
                    ptos = ptos + strtol(&ins[i-3], NULL, 16);
                }
            }
        }

        i = i + 3;
    }

    //-->Realiza a jogada (mexe nas listas)
    i = 0;
    while(ins[i] != '\0') {
        exclui_mao(ins[i], ins[i+1], jogadores, p_um);  //~FUNÇÃO PARA EXCLUIR DA MÃO.

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

        i = i + 3;
    }

    return ptos;
}

int verifica(PEDRA** tab, PEDRA** jogadores, int p_um, char* ins, int serie_dest){
	int quantos = 0;
	int i = 1;
    int j;
    int grupo = 0;
    int seq = 0;
    int vale;
	int flag;
    char flag2 = 'X';
    int serie_org;
    PEDRA* percorre;
	
    while(ins[quantos] != '\n') {
        quantos++;
    }
	
    while(ins[i] != '\0') {  //Verifica a existência
		if(ins[i-1] == 'M'){
            flag = existe(jogadores, p_um, ins[i], ins[i+1]);
		}
		if(ins[i-1] == 'T'){
            cabecalho();
            printf("\n\nDe qual serie do tabuleiro a pedra %c%c se origina?\n", ins[i], ins[i+1]);  
            setbuf(stdin, NULL);
            
            //##Testar se a serie de origem continua a ser uma serie. -->(RECURSÃO?)
            
            scanf("%d", &serie_org);
            
			flag = existe_tab(tab, ins[i], ins[i+1], serie_org);
		}
        if(ins[i-1] == 'X'){
            //Verificar se as pedras existem na serie especificada do tabuleiro
            percorre = *tab;
            
            flag = existe_tab(tab, ins[i], ins[i+1], serie_dest);
        }

        if(flag == 0) {
            cabecalho();
            printf("\n\n##A jogada não foi valida, pois essas pedras não estão disponíveis. Tente novamente!\n");
            setbuf(stdin, NULL);
            getchar();

            return 0;  //Se a jogada for inválida. (RETORNA ZERO)
        }

        i = i + 4;
    }
    
    //Verifica se é um grupo.
    
    j = 1; 
    i = 1;
    while(ins[i] != '\0') { //Mesmo número, cores diferentes (min 3 / max 4).
        if((ins[j] != '*') && (flag2 == 'X')){
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
        if((ins[j] != '*') && (flag2 == 'X')){
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
                if((strtol(&ins[i+4], NULL, 16) == strtol(&ins[i], NULL, 16) + 1) || (strtol(&ins[i+7], NULL, 16) == strtol(&ins[i], NULL, 16) + 2) || (ins[i+4] == '\0') || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-3], NULL, 16) + 1) || (strtol(&ins[i], NULL, 16) == strtol(&ins[i-6], NULL, 16) + 2)) {
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
        setbuf(stdin, NULL);
        getchar();

        return 0;  //Se a jogada for inválida. (RETORNA ZERO)
    }
    
	return 42;
}

void jogada(PEDRA** tab, PEDRA** jogadores, int p_um){
    int opcao;
	int flag = 0;
	int opcao2;
    int i;
	int serie_dest;  //Só importa na hora de conectar as series
	char ins[70];
	PEDRA* percorre;
	
	percorre = jogadores[p_um - 1];
	
    cabecalho();
    imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
	imprime_tab(*tab);
    printf("\n\n>>JOGADOR %d, escolha uma opcao:\n", p_um);
    printf("(1)Inserir pedras numa serie ja existente\n");
    printf("(2)Criar uma nova serie\n");
    scanf("%d", &opcao);	
	
	switch(opcao){
		case 1:
            while(flag == 0){
                for(i = 0; i < 70; i++){
                    ins[i] = '\0';    
                }
                
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
			
			//Verificação:
			
			    flag = verifica(tab, jogadores, p_um, ins, serie_dest);
                
            }
			
			//Alcançando a serie:
			
			//Manipulando as listas:
			
			break;
		case 2:
			cabecalho();
            imprime_idv(jogadores, p_um);  //Mostra a mão do jogador atual.
	        imprime_tab(*tab);
            printf("\n\n>>Quais pedras deseja baixar?\n");
            fgets(ins, 70, stdin);			
			
			//Verificação:
			
			//Manipulando as listas:
			
			break;
		default:
			exit(1);
			
			break;
	}
	
	return;
}

int main(void) {
//Declaração de variáveis------------------------------------------------
    int num_jogadores;  //Guarda o número de jogadores que estão participando.
    int i;
    int j;
    int valida;
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
    int* ptos;  //GUarda os pontos de cada jogador.
    int ganhou = 0;

//Atribuições e funções iniciais------------------------------------------------------------
    inicializa_pilha(&pilha);
    inicializa_pilha(&tab);

    for(i = 0; i < 70; i++) {
        ins[i] = '\0';
    }

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

                    valida = primeira_jogada(&tab, jogadores, p_um, ins);
                    ptos[p_um - 1] = ptos[p_um - 1] + valida;

                    for(j = 0; j < 70; j++) { //Limpa string.
                        ins[j] = '\0';
                    }
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
                jogada(&tab, jogadores, p_um);  //JOGAR

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

            //**NOTA DO P.: FUNÇÃO PARA DETECTAR VITÓRIA.

            if(p_um < num_jogadores) {
                p_um++;
            }
            else {
                p_um = 1;
            }
        }
    }
//-----------------------------------------------------------------------
//Dá free nas listas e exibe mensagem de vitória (FIM).

//-----------------------------------------------------------------------

    return 0;
}