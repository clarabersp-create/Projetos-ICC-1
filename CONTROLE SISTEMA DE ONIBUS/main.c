//NOME: CLARA BERTOLUZZI S�O PEDRO
//NUSP: 16860497

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100


FILE *fileptr;

//struct infos onibus
typedef struct
{
    int numero_parada;
    int dia, mes, ano;
    int hora, minuto;
    int entrada;
    int saida;
    int lotacao;
    char nome_linha[MAXN];
}onibus;

onibus *tds_paradas = NULL;

int le_arquivo(char *nome)
{
    int total_paradas = 0, n = 0;
    char reader[MAXN];

    //abrindo trajeto.txt
    fileptr = fopen(nome, "rt");
    if(fileptr == NULL)
    {
                    printf("ERRO\n");
                    return -1;
    }

    while(fgets(reader, MAXN, fileptr) != NULL)
    {
        if(strstr(reader, "-1") != NULL)
        {
            break;
        }
        if(reader[0] == '\n' || reader[0] == '\r' || strlen(reader) <= 1)
        {
            continue;
        }
        total_paradas++;
    }
    rewind(fileptr);
    //alocando espa�o p todas paradas
    tds_paradas = (onibus*)malloc(total_paradas * sizeof(onibus));

    while(fgets(reader, MAXN, fileptr) != NULL)
    {
        reader[strcspn(reader, "\r\n")] = '\0';

        if(strstr(reader, "-1") != NULL)
        {
            break;
        }

        char *token;

        token = strtok(reader, " ");
        if(token == NULL) continue;
        tds_paradas[n].numero_parada = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].dia = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].mes = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].ano = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].hora = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].minuto = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].entrada = atoi(token);

        token = strtok(NULL, " ");
        tds_paradas[n].saida = atoi(token);

        token = strtok(NULL, " ");
        strncpy(tds_paradas[n].nome_linha, token, sizeof(tds_paradas[n].nome_linha) - 1);
        tds_paradas[n].nome_linha[sizeof(tds_paradas[n].nome_linha) - 1] = '\0';

        n++;
        if(n >= total_paradas) break;
    }

    fclose(fileptr);
    return total_paradas;
}

void exibe_dados(int ndados)
{
    for(int i = 0; i < ndados; i++)
    {
        printf("%d %d %d %d %d %d %d %d %s\n", tds_paradas[i].numero_parada,tds_paradas[i].dia, tds_paradas[i].mes, tds_paradas[i].ano, tds_paradas[i].hora, tds_paradas[i].minuto, tds_paradas[i].entrada, tds_paradas[i].saida, tds_paradas[i].nome_linha);
    }
    printf("%d\n", ndados);
}

void calcula_dados(int ndados, int* tempohtot, int* tempomtot, int* passtot, int* lotmax, int *lotmin)
{
    int tempo, passageiros = 0;

    //calculando tempo da viagem
    tempo = (tds_paradas[ndados - 1].hora - tds_paradas[0].hora)*60;
    tempo += (tds_paradas[ndados - 1].minuto - tds_paradas[0].minuto);
    *tempohtot = tempo / 60;
    *tempomtot = tempo % 60;

    //calculando total passageiros
    for(int i = 0; i < ndados; i++)
    {
        passageiros += tds_paradas[i].entrada;
    }
    *passtot = passageiros;

    //calculando lota��o
    for(int i = 0; i < ndados; i++)
    {
        if(i == 0)
        {
            tds_paradas[i].lotacao = tds_paradas[i].entrada - tds_paradas[i].saida;
            *lotmax = tds_paradas[i].lotacao;
            *lotmin = tds_paradas[i].lotacao;
            continue;
        }
        else
        {
            tds_paradas[i].lotacao = tds_paradas[i-1].lotacao + tds_paradas[i].entrada - tds_paradas[i].saida;
        }

        if(tds_paradas[i].lotacao > *lotmax)
        {
            *lotmax = tds_paradas[i].lotacao;
        }
        if(tds_paradas[i].lotacao != 0 && tds_paradas[i].lotacao < *lotmin)
        {
            *lotmin = tds_paradas[i].lotacao;
        }
    }
}

void exibe_calculos(int temphtot, int tempomtot, int passtot, int lotmax, int lotmin)
{
    printf("%d %d\n", temphtot, tempomtot);
    printf("%d\n%d\n%d\n", passtot, lotmin, lotmax);
}

void grava_dados(char *nome, int ndados)
{
    fileptr = fopen(nome, "wt");
    if(fileptr == NULL)
    {
                    printf("ERRO\n");
                    return;
    }

    for(int i = 0; i < ndados; i++)
    {
        fprintf(fileptr, "%d %d %d %d\n", tds_paradas[i].numero_parada, tds_paradas[i].entrada, tds_paradas[i].saida, tds_paradas[i].lotacao);
    }

    fclose(fileptr);
}

int main()
{
    int total_paradas, tempohtot, tempomtot, passtot, lotmax, lotmin;

    total_paradas = le_arquivo("trajeto.txt");
    exibe_dados(total_paradas);
    calcula_dados(total_paradas, &tempohtot, &tempomtot, &passtot, &lotmax, &lotmin);
    exibe_calculos(tempohtot, tempomtot, passtot, lotmax, lotmin);
    grava_dados("dados.txt", total_paradas);

    fflush(stdout);
    system("sleep 1; cat dados.txt");

    free(tds_paradas);
    return 0;
}
