//NOME: CLARA BERTOLUZZI SÃO PEDRO
//Nº USP: 16860497

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100

FILE *fptr;

int cod_produto[MAXN];
int qntd_produto[MAXN];
float preco_produto[MAXN];
char descricao[MAXN];

int main()
{
    int i = 0;
    float preco_total = 0.0;
    fptr = fopen("acme.txt", "wt");

    while(1)
    {
        scanf("%d", &cod_produto[i]);
        while (getchar() != '\n');
        if(cod_produto[i] == -1)
        {
            break;
        }
        fgets(descricao, MAXN, stdin);
        scanf("%d", &qntd_produto[i]);
        scanf("%f", &preco_produto[i]);
        while (getchar() != '\n');
        preco_total += qntd_produto[i] * preco_produto[i];

        fprintf(fptr, "%d\n", cod_produto[i]);
        fputs(descricao, fptr);
        fprintf(fptr, "%d\n%.2f\n", qntd_produto[i], preco_produto[i]);
        i++;
    }
    fprintf(fptr, "#%d\n$%.2f", i, preco_total);
    fclose(fptr);
    system("cat acme.txt");
    return 0;
}
