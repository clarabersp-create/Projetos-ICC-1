#include <stdio.h>
#include <stdlib.h>

void print_board(int board[10])
{
                for(int i = 1; i <= 9; i++)
                {
                                if(i % 3 == 0)
                                {
                                                printf("%d\n", board[i]);
                                }
                                else
                                {
                                                printf("%d ", board[i]);
                                }

                }
}

int check_draw(int board[10])
{
                int flag;
                for(int i = 1; i < 10; i++)
                {
                                if(board[i] == 0)
                                {
                                                flag = 0;
                                                return 0;
                                }
                                else
                                {
                                                flag = 1;
                                }
                }
                if(flag)
                {
                                printf("Empate!\n");
                                print_board(board);
                                return 1;
                }
}

int main()
{
    int position, player = 1, board[10] = {0}, flag = 2, draw;
    for(;;)
    {
               scanf("%d", &position);
               if(position == -1) // caso abandono jogo
               {
                         printf("Jogada Valida %d\nFIM DE JOGO", position) ;
                         break;
               }
               else if(player == 1)  //vez do jogador 1
               {
                                if(board[position] == 0 && position >= 1 && position <= 9) //testa validade da jogada ou possível empate
                                {
                                                board[position] = 1;
                                                printf("Jogada Valida %d\n", position);
                                                if(board[1] == 1 && board[2] == 1 && board[3] == 1 || board[4] == 1 && board[5] == 1 && board[6] == 1 || board[7] == 1 && board[8] == 1 && board[9] == 1 || board[1] == 1 && board[5] == 1 && board[9] == 1 || board[3] == 1 && board[5] == 1 && board[7] == 1 || board[1] == 1 && board[4] == 1 && board[7] == 1 || board[2] == 1 && board[5] == 1 && board[8] == 1 || board[3] == 1 && board[6] == 1 && board[9] == 1)
                                                { //caso jogador 1 vence
                                                                printf("Jogador 1 venceu!\nTabuleiro:\n");
                                                                print_board(board);
                                                                break;

                                                }
                                                else
                                                {
                                                                printf("Jogador 2 joga agora!\nTabuleiro:\n");
                                                                print_board(board);
                                                }
                                }
                                else //casos jogada invalida
                                {
                                                for(int i = 1; i <= 9; i++)
                                                {
                                                      if(board[i] == 0)
                                                      {
                                                                      flag = 0;
                                                                      break;
                                                      }
                                                }
                                                if(flag == 0)
                                                {
                                                           printf("Jogada Invalida %d\n", position);
                                                           printf("Jogador 1 joga agora!\nTabuleiro:\n");
                                                                print_board(board);
                                                           continue;
                                                }
                                }
                                draw = check_draw(board);
                                if(draw)
                                {
                                                break;
                                }
                                player = (player % 2) + 1; //mudar a vez para o outro jogador
                                continue;
               }
               else if(player == 2) //vez do jogador 2
               {
                                if(board[position] == 0 && position >= 1 && position <= 9) //testa validade jogada e possivel empate
                                {
                                                board[position] = 2;
                                                printf("Jogada Valida %d\n", position);
                                                if(board[1] == 2 && board[2] == 2 && board[3] == 2 || board[4] == 2 && board[5] == 2 && board[6] == 2 || board[7] == 2 && board[8] == 2 && board[9] == 2 || board[1] == 2 && board[5] == 2 && board[9] == 2 || board[3] == 2 && board[5] == 2 && board[7] == 2 || board[1] == 2 && board[4] == 2 && board[7] == 2 || board[2] == 2 && board[5] == 2 && board[8] == 2 || board[3] == 2 && board[6] == 2 && board[9] == 2)
                                                { //caso jogador 2 vencedor
                                                                printf("Jogador 2 venceu!\nTabuleiro:\n");
                                                                print_board(board);
                                                                break;
                                                }
                                                else
                                                {
                                                                printf("Jogador 1 joga agora!\nTabuleiro:\n");
                                                }
                                                print_board(board);
                                }
                                else //casos jogada invalida ou empate
                                {
                                                for(int i = 1; i <= 9; i++)
                                                {
                                                      if(board[i] == 0)
                                                      {
                                                                      flag = 0;
                                                                      break;
                                                      }
                                                }
                                                if(flag == 0)
                                                {
                                                           printf("Jogada Invalida %d\n", position);
                                                           printf("Jogador 2 joga agora!\nTabuleiro:\n");
                                                           print_board(board);
                                                           continue;
                                                }
                                }
                                draw = check_draw(board);
                                if(draw)
                                {
                                                break;
                                }
                                player = (player % 2) + 1;
                                continue;
               }

    }
    return 0;
}
