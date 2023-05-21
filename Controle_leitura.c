#include "Controle_leitura.h"

void menu (void) {
    LimparTela_Pausar(1);
    //int escolha = ColetarApenasInteiros("[0] Adicionar meta\n[1] Mostrar historico de metas \n ");

    switch(ColetarApenasInteiros("---------------------------------\n|\tEscolha uma opção      \t|\n---------------------------------\n[0] Adicionar meta \t\t|\n[1] Alterar meta\t\t|\n[2] Excluir meta\t\t|\n[3] Printar meta\t\t|\n[4] Mostrar histórico de metas  |\n[5] Outro numero para finalziar |\n---------------------------------\n")) {
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

void printar_meta(int ir) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt","rb");
    verificar_ErroAbrir(arquivo);
    if (tamanho(arquivo) == 0) {
        fclose(arquivo);
        return;
    }

    fseek(arquivo, sizeof(METAS) * ir, SEEK_SET);
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
    printf("Dia de modificação: %02d/%02d/%4d\n", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano);

    return;
}

void Mostrar_historico(void) {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    verificar_ErroAbrir(arquivo);

    int tamanho_arquivo = tamanho(arquivo);

    if (tamanho_arquivo == 0) {
        printf("Não existe histórico para mostrar\n");
        fclose(arquivo);
        LimparTela_Pausar(2);
        return;
    }

    rewind(arquivo);

    for (int i = 0; i < tamanho_arquivo; i++) {
        fread(&meta, sizeof(METAS), 1, arquivo);
        printf("\nID: %03d\nNome: %sNúmero da página inicial: %2d\nNúmero da página final: %3d", meta.ID ,meta.livro.nome, meta.livro.PaginaInicial, meta.livro.PaginaFinal);
        printf("\nLeitura por dia: %3.2f\nQuantidade de dias: %3d", meta.Leitura_porDia, meta.QuantidadeDias);
        printf("\nData de modificação: %02d/%02d/%4d\n", meta.modificacao.dia, meta.modificacao.mes, meta.modificacao.ano );
    }

    fclose(arquivo);
    LimparTela_Pausar(2);
    return;
}

void Mostrar_meta() {
    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    verificar_ErroAbrir(arquivo);

    int escolha =  escolher_meta(arquivo);
    if (tamanho(arquivo) == 0) {
        fclose(arquivo);
        return;
    }

    printar_meta(escolha - 1);

    LimparTela_Pausar(2);

    return;
}

void Alterar_meta() {
    METAS meta;
    FILE *arquivo = fopen("dados_leitura.txt", "r+b");
    verificar_ErroAbrir(arquivo);
    if (tamanho(arquivo) == 0) {
        printf("Não existe histórico para mostrar\n");
        fclose(arquivo);
        LimparTela_Pausar(2);
        return;
    }

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

    FILE *arquivo = fopen("dados_leitura.txt", "rb");
    verificar_ErroAbrir(arquivo);

    int tamanho_arquivo = tamanho(arquivo);
    if (tamanho_arquivo == 0) {
        printf("Não existe histórico para excluir\n");
        fclose(arquivo);
        LimparTela_Pausar(2);
        return;
    }

    int indice = 0;
    int pular = escolher_meta(arquivo);

    METAS meta[tamanho_arquivo - 1];

    for (int i = 1; i <= tamanho_arquivo + 1 ; i++) {
       if (i != pular){
            fseek(arquivo, sizeof(METAS) * i,SEEK_SET);
            fread(&meta[indice], sizeof(METAS), 1, arquivo);
            meta[indice].ID = indice + 1;
            indice++;
       }
    }
    LimparTela_Pausar(2);
    fclose(arquivo);

    arquivo = fopen("dados_leitura.txt", "wb");
    fwrite(meta, sizeof(METAS), tamanho_arquivo - 1, arquivo);
    fclose(arquivo);
    return;
}

