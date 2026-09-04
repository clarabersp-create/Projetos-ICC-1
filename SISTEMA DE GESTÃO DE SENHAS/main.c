#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLOG 80
#define MAXSENHA 30
#define MAXINFO 100
#define MAXN 5

FILE *fileptr;

//struct com infos login
typedef struct
{
    char login[MAXLOG];
    char senha[MAXSENHA];
    char info[MAXINFO];
    char atividade;
}cadastro;

cadastro *tds_cadastros = NULL;

//converte string toda para letras minusculas
void para_minusculo(char str[])
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

//criptografar
void criptografar(char senha_mestra[], char str[])
{
                int aux, size;
                size = strlen(str);
                for(int j = 0; j < size; j++)
                {
                                aux = senha_mestra[ j % strlen(senha_mestra)] - '0';
                                str[j] += aux;
                }
}

//descriptografar
void descriptografar(char senha_mestra[], char str[])
{
                int aux, size;
                size = strlen(str);
                for(int j = 0; j < size; j++)
                {
                                aux = senha_mestra[ j % strlen(senha_mestra)] - '0';
                                str[j] -= aux;
                }
}

//le opção de rotina a ser executada
int le_opcao()
{
    char opcao_str[MAXN];
    int opcao;

    fgets(opcao_str, MAXN, stdin);
    opcao = atoi(opcao_str);

    return opcao;
}

//subrotina opcao 0
void opcao_zero(char senha_mestra[])
{
                fgets(senha_mestra, 11, stdin);
                senha_mestra[strcspn(senha_mestra, "\r\n")] = '\0';
                printf("EXEC OPC0\n");
}

//subrotina opcao 1
int opcao_um()
{
                int total_cadastros;

                fileptr = fopen("senhas-salvas.bin", "rb");
                if(fileptr == NULL)
                {
                                printf("ERRO ARQ\n");
                                return -1;
                }
                //ler qntd de cadastros do arquivo
                fread(&total_cadastros, sizeof(int), 1, fileptr);

                //alocar espaço para N + 30 cadastros
                tds_cadastros = (cadastro*) calloc(total_cadastros + 30, sizeof(cadastro));

                //lendo os N cadastros do arquivo .bin
                fread(tds_cadastros, sizeof(cadastro), total_cadastros, fileptr);

                fclose(fileptr);
                printf("EXEC OPC1\n");

                return total_cadastros;
}

//subrotina opcao 2
void opcao_dois(int capacidade)
{
                int cadastros_ativos = 0;

                fileptr = fopen("senhas.bin", "wb");
                if(fileptr == NULL)
                {
                                printf("ERRO ARQ\n");
                                return;
                }
                //contando qnts cadastros ativos tem e registrando no arquivo senhas.bin
                for(int i = 0; i < capacidade; i++)
                {
                                if(tds_cadastros[i].atividade == 'A')
                                {
                                                cadastros_ativos++;
                                }
                }
                fwrite(&cadastros_ativos, sizeof(int), 1, fileptr);
                //registrando cadastros ativos
                for(int i = 0; i < capacidade; i++)
                {
                                 if(tds_cadastros[i].atividade == 'A')
                                 {
                                                 fwrite(&tds_cadastros[i], sizeof(cadastro), 1, fileptr);
                                 }
                }
                fclose(fileptr);
                printf("EXEC OPC2\n");
}

//subrotina opcao 3
void opcao_tres(int capacidade, char senha_mestra[])
{
                int cadastros_ativos = 0;
                char senha_descripto[MAXSENHA + 1];

                fileptr = fopen("senhas-bkp.txt", "wt");
                if(fileptr == NULL)
                {
                                printf("ERRO ARQ\n");
                                return;
                }

                //contando qnts cadastros ativos tem e registrando no arquivo senhas-bkp.txt
                for(int i = 0; i < capacidade; i++)
                {
                                if(tds_cadastros[i].atividade == 'A')
                                {
                                                cadastros_ativos++;
                                }
                }
                fprintf(fileptr, "%d\n", cadastros_ativos);
                for(int i = 0; i < capacidade; i++)
                {
                                if(tds_cadastros[i].atividade == 'A')
                                {
                                                fprintf(fileptr, "%s\n", tds_cadastros[i].login);

                                                strncpy(senha_descripto, tds_cadastros[i].senha, MAXSENHA);
                                                senha_descripto[MAXSENHA] = '\0';
                                                descriptografar(senha_mestra, senha_descripto);

                                                fprintf(fileptr, "%s\n", senha_descripto);

                                                fprintf(fileptr, "%s\n", tds_cadastros[i].info);
                                }
                }
                fclose(fileptr);
                printf("EXEC OPC3\n");
}

