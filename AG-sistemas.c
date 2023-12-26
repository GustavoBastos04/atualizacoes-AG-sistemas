#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_ATUALIZACOES 100
#define MAX_FILENAME 256
// Struct para operacoes novas
typedef struct estr{
    char operacao;
    int instante;
    int id;
    int tempo;
} MUDANCA;

typedef struct aux{
  int duracao;
  int id;
} ATUALIZACAO;

/*
==============================================================================================================
    BUBBLE SORT PARA CENÁRIO 1
==============================================================================================================
*/
void bubbleSort (ATUALIZACAO vetor[], int n) {
    int k, j;
    ATUALIZACAO aux;
    for (k=1; k<n; k++) {
      for (j=0; j<n-1; j++) {
        if (vetor[j].duracao > vetor[j+1].duracao) {
          aux = vetor[j];
          vetor[j] = vetor[j+1];
          vetor[j+1] = aux;
        }
      }
    }
}
/*
==============================================================================================================
    HEAP PARA CENÁRIO 2
==============================================================================================================
*/


typedef struct Heap {
    int *valores;
    int tamanho;
    int capacidade;
} heap;

void refazHeap(heap* h, int indice)
{
    int pai = (indice - 1)/2;
 
    if (h->valores[pai] > h->valores[indice]) {
        int aux = h->valores[pai];
        h->valores[pai] = h->valores[indice];
        h->valores[indice] = aux;
        refazHeap(h, pai);
    }
}

void heapify(heap* h, int indice)
{
    int esquerda = indice * 2 + 1;
    int direita = indice * 2 + 2;
    int min = indice;

    if (esquerda >= h->tamanho || esquerda < 0) esquerda = -1;
    if (direita >= h->tamanho || direita < 0) direita = -1;
    
    if (esquerda != -1 && h->valores[esquerda] < h->valores[indice]) min = esquerda;
    if (direita != -1 && h->valores[direita] < h->valores[min]) min = direita;
    
    if (min != indice){
        int aux = h->valores[min];
        h->valores[min] = h->valores[indice];
        h->valores[indice] = aux;
        heapify(h, min);
    }
}

heap* Heap(int capacidade, ATUALIZACAO* atualizacao, int quantAtualiz){
    heap* h = (heap*)malloc(sizeof(heap));
    h->tamanho = 0;
    h->capacidade = capacidade;
    h->valores = (int*) malloc(sizeof(int)*capacidade);
    int i;
    for (i = 0; i < quantAtualiz; i++) {
      h->valores[i] = atualizacao[i].duracao;
    }
    h->tamanho = i;
    i = (h->tamanho - 2)/2;
    while (i >= 0) {
        heapify(h, i);
        i--;
    }
    return h;
}
 
int extrairMinHeap(heap* h)
{
    int deletado;
    deletado = h->valores[0];
    h->valores[0] = h->valores[h->tamanho-1];
    h->tamanho--;
    heapify(h, 0);
    return deletado;
}
 
void insercaoHeap(heap* h, int valor){
    if (h->tamanho < h->capacidade) {
        h->valores[h->tamanho] = valor;
        refazHeap(h, h->tamanho);
        h->tamanho++;
    }
}
/*
==============================================================================================================
    CENÁRIO 1
==============================================================================================================
*/

void atualizacoesCenario1(int quantAtualiz, int horas, ATUALIZACAO atualizacao[]){
  int atualizacoesTotais = 0;
  for(int i = 0; i<quantAtualiz; i++){
    horas -= atualizacao[i].duracao;
    if(horas<0) break;
    atualizacoesTotais++;
  }
  printf("%d ", atualizacoesTotais);
  for(int j = 0; j< atualizacoesTotais; j++){
    printf("%d ", atualizacao[j].id);
  }
}

/*
==============================================================================================================
    CENÁRIO 2
==============================================================================================================
*/

int encontrarID(ATUALIZACAO* atualiz, int quantAtualiz, int tempo){
  for(int i = 0; i<quantAtualiz;i++){
    if (atualiz[i].duracao == tempo){
      atualiz[i].duracao = INT_MAX;
      return atualiz[i].id;
    }
  }
}

