#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTXT 100

FILE *fileptr;

//struct para com infos para cada conta
typedef struct
{
    int number;
    char name[MAXTXT];
    double balance;
}accounts;

accounts *all_accounts = NULL;
int clients = 0;

//recebe do teclado opção de 0 a 7 para executar
int le_menu_opcao()
{
    char input_str[MAXTXT];
    int input;

    fgets(input_str, MAXTXT, stdin);
    input = atoi(input_str);

    return input;
}

//receber entrada para cada tipo de variavel
int recebe_vint()
{
                char str[MAXTXT];
                int nro_conta;

                fgets(str, MAXTXT, stdin);
                nro_conta =  atoi(str);
                return nro_conta;
}

double recebe_vdouble()
{
                char str[MAXTXT];
                double valor;

                fgets(str, MAXTXT, stdin);
                valor = atof(str);

                return valor;
}

//leitura file com informacões dos clientes
int le_arquivo(char *name)
{
    int aux = 0, client_index = 0, j;
    char reader[MAXTXT];

    //abrindo master.txt
    fileptr = fopen(name, "rt");
    if(fileptr == NULL)
    {
                    fclose(fileptr);
                    return -1;
    }

    //lendo do arquivo tamanho do vetor que deve alocar as contas e alocando dinamicamente esse vetor
    clients = atoi(fgets(reader, MAXTXT, fileptr));
    all_accounts = (accounts*)malloc(clients * sizeof(accounts));

    while(fgets(reader, MAXTXT, fileptr) != NULL)
    {
        if(reader[0] == '#')
        {
            break;
        }

        //alocar numero conta
        if(aux == 0)
        {
            all_accounts[client_index].number = atoi(reader);
            aux++;
        }

        //alocar nome da conta
        else if(aux == 1)
        {
            reader[strcspn(reader, "\r\n")] = '\0';
           //zerar j para novas execuções
            j = 0;

            //eliminando caracteres ocultos antes de alocar nome do cliente
            for(int i = 0; i <= strlen(reader); i++)
            {
                //i vai seguir leitura corrente da string original, enquanto j vai seguir determinando o novo tamanho sem caracteres ocultos
                if(reader[i] >= 32)
                {
                    reader[j] = reader[i];
                    j++;
                }
            }
            reader[j] = '\0';

            strcpy(all_accounts[client_index].name, reader);

            aux++;
        }

        //alocar saldo conta
        else if(aux == 2)
        {
            all_accounts[client_index].balance = atof(reader);
            aux = 0;
            client_index++;
        }
    }
    //fechando master.txt
    fclose(fileptr);

    return clients;
}

//gravar em final.txt dados contas
int grava_arquivo(char *name, int clients)
{
                fileptr = fopen(name, "wt");
                if(fileptr == NULL)
                {
                                fclose(fileptr);
                                return -1;
                }

                fprintf(fileptr, "%d\n", clients);
                for(int i = 0; i < clients; i++)
                {
                       fprintf(fileptr, "%d\n", all_accounts[i].number);
                       fprintf(fileptr, "%s\n", all_accounts[i].name);
                       fprintf(fileptr, "%.2lf\n", all_accounts[i].balance);
                }
                fprintf(fileptr, "#\n");

                fclose(fileptr);
                return clients;
}

//busca existência de cadastro para uma conta baseado no seu número
int procura_conta(int number)
{
                for(int i = 0; i < clients; i++)
                {
                                if(all_accounts[i].number == number)
                                {
                                                return i;
                                }
                }
                return -1;
}

//deposito
double deposito(int index_cliente, double deposito)
{
                all_accounts[index_cliente].balance += deposito;
                return all_accounts[index_cliente].balance;
}

//saque
double saque(int index_cliente, double saque)
{
                all_accounts[index_cliente].balance -= saque;
                return all_accounts[index_cliente].balance;
}

//transferência entre contas
void transfere(double *balance1, double *balance2)
{
                double aux;
                aux = *balance1;
                *balance1 = *balance2;
                *balance2 = aux;
}

