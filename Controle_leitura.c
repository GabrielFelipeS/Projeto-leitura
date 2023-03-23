#include "Controle_leitura.h"

void menu (void) {
    LimparTela_Pausar(1);
    //int escolha = ColetarApenasInteiros("[0] Adicionar meta\n[1] Mostrar historico de metas \n ");

    switch(ColetarApenasInteiros("---------------------------------\n|\tEscolha uma opção      \t|\n---------------------------------\n[0] Adicionar meta \t\t|\n[1] Mostrar historico de metas  |\n---------------------------------\n")) {
        case 0:
            LimparTela_Pausar(1);
            Adicionar_meta();
            break;
        case 1:
            LimparTela_Pausar(1);
            Mostrar_historico();
            break;

        default:
            exit(0);
    }
    menu();
}

void Adicionar_meta(void) {
    FILE *arquivo = fopen("dados_leitura.txt","ab");
    METAS meta;
      if (arquivo == NULL) {
        printf("Erro na abertura!!\n");
        LimparTela_Pausar(2);
        exit(1);
    }

    printf("\tDigite os dados a seguir:\n\n");
    meta = Coletar_Dados();

    fwrite(&meta, sizeof(METAS), 1, arquivo);
    fclose(arquivo);

    printar_meta();



    LimparTela_Pausar(2);
    return;
}

METAS Coletar_Dados(void) {
    METAS meta;
    //Dados inseridos pelo usuario
    FILE *arquivo = fopen("dados_leitura.txt","rb");
    if (arquivo == NULL) {
        printf("ERRO!\nNa abertura do arquvio.");
        LimparTela_Pausar(2);
        exit(1);
    }

    int tamanho_arquivo = tamanho(arquivo);
    fclose(arquivo);


    meta.ID = tamanho_arquivo + 1;
    printf("Digite o nome do livro: ");
        fgets(meta.livro.nome, 50, stdin);

    meta.livro.PaginaInicial = ColetarApenasInteiros("Digite o número da página inicial: ");
    meta.livro.PaginaFinal = ColetarApenasInteiros("Digite o número da página final: ");
    meta.QuantidadeDias = ColetarApenasInteiros("Digite em quantos dias gostaria de ler o livro: ");

    //Calculo de paginas baseado no valor entregue pelo usuario
    meta.Leitura_porDia = (meta.livro.PaginaFinal - meta.livro.PaginaInicial) / (float) meta.QuantidadeDias;

    //Dados de tempo coletado pelo sistema
    time_t sistema_data;
    time(&sistema_data);
    struct tm *dados_sistema = localtime(&sistema_data);

    meta.modificacao.dia = dados_sistema->tm_mday;
    meta.modificacao.mes = dados_sistema->tm_mon + 1;
    meta.modificacao.ano = dados_sistema->tm_year + 1900;

    return meta;
}

void printar_meta(void) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt","rb");
    if (arquivo == NULL) {
        printf("ERRO!\nNa abertura do arquvio.");
        LimparTela_Pausar(2);
        exit(1);
    }

    fseek(arquivo, sizeof(METAS) * -1, SEEK_END);
    fread(&meta, sizeof(METAS), 1, arquivo);

    float pagina_atual = meta.livro.PaginaInicial;

    printf("\nVocê deve se encontrar em tal pagina a cada dia para completar a meta: \n");
    for (int por_Dia = 0; por_Dia < meta.QuantidadeDias; por_Dia++) {
        pagina_atual += meta.Leitura_porDia;
        printf("| %2dº dia: %02.2f\t", por_Dia + 1, pagina_atual);
        if (por_Dia % 2 != 0){
            printf("\n");
        }
    }

    printf("\nA meta é ler %sQue começa na página %d e termina na página %d\n", meta.livro.nome, meta.livro.PaginaInicial, meta.livro.PaginaFinal);
    printf("Lendo %.2f páginas por dia durante %d dias\n", meta.Leitura_porDia, meta.QuantidadeDias);
    printf("Dia de modificação: %02d/%02d/%4d", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano);

    return;
}

void Mostrar_historico(void) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    if (arquivo == NULL) {
        printf("ERRO!\nNa abertura do arquvio.");
        LimparTela_Pausar(2);
        exit(1);
    }

    int tamanho_arquivo = tamanho(arquivo);

    if (tamanho_arquivo == 0) {
        printf("Não existe historico para mostrar\n");
        fclose(arquivo);
        LimparTela_Pausar(2);
        return;
    }

    rewind(arquivo);

    for (int i = 0; i < tamanho_arquivo; i++) {
        fread(&meta, sizeof(METAS), 1, arquivo);
        printf("\nID: %03d\nNome: %sNúmero da página inicial: %3d\nNúmero da página final: %3d", meta.ID ,meta.livro.nome, meta.livro.PaginaInicial, meta.livro.PaginaFinal);
        printf("\nLeitura por dia: %3.2f\nQuantidade de dias: %3d", meta.Leitura_porDia, meta.QuantidadeDias);
        printf("\nData de modificação: %02d/%02d/%4d", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano );
    }

    fclose(arquivo);
    LimparTela_Pausar(2);
    return;
}

int tamanho(FILE *arquivo) {
        if (arquivo == NULL) {
        printf("Erro na abertura!!\n");
        LimparTela_Pausar(2);
        exit(1);
    }
    fseek(arquivo, 0, SEEK_END);

    int tamanho = ftell(arquivo) / sizeof(METAS);
    return tamanho;
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