//subrotina opcao 4
int opcao_quatro(char senha_mestra[])
{
                char reader[MAXINFO];
                int total_cadastros;

                fileptr = fopen("senhas-bkp.txt", "rt");
                if(fileptr == NULL)
                {
                                printf("ERRO ARQ\n");
                                return -1;
                }
                //ler qntd de cadastros do arquivo
               fgets(reader, MAXINFO, fileptr);
               total_cadastros = atoi(reader);

                //alocar espaço para N + 30 cadastros
                tds_cadastros = (cadastro*) calloc(total_cadastros + 30, sizeof(cadastro));

                for(int i = 0; i < total_cadastros; i++)
                {
                          //gravando login
                          fgets(reader, MAXINFO, fileptr);
                          reader[strcspn(reader, "\r\n")] = '\0';
                          strcpy(tds_cadastros[i].login, reader);

                          //gravando senha criptografada
                          fgets(reader, MAXINFO, fileptr);
                          reader[strcspn(reader, "\r\n")] = '\0';
                          criptografar(senha_mestra, reader);
                          strncpy(tds_cadastros[i].senha, reader, MAXSENHA);

                          //gravando info
                          fgets(reader, MAXINFO, fileptr);
                          reader[strcspn(reader, "\r\n")] = '\0';
                          strcpy(tds_cadastros[i].info, reader);

                          //gravando atividade
                          tds_cadastros[i].atividade = 'A';
                }
                fclose(fileptr);
                printf("EXEC OPC4\n");
                return total_cadastros;
}

//subrotina opcao 5
int opcao_cinco(int total_cadastros, int *capacidade, char senha_mestra[])
{
    char reader[MAXINFO];
    int aux = 0, index;

    // se o cadastro ainda não foi criado, aloca 30 registros do zero
    if(tds_cadastros == NULL)
    {
        tds_cadastros = (cadastro*) calloc(30, sizeof(cadastro));
        *capacidade = 30;
    }

    index = total_cadastros;

    while(1)
    {
        fgets(reader, MAXINFO, stdin);
        reader[strcspn(reader, "\r\n")] = '\0';

        if(reader[0] == '#')
        {
            break;
        }
        else if(aux == 0)
        {
            strcpy(tds_cadastros[index].login, reader);
            aux++;
        }
        else if(aux == 1)
        {
            criptografar(senha_mestra, reader);
            strncpy(tds_cadastros[index].senha, reader, MAXSENHA);
            aux++;
        }
        else if(aux == 2)
        {
            strcpy(tds_cadastros[index].info, reader);
            tds_cadastros[index].atividade = 'A';
            aux = 0;
            index++;
        }
    }

    printf("EXEC OPC5\n");
    return index;
}

//subrotina opcao 6
void opcao_seis(int capacidade, char senha_mestra[])
{
    char *resultado = NULL;
    char find[MAXLOG];
    char find_lower[MAXLOG];
    char login_lower[MAXLOG];
    char senha_descripto[MAXSENHA + 1];

    fgets(find, MAXLOG, stdin);
    find[strcspn(find, "\r\n")] = '\0';

    strcpy(find_lower, find);
    para_minusculo(find_lower);

    printf("EXEC OPC6\n");
    for(int i = 0; i < capacidade; i++)
    {
        if(tds_cadastros[i].atividade == 'A')
        {
            strcpy(login_lower, tds_cadastros[i].login);
            para_minusculo(login_lower);

            resultado = strstr(login_lower, find_lower);
            if(resultado != NULL)
            {
                strncpy(senha_descripto, tds_cadastros[i].senha, MAXSENHA);
                senha_descripto[MAXSENHA] = '\0';
                descriptografar(senha_mestra, senha_descripto);

                printf("%s\n", tds_cadastros[i].login);
                printf("%s\n", senha_descripto);
                printf("%s\n", tds_cadastros[i].info);
            }
        }
    }
}

//subrotina opcao 7
void opcao_sete(int capacidade, char senha_mestra[])
{
    char find[MAXLOG];
    char nova_senha[MAXSENHA];
    int achou = 0;

    printf("EXEC OPC7\n");

    fgets(find, MAXLOG, stdin);
    find[strcspn(find, "\r\n")] = '\0';

    fgets(nova_senha, MAXSENHA, stdin);
    nova_senha[strcspn(nova_senha, "\r\n")] = '\0';

    for(int i = 0; i < capacidade; i++)
    {
        if(tds_cadastros[i].atividade == 'A' && strcmp(tds_cadastros[i].login, find) == 0)
        {
            criptografar(senha_mestra, nova_senha);
            strcpy(tds_cadastros[i].senha, nova_senha);
            achou = 1;
            printf("EXEC NEWPWD\n");
            break;
        }
    }

    if(!achou)
    {
        printf("EXEC FAILPWD\n");
    }
}