//exibir saldo conta
void exibe_saldo_conta(int index_cliente)
{
                printf("%d\n", all_accounts[index_cliente].number);
                printf("%s\n", all_accounts[index_cliente].name);
                printf("%.2lf\n", all_accounts[index_cliente].balance);
}

//exibir todas as contas
void exibe_contas()
{
                printf("CONTAS %d\n", clients);
                for(int i = 0; i < clients; i++)
                {
                                printf("%d\n", all_accounts[i].number);
                                printf("%s\n", all_accounts[i].name);
                                printf("%.2lf\n", all_accounts[i].balance);
                }
}

int main()
{
                int opcao, aux_return, nro_conta, index[2];
                double valor, aux_return_value;

                while(1)
                {
                        opcao = le_menu_opcao();

                        //encerrar programa
                        if(opcao == 0)
                        {
                                        break;
                        }

                        //ler arquivos e alocar
                        else if(opcao == 1)
                        {
                                        aux_return = le_arquivo("master.txt");
                                        if(aux_return == -1)
                                        {
                                                        printf("ERRO ARQUIVO\n");
                                        }
                                        else
                                        {
                                                        printf("LIDOS %d\n", aux_return);
                                        }
                        }

                        //deposito
                        else if(opcao == 2)
                        {
                                        //entrada infos para deposito
                                        nro_conta = recebe_vint();
                                        valor = recebe_vdouble();

                                        aux_return = procura_conta(nro_conta);
                                        if(aux_return == -1)
                                        {
                                                        printf("ERRO CONTA\n");
                                        }
                                        else
                                        {
                                                        aux_return_value = deposito(aux_return, valor);
                                                        printf("SALDO %.2lf\n", aux_return_value);
                                        }
                        }

                        //saque
                        else if(opcao == 3)
                        {
                                        //entrada infos para saque
                                        nro_conta = recebe_vint();
                                        valor = recebe_vdouble();

                                        aux_return = procura_conta(nro_conta);
                                        if(aux_return == -1)
                                        {
                                                        printf("ERRO CONTA\n");
                                        }
                                        else
                                        {
                                                        aux_return_value = saque(aux_return, valor);
                                                        printf("SALDO %.2lf\n", aux_return_value);
                                        }
                        }

                        //transferencia
                        else if(opcao == 4)
                        {
                                        //entrada infos para transferencia
                                        for(int i = 0; i < 2; i++)
                                        {
                                                nro_conta = recebe_vint();
                                                index[i] = procura_conta(nro_conta);
                                                if(index[i] == -1)
                                                {
                                                        printf("ERRO CONTA%d\n", i+1);
                                                        aux_return = -1;
                                                        break;
                                                }
                                                else
                                                {
                                                                aux_return = 0;
                                                }
                                        }
                                        if(aux_return == 0)
                                        {
                                                        transfere(&all_accounts[index[0]].balance, &all_accounts[index[1]].balance);
                                                        printf("TRANSF OK\n");
                                        }
                        }

                        //consultar saldo de uma conta
                        else if(opcao == 5)
                        {
                                        nro_conta = recebe_vint();
                                        aux_return = procura_conta(nro_conta);
                                        if(aux_return == -1)
                                        {
                                                        printf("ERRO CONTA\n");
                                        }
                                        else
                                        {
                                                        exibe_saldo_conta(aux_return);
                                        }
                        }

                        //exibir todas contas
                        else if(opcao == 6)
                        {
                                        exibe_contas();
                        }

                        //gravar arquivo
                        else if(opcao == 7)
                        {
                                       aux_return =  grava_arquivo("final.txt", clients);
                                        if(aux_return == -1)
                                        {
                                                        printf("ERRO ARQUIVO\n");
                                        }
                                        else
                                        {
                                                        printf("GRAVADOS %d\n", aux_return);
                                        }
                        }
                }

                if (all_accounts != NULL)
                {
                                free(all_accounts);
                }
                return 0;
}
