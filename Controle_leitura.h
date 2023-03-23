#ifndef LEITURA_CONTROLE
    #define LEITURA_CONTROLE
    #ifdef __unix__

    #elif defined( _WIN32) || defined(WIN32)
        #define OS_WINDOWS

    #endif

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

//Estruturas
typedef struct {
    char nome[50];
    unsigned short int PaginaInicial;
    unsigned short int PaginaFinal;
} LIVROS;

typedef struct {
    unsigned short int dia;
    unsigned short int mes;
    unsigned short int ano;
} DATAS;

typedef struct {
    unsigned short int ID;
    unsigned short int QuantidadeDias;
    float Leitura_porDia;
    LIVROS livro;
    DATAS modificacao;
} METAS;

//Funções casuais
void menu(void);
void Adicionar_meta(void);
METAS Coletar_Dados(void);
void Mostrar_historico(void);
void printar_meta(void);

// funções de controle de dados
int tamanho (FILE *arquivo);
int ColetarApenasInteiros(char mensagem[]);
void LimparTela_Pausar(unsigned short int escolha);









#endif // LEITURA_CONTROLE