//subrotina opcao 8
void opcao_oito(int capacidade, char senha_mestra[])
{
    char *resultado1 = NULL;
    char *resultado2 = NULL;
    char find[MAXLOG];
    char find_lower[MAXLOG];
    char login_lower[MAXLOG];
    char info_lower[MAXINFO];
    char senha_descripto[MAXSENHA];

    fgets(find, MAXLOG, stdin);
    find[strcspn(find, "\r\n")] = '\0';

    strcpy(find_lower, find);
    para_minusculo(find_lower);

    printf("EXEC OPC8\n");
    for(int i = 0; i < capacidade; i++)
    {
        if(tds_cadastros[i].atividade == 'A')
        {
            strcpy(login_lower, tds_cadastros[i].login);
            para_minusculo(login_lower);

            strcpy(info_lower, tds_cadastros[i].info);
            para_minusculo(info_lower);

            resultado1 = strstr(login_lower, find_lower);
            resultado2 = strstr(info_lower, find_lower);

            if(resultado1 != NULL || resultado2 != NULL)
            {
                strncpy(senha_descripto, tds_cadastros[i].senha, MAXSENHA);
                senha_descripto[MAXSENHA] = '\0';
                descriptografar(senha_mestra, senha_descripto);

                printf("%s\n", tds_cadastros[i].login);
                printf("%s\n", senha_descripto);
                printf("%s\n", tds_cadastros[i].info);
            }
        }
    }
}

//subrotina opcao 9
void opcao_nove(int capacidade)
{
         char find[MAXLOG];
         int achou = 0;

         printf("EXEC OPC9\n");

         fgets(find, MAXLOG, stdin);
         find[strcspn(find, "\r\n")] = '\0';

         for(int i = 0; i < capacidade; i++)
         {
                if(tds_cadastros[i].atividade == 'A' && strcmp(tds_cadastros[i].login, find) == 0)
                {
                tds_cadastros[i].atividade = 'I';
                achou = 1;
                printf("EXEC DEL\n");
                break;
                }
         }
         if(!achou)
         {
                         printf("EXEC ERRDEL\n");
         }
}


int main()
{
    int opcao, total_cadastros = 0, capacidade = 0;
    int primeira_opcao = 1;
    char senha_mestra[11];

    printf("EXEC MAIN\n");
    while(1)
    {
                   opcao = le_opcao();
                   if(primeira_opcao)
                   {
                                if(opcao == -1)
                                {
                                    printf("EXEC FIM\n");
                                    break;
                                }
                                if(opcao != 0)
                                {
                                     printf("ZERO ERRO\n");
                                     break;
                                }
                                primeira_opcao = 0;
                   }
                   if(opcao == -1)
                   {
                                    free(tds_cadastros);
                                    printf("EXEC FIM\n");
                                    break;
                    }
                    else if(opcao == 0)
                    {
                                    opcao_zero(senha_mestra);
                    }
                    else if(opcao == 1)
                    {
                                    total_cadastros = opcao_um();
                                    //evitar que capacidade seja calculada com a saida erro -1
                                    if(total_cadastros >= 0)
                                    {
                                                    capacidade = total_cadastros + 30;
                                    }
                    }
                    else if(opcao == 2)
                    {
                                    opcao_dois(capacidade);
                    }
                    else if(opcao == 3)
                    {
                                    opcao_tres(capacidade, senha_mestra);
                    }
                    else if(opcao == 4)
                    {
                                   total_cadastros = opcao_quatro(senha_mestra);
                                    //evitar que capacidade seja calculada com a saida erro -1
                                    if(total_cadastros >= 0)
                                    {
                                                    capacidade = total_cadastros + 30;
                                    }
                    }
                    else if(opcao == 5)
                    {
                                    total_cadastros = opcao_cinco(total_cadastros, &capacidade, senha_mestra);
                    }
                    else if(opcao == 6)
                    {
                                    opcao_seis(capacidade, senha_mestra);
                    }
                    else if(opcao == 7)
                    {
                                    opcao_sete(capacidade, senha_mestra);
                    }
                    else if(opcao == 8)
                    {
                                    opcao_oito(capacidade, senha_mestra);
                    }
                    else if(opcao == 9)
                    {
                                    opcao_nove(capacidade);
                    }
                    else
                    {
                                    printf("EXEC ERRO\n");
                                    continue;
                    }
    }
    return 0;
}
