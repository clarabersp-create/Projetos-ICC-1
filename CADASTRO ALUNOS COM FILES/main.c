//NOME: CLARA BERTOLUZZI SÃO PEDRO
//NUSP: 16860497

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTXT 50

FILE *fileptr;

//struct para com infos cada aluno
typedef struct
{
    int number;
    char name[MAXTXT];
    double media;
}alunos;

int students = 0;
alunos *all_students = NULL;
alunos *search_students = NULL;

//remove caracteres ocultos de uma string
void remove_ocultos(char *str)
{
    int index_cleanstr = 0;
    int tam = strlen(str);

    for(int i = 0; i < tam; i++)
    {
        if(str[i] >= 32)
        {
            str[index_cleanstr] = str[i];
            index_cleanstr++;
        }
    }
    str[index_cleanstr] = '\0';
}

//passar para maiúsculas
void passa_maiuscula(char *reader)
{
    int tam = strlen(reader);
    for(int a = 0; a < tam; a++)
    {
        reader[a] = toupper((unsigned char)reader[a]);
    }
}

//leitura arquivo com infos alunos
int le_arquivo(char *name)
{
    int index = 0, aux = 0;
    char reader[MAXTXT];

    //abrindo alunos.txt
    fileptr = fopen(name, "rt");
    if(fileptr == NULL)
    {
                    printf("ERRO\n");
                    return -1;
    }

    //lendo do arquivo tamanho qnts alunos e alocando vetor p/ dados iniciais
    students = atoi(fgets(reader, MAXTXT, fileptr));
    all_students = (alunos*)malloc(students * sizeof(alunos));

    //alocando struct para busca dos alunos com max tam possível (tds alunos)
    search_students = (alunos*)malloc(students * sizeof(alunos));

    while(fgets(reader, MAXTXT, fileptr) != NULL)
    {
        if(reader[0] == '#')
        {
            break;
        }

        //alocar nusp
        if(aux == 0)
        {
            all_students[index].number = atoi(reader);
            aux++;
        }

        //alocar nome
        else if(aux == 1)
        {
            reader[strcspn(reader, "\r\n")] = '\0';

            //eliminando caracteres ocultos antes de alocar nome
            remove_ocultos(reader);

            //passando para letra maiuscula
            passa_maiuscula(reader);

            strcpy(all_students[index].name, reader);

            aux++;
        }

        //alocar media
        else if(aux == 2)
        {
            all_students[index].media = atof(reader);
            aux = 0;
            index++;
        }
    }
    //fechando alunos.txt
    fclose(fileptr);

    return students;
}

//exibe dados de n alunos armazenados no vetor criado
void exibe_alunos(alunos *lista, int numalunos)
{
    printf("%d\n", numalunos);
    for(int b = 0; b < numalunos; b++)
    {
        printf("%d\n", lista[b].number);
        printf("%s\n", lista[b].name);
        printf("%.2lf\n", lista[b].media);
    }
    printf("#\n");
}

//procura nome/parte de nome armazenado no cadastro e aloca na struct de resultado
void procura_alunos(int *total_achados, char *buscar, double nova_media)
{
    int aux = 0;
    char *resultado_busca;

    remove_ocultos(buscar);

    for(int c = 0; c < students; c++)
    {
        resultado_busca = strstr(all_students[c].name, buscar);
        if(resultado_busca == NULL)
        {
            continue;
        }
        else
        {
            search_students[aux].number = all_students[c].number;
            strcpy(search_students[aux].name, all_students[c].name);
            search_students[aux].media = nova_media;

            (*total_achados)++;
            aux++;
        }
    }

}

//gravar alunos encontrados na busca em "saida.txt"
void grava_alunos(char *name, int *total_achados)
{
    fileptr = fopen(name, "wt");
                if(fileptr == NULL)
                {
                        printf("ERRO\n");
                        return;
                }

                fprintf(fileptr, "%d\n", *total_achados);
                for(int i = 0; i < *total_achados; i++)
                {
                       fprintf(fileptr, "%d\n", search_students[i].number);
                       fprintf(fileptr, "%s\n", search_students[i].name);
                       fprintf(fileptr, "%.2lf\n", search_students[i].media);
                }
                fprintf(fileptr, "#\n");

                fclose(fileptr);
}

int main()
{
    int total_achados = 0;
    char aux[MAXTXT];
    double nova_media = 100.0;

    le_arquivo("alunos.txt");

    fgets(aux, MAXTXT, stdin);
    remove_ocultos(aux);
    passa_maiuscula(aux);

    procura_alunos(&total_achados, aux, nova_media);

    grava_alunos("saida.txt", &total_achados);

    system("sleep 3; cat saida.txt &");

    exibe_alunos(all_students, students);

    free(all_students);
    free(search_students);
    return 0;
}
