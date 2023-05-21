#include "Controle_leitura.h"

int main()
{
    setlocale(LC_ALL, "");
    menu();
    LimparTela_Pausar(2);
    return 0;
}

int tamanho(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);

    int tamanho = ftell(arquivo) / sizeof(METAS);
    return tamanho;
}

void verificar_ErroAbrir (FILE *arquivo) {
    if (arquivo == NULL) {
        printf("ERRO!\nNa abertura do arquvio.");
        LimparTela_Pausar(2);
        exit(1);
    }

    return;
}

int escolher_meta(FILE *arquivo) {
    METAS meta;
    int tamanho_arquivo = tamanho(arquivo);
    if (tamanho(arquivo) == 0) {
        printf("Não existe histórico para printar\n");
        fclose(arquivo);
        return;
    }

    rewind(arquivo);

    printf("[id] nome do livro\n");
    for (int i = 0; i < tamanho_arquivo; i++) {
            fread(&meta, sizeof(METAS), 1, arquivo);
            printf("[%2i] %s", meta.ID, meta.livro.nome);
    }
    int escolha = ColetarApenasInteiros("\nEscolha um ID: \n");
    escolha = (escolha <= 0 || escolha > tamanho_arquivo + 1)? 1: escolha;
    return escolha;
}

int ColetarApenasInteiros(char mensagem[]) {
    bool loop;
    char string[11];

    do {
        loop = false;
        printf("%s", mensagem);
            fgets(string, 11, stdin);
        int tamanho = strlen(string);
        for ( int i = 0; i < tamanho; i++) {
            if (isalpha(string[i])) {
                loop = true;
                printf("ERRO!\n");
                break;
            }
        }

        fflush(stdin);
    } while(loop);

    return atoi(string);
}

void LimparTela_Pausar(unsigned short int escolha) {
#ifdef OS_WINDOWS
    if(escolha == 1) {
        system("cls");

    } else if (escolha == 2) {
        printf("\n\n");
        system("pause");
    }
#else
    if(escolha == 1) {
        system("clear");

    } else if (escolha == 2) {
        printf("\n\n");
        getchar();
    }

#endif // OS_WINDOWS


    return;
}

