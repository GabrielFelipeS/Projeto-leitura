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

//Funções casuais - Encontradas do Controle_leitura
void menu(void);
void Adicionar_meta(void);
METAS Coletar_Dados(void);
void Mostrar_historico(void);
void printar_meta(int ir);
void Mostrar_meta(void);
void Alterar_meta(void);
void Excluir_meta(void);

// Funções de controle de dados - Econtradas na main para facilitar a organização
int tamanho (FILE *arquivo);
void verificar_ErroAbrir (FILE *arquivo);
int escolher_meta(FILE *arquivo);
int ColetarApenasInteiros(char mensagem[]);
void LimparTela_Pausar(unsigned short int escolha);








#endif // LEITURA_CONTROLE

