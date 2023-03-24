#include "Controle_leitura.h"

void menu (void) {
    LimparTela_Pausar(1);
    //int escolha = ColetarApenasInteiros("[0] Adicionar meta\n[1] Mostrar historico de metas \n ");

    switch(ColetarApenasInteiros("---------------------------------\n|\tEscolha uma op��o      \t|\n---------------------------------\n[0] Adicionar meta \t\t|\n[1] Alterar meta\t\t|\n[2] Excluir meta\t\t|\n[3] Printar meta\t\t|\n[4] Mostrar historico de metas  |\n---------------------------------\n")) {
        case 0:
            LimparTela_Pausar(1);
            Adicionar_meta();
            break;
        case 1:
            LimparTela_Pausar(1);
            Alterar_meta();
            break;

        case 2:
            LimparTela_Pausar(1);
            Excluir_meta();
            break;

        case 3:
            LimparTela_Pausar(1);
            Mostrar_meta();
            break;

        case 4:
            LimparTela_Pausar(1);
            Mostrar_historico();
            break;
        default:
            exit(0);
    }
    menu();
}

void Adicionar_meta(void) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt","a+b");
    verificar_ErroAbrir(arquivo);

    int tamanho_arquivo = tamanho(arquivo);

    printf("\tDigite os dados a seguir:\n\n");
    meta = Coletar_Dados();
    meta.ID = tamanho_arquivo + 1;

    fwrite(&meta, sizeof(METAS), 1, arquivo);
    fclose(arquivo);

    printar_meta(tamanho_arquivo);

    LimparTela_Pausar(2);
    return;
}

METAS Coletar_Dados(void) {
    METAS meta;
    //Dados inseridos pelo usuario

    printf("Digite o nome do livro: ");
        fgets(meta.livro.nome, 50, stdin);

    meta.livro.PaginaInicial = ColetarApenasInteiros("Digite o n�mero da p�gina inicial: ");
    meta.livro.PaginaFinal = ColetarApenasInteiros("Digite o n�mero da p�gina final: ");
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

void printar_meta(int ir) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt","rb");
    verificar_ErroAbrir(arquivo);

    fseek(arquivo, sizeof(METAS) * ir, SEEK_SET);
    fread(&meta, sizeof(METAS), 1, arquivo);

    float pagina_atual = meta.livro.PaginaInicial;

    printf("\nVoc� deve se encontrar em tal pagina a cada dia para completar a meta: \n");
    for (int por_Dia = 0; por_Dia < meta.QuantidadeDias; por_Dia++) {
        pagina_atual += meta.Leitura_porDia;
        printf("| %2d� dia: %02.2f\t", por_Dia + 1, pagina_atual);
        if (por_Dia % 2 != 0){
            printf("\n");
        }
    }

    printf("\nA meta � ler %sQue come�a na p�gina %d e termina na p�gina %d\n", meta.livro.nome, meta.livro.PaginaInicial, meta.livro.PaginaFinal);
    printf("Lendo %.2f p�ginas por dia durante %d dias\n", meta.Leitura_porDia, meta.QuantidadeDias);
    printf("Dia de modifica��o: %02d/%02d/%4d", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano);

    return;
}

void Mostrar_historico(void) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    verificar_ErroAbrir(arquivo);

    int tamanho_arquivo = tamanho(arquivo);

    if (tamanho_arquivo == 0) {
        printf("N�o existe historico para mostrar\n");
        fclose(arquivo);
        LimparTela_Pausar(2);
        return;
    }

    rewind(arquivo);

    for (int i = 0; i < tamanho_arquivo; i++) {
        fread(&meta, sizeof(METAS), 1, arquivo);
        printf("\nID: %03d\nNome: %sN�mero da p�gina inicial: %3d\nN�mero da p�gina final: %3d", meta.ID ,meta.livro.nome, meta.livro.PaginaInicial, meta.livro.PaginaFinal);
        printf("\nLeitura por dia: %3.2f\nQuantidade de dias: %3d", meta.Leitura_porDia, meta.QuantidadeDias);
        printf("\nData de modifica��o: %02d/%02d/%4d\n", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano );
    }

    fclose(arquivo);
    LimparTela_Pausar(2);
    return;
}

void Mostrar_meta() {
    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    verificar_ErroAbrir(arquivo);

    int escolha =  escolher_meta(arquivo);


    printar_meta(escolha - 1);

    LimparTela_Pausar(2);
    fclose(arquivo);
    return;
}

void Alterar_meta() {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "r+b");
    verificar_ErroAbrir(arquivo);

    int escolha =  escolher_meta(arquivo);
    printar_meta(escolha - 1);

    meta = Coletar_Dados();

    fseek(arquivo, sizeof(METAS) * (escolha - 1), SEEK_SET);
    meta.ID = (ftell(arquivo) / sizeof(METAS)) + 1;

    fwrite(&meta, sizeof(METAS), 1, arquivo);
        fclose(arquivo);

    printar_meta(escolha - 1);

    LimparTela_Pausar(2);

    return;
}

void Excluir_meta() {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "r+b");
    verificar_ErroAbrir(arquivo);

    int pular = escolher_meta(arquivo);

    int tamanho_arquivo = tamanho(arquivo);

    //!feof(arquivo)
    for (int i = pular; i < tamanho_arquivo ; i++) {
        fseek(arquivo, sizeof(METAS) * pular, SEEK_SET);
        fread(&meta, sizeof(METAS), 1, arquivo);
        fseek(arquivo, sizeof(METAS) * (pular - 1), SEEK_SET);
        meta.ID = (ftell(arquivo) / sizeof(METAS)) + 1;
        fwrite(&meta, sizeof(METAS) , 1, arquivo);
        pular++;
    }

    METAS *apagar = calloc(1, 0);
    fseek(arquivo, sizeof(METAS) * -1, SEEK_END);
    fwrite(apagar, 0, 1, arquivo);

    free(apagar);
    fclose(arquivo);
    return;
}

