#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------Estruturas----------------------------

typedef struct ponto{
    char valor[50];
    char pista[50];
    int cooldown;
    struct ponto* esquerda;
    struct ponto* direita;
}ponto;

#define tamanho 4

typedef struct hash{
    char nome[50];
    char pista[50];
    struct hash* proximo;
}hash;

//---------------Globais----------------------------

hash* tabela[tamanho];

char* pistaQuarto[2]={
    "Tapete bagunçado",
    "Traveseiros no chão"
};

char* pistaBiblioteca[1]={
    "Lixeira caída"
};

char* pistaSala[2]={
    "Ração pela casa",
    "Carpete sujo de terra"
};

//----------------Funções ponteiro---------------------------

ponto* CriarPonto(char* valor){
    ponto *novo = (ponto*) malloc(sizeof(ponto));
    if (novo == NULL){
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(novo->valor, valor);
    novo->pista[0] = '\0';
    novo->cooldown = 0;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

ponto* andarEsquerda(ponto* atual){
    if (atual->esquerda != NULL){
        atual = atual->esquerda;
        printf("\nVocê foi para: %s\n", atual->valor);
    } else {
        printf("\nNão há caminho à esquerda!\n");
    }
    return atual;
}

ponto* andarDireita(ponto* atual){
    if (atual->direita != NULL){
        atual = atual->direita;
        printf("\nVocê foi para: %s\n", atual->valor);
    } else {
        printf("\nNão há caminho à direita!\n");
    }
    return atual;
}

ponto* inserir(ponto* raiz, char* valor){
    if (raiz == NULL) 
        return CriarPonto(valor);
    
    if (strcmp(valor, raiz->valor) < 0)
        raiz->esquerda = inserir(raiz->esquerda, valor);
    else if (strcmp(valor, raiz->valor) > 0)
        raiz->direita = inserir(raiz->direita, valor);
        
    return raiz;
}

//--------------------Funções void---------------------------

void liberar(ponto* raiz){
    if (raiz != NULL){
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

void atribuirPista(ponto* local, char* pista){
    if(local != NULL && pista != NULL)
        strcpy(local->pista, pista);
    }
    
void atribuirNovaPista(ponto* local){
    if (strcmp(local->valor, "Quarto") == 0) {
        static int indiceQuarto = 0;
        if (indiceQuarto < 2) {
            strcpy(local->pista, pistaQuarto[indiceQuarto]);
            indiceQuarto++;
        } else local->pista[0] = '\0';
    } 
    else if (strcmp(local->valor, "Biblioteca") == 0) {
        static int indiceBiblioteca = 0;
        if (indiceBiblioteca < 1) {
            strcpy(local->pista, pistaBiblioteca[indiceBiblioteca]);
            indiceBiblioteca++;
        } else local->pista[0] = '\0';
    } 
    else if (strcmp(local->valor, "Sala de estar") == 0) {
        static int indiceSala = 0;
        if (indiceSala < 2) {
            strcpy(local->pista, pistaSala[indiceSala]);
            indiceSala++;
        } else local->pista[0] = '\0';
    }
}

void capturarPista(ponto** arvoreBST, ponto* local){
    if (local->pista[0] != '\0'){
        printf("--------------------------------------\n");
        printf("Pista [ %s ] encontrada.\n", local->pista);
        printf("--------------------------------------\n");
        *arvoreBST = inserir(*arvoreBST, local->pista);
        local->pista[0] = '\0';
        local->cooldown = 2;
        printf("Uma nova pista irá aparecer em certos lugares.\n");
        printf("--------------------------------------\n");
    } else {
        printf("--------------------------------------\n");
        printf("Não tem nenhuma pista aqui.\n");
        printf("--------------------------------------\n");
    }
}

void inOrder(ponto* raiz){
    if (raiz != NULL){
       inOrder(raiz->esquerda);
       printf("[ %s ]¬\n", raiz->valor);
       inOrder(raiz->direita);
    }
}

void atualizarPistas(ponto* raiz){
    if (raiz == NULL)
        return;

        atualizarPistas(raiz->esquerda);

        if (raiz->cooldown > 0){
        raiz->cooldown--;
        if (raiz->cooldown == 0 && raiz->pista[0] == '\0'){
            atribuirNovaPista(raiz);
        }
    }

    atualizarPistas(raiz->direita);
}

//------------------Funções Hash---------------------------

int funcao_hash(const char* chave){
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % tamanho;
}

void colocar(const char* nome, const char* pista){
    int indice = funcao_hash(nome);
    hash* novo = (hash*)malloc(sizeof(hash));
    strcpy(novo->nome, nome);
    strcpy(novo->pista, pista);
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

void preencherTabelaHash(){
    colocar("Gato", "Tapete bagunçado");
    colocar("Gato", "Cama com pelos");
    colocar("Gato", "Travesseiros no chão");
    colocar("Cachorro", "Ração pela casa");
    colocar("Cachorro", "Sofá com marcas de pegadas");
    colocar("Cachorro", "Carpete sujo de terra");
    colocar("Guaxinim", "Lixeira caída");
    colocar("Guaxinim", "Vaso de plantas quebrado");
    colocar("Guaxinim", "Livros rasurados");
}

int verificarPistasSuspeito(ponto* arvoreBST, const char* suspeito){
    int indice = funcao_hash(suspeito);
    int contador = 0;

    for (hash* atual = tabela[indice]; atual != NULL; atual = atual->proximo){
        ponto* cursor = arvoreBST;
        while (cursor != NULL){
            int cmp = strcmp(atual->pista, cursor->valor);
            if (cmp == 0){
                contador++;
                break;
            } else if (cmp < 0){
                cursor = cursor->esquerda;
            } else {
                cursor = cursor->direita;
            }
        }
    }
    return contador;
}

//--------------------MAIN---------------------------

int main()
{
    int opcao;
    int win=0;
    ponto *raiz = CriarPonto("Entrada");
    ponto *atual = raiz;
    ponto *arvoreBST = NULL;

    preencherTabelaHash();

    raiz->esquerda = CriarPonto("Biblioteca");
    atribuirPista(raiz->esquerda, "Livros rasurados");
    raiz->direita = CriarPonto("Sala de estar");
    atribuirPista(raiz->direita, "Sofá com marcas de pegadas");
    raiz->direita->esquerda = CriarPonto("Quarto");
    atribuirPista(raiz->direita->esquerda, "Cama com pelos");
    raiz->direita->direita = CriarPonto("Varanda");
    atribuirPista(raiz->direita->direita, "Vaso de plantas quebrado");

    printf("\n\tINICIANDO JOGO...\n");


    do{
        if(win==3)
            printf("VOCÊ GANHOU O JOGO!! PARABÉNS POR ENCONTRAR TODOS OS CULPADOS");

        printf("\n_____________MAPA_______________\n");
        printf("~~ Raiz: %s\n", raiz->valor);
        printf(">> Esquerda: %s\n", raiz->esquerda->valor);
        printf(">> Direita: %s\n", raiz->direita->valor);
        printf(">> Direita >> Esquerda: %s\n", raiz->direita->esquerda->valor);
        printf(">> Direita >> Direita: %s\n", raiz->direita->direita->valor);
        printf("__________________________________\n");

        printf("\n| Você está em: %s\n", atual->valor);
        printf("| Tempo para nova pista: %d\n", atual->cooldown);
        printf("__________________________________\n");
        printf("1. Ir para esquerda.\n");
        printf("2. Ir para direita.\n");
        printf("3. Voltar para o início.\n");
        printf("4. Procurar pista.\n");
        printf("5. Visualizar pistas.\n");
        printf("6. Apontar suspeito.\n");
        printf("0. Sair.\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1: atual = andarEsquerda(atual); atualizarPistas(atual); break;
            case 2: atual = andarDireita(atual); atualizarPistas(atual); break;
            case 3: atual = raiz; printf("\nVocê voltou para o início.\n"); break;
            case 4: capturarPista(&arvoreBST, atual); break;
            case 5:{
                printf("Suas pistas:\n");
                if(arvoreBST != NULL) inOrder(arvoreBST);
                else printf("Vazio.\n");
                break;
            }
            case 6:{
                int esc;
                printf("\n1. Gato.\n2. Cachorro.\n3. Guaxinim.\n");
                printf("Quem foi que fez a bagunça? ");
                scanf("%d", &esc);

                const char* suspeito = NULL;
                if (esc == 1) suspeito = "Gato";
                else if (esc == 2) suspeito = "Cachorro";
                else if (esc == 3) suspeito = "Guaxinim";
                else { printf("Opção inválida.\n"); break; }

                int n = verificarPistasSuspeito(arvoreBST, suspeito);
                if (n >= 2){
                    printf("\nVocê acertou! O %s fez a bagunça! (%d pistas batem)\n", suspeito, n);
                    win++;
                }    
                else
                    printf("\nAinda faltam pistas que comprovem o %s. (%d pista(s) batem)\n", suspeito, n);
                break;
            }
            case 0: printf("\nSaindo do jogo...\n"); break;
            default: printf("Opção inválida.\n"); break;
        }
    } while (opcao != 0);

    liberar(raiz);
    free(arvoreBST);
    return 0;
}