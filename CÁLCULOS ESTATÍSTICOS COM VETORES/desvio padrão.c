//NOME: CLARA BERTOLUZZI SÃO PEDRO
//Nº USP: 16860497

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000

FILE *fptr;
double numbers_f[MAXN];
char numbers_c[MAXN];

int main()
{
    int dado_inicial, tam_janela, nro_janelas, deslocamento;
    double max, min, media = 0.0;

    scanf("%d", &dado_inicial);
    scanf("%d", &tam_janela);
    scanf("%d", &nro_janelas);
    scanf("%d", &deslocamento);

    fptr = fopen("serie.txt", "rt");
    if(fptr == NULL)
    {
        printf("ERROARQ\n");
        return -1;
    }

    for(int i = 0; !feof(fptr); i++)
    {
        if(i < MAXN)
        {
            fgets(numbers_c, MAXN, fptr);
            if(strncmp(numbers_c, "#", 1) == 0)
            {
                break;
            }
            else
            {
                numbers_f[i] = atof(numbers_c);
            }
        }
        else
        {
            printf("ERRODADO\n");
            return -1;
        }
    }


    for( int j = 0; j < nro_janelas; j++)
    {
        max = numbers_f[dado_inicial];
        min = numbers_f[dado_inicial];
        for(int i = dado_inicial; i < dado_inicial + tam_janela; i++)
        {
            if(numbers_f[i] < min)
            {
                min = numbers_f[i];
            }
            if(numbers_f[i] > max)
            {
                max = numbers_f[i];
            }
            media += numbers_f[i];
        }
        printf("%.2lf\n", min);
        printf("%.2lf\n", max);
        printf("%.2lf\n", media / tam_janela);

        dado_inicial += deslocamento;
        media = 0;
    }
    fclose(fptr);

}