void atualizacoesCenario2(heap* h,int quantAtualiz, int horas, ATUALIZACAO* atualizacao, MUDANCA* mudancas){
  int atualizacoesTotais = 0;
  int ordemInstante = 0;
  int escolhidos[MAX_ATUALIZACOES];
  int tempoAcumulado = 0;
  int tempoAcumuladoGlobal = 0;
  for(int x = 0; x<quantAtualiz; x++){
  
  int prioridade = extrairMinHeap(h);

  tempoAcumuladoGlobal += prioridade;

  for(int t = tempoAcumulado; t<tempoAcumuladoGlobal; t++){    
    if((t+1) == mudancas[ordemInstante].instante){
      if(mudancas[ordemInstante].operacao == 'c'){
        for(int i = 0; i<quantAtualiz; i++){
          if(atualizacao[i].id == mudancas[ordemInstante].id){
            for(int z  = 0; z<quantAtualiz; z++){
              if(atualizacao[i].duracao == h->valores[z]){
                atualizacao[i].duracao = mudancas[ordemInstante].tempo;
                h->valores[z] = atualizacao[i].duracao;
                refazHeap(h, z); 
              }
            }
          }   
        }
      }
      if(mudancas[ordemInstante].operacao == 'i'){
        insercaoHeap(h, mudancas[ordemInstante].tempo);
        quantAtualiz++;
        atualizacao[quantAtualiz-1].duracao = mudancas[ordemInstante].tempo;
        atualizacao[quantAtualiz-1].id = mudancas[ordemInstante].id;
      }
      ordemInstante++;
    }
  }  

    horas -= prioridade;
    if(horas<0) break;
    escolhidos[x] = encontrarID(atualizacao, quantAtualiz , prioridade);
    atualizacoesTotais++;
    tempoAcumulado += prioridade;
  }  
  
  printf("%d ", atualizacoesTotais);
  for(int j = 0; j<atualizacoesTotais; j++){
    printf("%d ", escolhidos[j]);
  }
}

/*
==============================================================================================================
    MAIN
==============================================================================================================
*/

int main(int argc, char* argv[]){
    
    if (argc != 4)
    {
        printf("Este programa três exige argumentos\n"); 
        exit(-1); 
    }

    //periodo lido de 14 a 20 horas
    char periodoChar[3];
    strcpy(periodoChar, argv[1]);
    int periodo = atoi(periodoChar);
    if(periodo >20|| periodo <14){
      printf("Aceito apenas entre 14 e 20 horas\n");
      exit(-1);
    }

    //Cenário escolhido
    char cenarioChar[2];
    strcpy(cenarioChar, argv[2]);
    int cenario = atoi(cenarioChar);
    if(cenario != 1 && cenario != 2) exit(-1);
    
    //Guarda a quantidade de atualizações e uma struct para o id das atualizacoes e duracao
    int quantAtualizacoes;
    ATUALIZACAO atualizacao[MAX_ATUALIZACOES];

    //structs que indicam se há alteração ou inserção
    MUDANCA operacoes[MAX_ATUALIZACOES];
    int quantAlteracoes = 0;
    //Heap
    heap* h; 

    //Ler arquivo de entrada
    char filename[MAX_FILENAME] = "";
    strcpy(filename, argv[3]);  
    FILE* fp = fopen(filename, "r"); 
    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
        return -1;
    }

    //Ler abaixo os arquivos de entrada
    fscanf(fp, "%d", &quantAtualizacoes);
    for(int i = 0; i < quantAtualizacoes; i++){
      fscanf(fp, "%d", &atualizacao[i].duracao);
      atualizacao[i].id = i+1;
    }

    if(cenario == 2){
      int y = 0;
      while(!feof(fp)){
        fscanf(fp,"\n%c", &operacoes[y].operacao);
        fscanf(fp,"%d", &operacoes[y].instante);
        fscanf(fp,"%d", &operacoes[y].id);
        fscanf(fp,"%d\n", &operacoes[y].tempo);
        y++;
        quantAlteracoes++;
      }
    }
    fclose(fp);

    // Ordenar as atualizações
    if(cenario == 1){
      bubbleSort(atualizacao,quantAtualizacoes);
    }
    // Heap
    if(cenario == 2){
      h = Heap(MAX_ATUALIZACOES, atualizacao, quantAtualizacoes);
    }

    //Criando arquivo.txt de saida
    
    FILE* output;
    if(cenario == 1 ) output = fopen("saida1.txt","w");
    if(cenario == 2 ) output = fopen("saida2.txt","w");

    if (output == NULL)
    {
        fprintf(stderr, "Erro com o arquivo de saida\n");
        return -1;
    }
    int fd=fileno(output), copy=dup( STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);

    //Escrever função que imprime as saídas
    if(cenario == 1){
      atualizacoesCenario1(quantAtualizacoes, periodo, atualizacao);
      printf("\n");
    }
    if(cenario == 2){
      atualizacoesCenario2(h,quantAtualizacoes, periodo, atualizacao, operacoes); 
      printf("\n");
    }

    fflush(stdout);
        fclose(output); 
    dup2(copy, STDOUT_FILENO);   

    //Escrever função que imprime as saídas
    if(cenario == 1){
      printf("Execução concluida! Verifique o arquivo saida1.txt para ver os resultados!\n");
    }
    if(cenario == 2){
      printf("Execução concluida! Verifique o arquivo saida2.txt para ver os resultados!\n");
    }
}
