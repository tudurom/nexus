#include <stdiO.h>
#include <stdlib.h>
#define TABLE_WIDTH 7
#define D 1

struct TableStatus { // Structura care tine minte in ce stadiu este tabla
  int scoreX, scoreO;
  int freeTiles;
  char currentPlayer;
  char winner;
};

struct TableStatus* getTableStatus(char board[][TABLE_WIDTH]); // Returneaza starea unei table
int minimax(char board[][TABLE_WIDTH], int depth, char player); // Logica

int movelf, movecf;

int main()
{
    FILE *fin, *fout;
    int i, j;
    char board[TABLE_WIDTH][TABLE_WIDTH];
    struct TableStatus *status;
    int movel, movec;
    int score;
    char player;
    if(D) {
      fin = freopen("nexus.in", "r", stdin); // Folosim freopen cum trebuie :)
      fout = freopen("nexus.out", "w", stdout);
    }
    for(i = 0; i < TABLE_WIDTH; i++) { // CIteste tabla
      for(j = 0; j < TABLE_WIDTH; j++)
        board[i][j] = fgetc(stdin);
      fgetc(stdin);
    }
    status = getTableStatus(board);
    player = status->currentPlayer;
    movel = movec = -1;
    score = -1666;
    for(i = 0; i < TABLE_WIDTH; i++) {
      for(j = 0; j < TABLE_WIDTH; j++) {
        if(board[i][j] == '-') {
          board[i][j] = player;
          int tempScore = -minimax(board, 3, player == 'X' ? 'O' : 'X');
          board[i][j] = '-';
          if(tempScore > score) {
            score = tempScore;
            movel = i;
            movec = j;
          }
        }
      }
    }
    board[movel][movec] = player;
    for(i = 0; i < TABLE_WIDTH; i++) {
      for(j = 0; j < TABLE_WIDTH; j++)
        fputc(board[i][j], stdout);
      fputc('\n', stdout);
    }
    if(D) {
      fclose(fin);
      fclose(fout);
    }
    return 0;
}

struct TableStatus* getTableStatus(char board[][TABLE_WIDTH]) {
  struct TableStatus* status = malloc(sizeof(struct TableStatus));
  int *scoreX = &(status->scoreX);
  int *scoreO = &(status->scoreO);
  int *freeTiles = &(status->freeTiles);
  int i, j;
  int prevCh, lenSir;
  int xMoves, oMoves;
  int newScore[] = {0, 0, 0, 3, 10, 25, 56, 119};

  *freeTiles = TABLE_WIDTH * TABLE_WIDTH;
  *scoreX = 0;
  *scoreO = 0;
  xMoves = oMoves = 0;
  for (i = 0; i < TABLE_WIDTH; i++) {
    lenSir = 1;
    prevCh = 0;
    for (j = 0; j < TABLE_WIDTH; j++) {
      if (prevCh == 'X' || prevCh == 'O') { // Calculaza scorul
        if (board[i][j] == prevCh) {
          lenSir++;
        } else {
          if (prevCh == 'X')
            (*scoreX) += newScore[lenSir];
          else if (prevCh == 'O')
            (*scoreO) += newScore[lenSir];
          lenSir = 1;
        }
      }
      // Calculeaza numarul de mutari
      if (board[i][j] == 'X')
        xMoves++;
      else if(board[i][j] == 'O')
        oMoves++;
      else if(board[i][j] == '-') // Calculeaza numarul de patrate libere
        (*freeTiles)--;
      prevCh = board[i][j];
    }
    if (lenSir > 0) {
      if (prevCh == 'X')
        *scoreX += newScore[lenSir];
      else if (prevCh == 'O')
        *scoreO += newScore[lenSir];
      lenSir = 0;
    }
  }
  for (j = 0; j < TABLE_WIDTH; j++) {
    lenSir = 1;
    prevCh = 0;
    for (i = 0; i < TABLE_WIDTH; i++) {
      if (prevCh == 'X' || prevCh == 'O') {
        if (board[i][j] == prevCh) {
          lenSir++;
        } else {
          if (prevCh == 'X')
            (*scoreX) += newScore[lenSir];
          else if (prevCh == 'O')
            (*scoreO) += newScore[lenSir];
          lenSir = 1;
        }
      }
      prevCh = board[i][j];
    }
    if (lenSir > 0) {
      if (prevCh == 'X')
        (*scoreX) += newScore[lenSir];
      else if (prevCh == 'O')
        (*scoreO) += newScore[lenSir];
      lenSir = 0;
    }
  }
  *freeTiles = 49 - *freeTiles;
  if(*freeTiles == 0) {
    status->winner = *scoreX > *scoreO ? 'X' : 'O';
  } else {
    status->winner = 0;
  }
  if(xMoves > oMoves)
    status->currentPlayer = 'O';
  else
    status->currentPlayer = 'X';

  return status;
}

int minimax(char board[][TABLE_WIDTH], int depth, char player) {
  struct TableStatus *status = getTableStatus(board);
  int winner = status->winner;
  if(depth == 0 || winner != 0) {
    int winnerCode = winner == player ? -1 : 1;
    if(winner == player)
      return winnerCode - depth;
    else
      return depth - winnerCode;
  }
  int movel, movec;
  int score = -1666;
  int i, j;
  movel = movec = -1;
  for(i = 0; i < TABLE_WIDTH; i++) {
    for(j = 0; j < TABLE_WIDTH; j++) {
      if(board[i][j] == '-') {
        board[i][j] = player;
        int thisScore = -minimax(board, depth - 1, (player == 'X' ? 'O' : 'X'));
        if(thisScore > score) {
          score = thisScore;
          movel = i;
          movec = j;
        }
        board[i][j] = '-';
      }
    }
  }
  if(movec == -1 || movel == -1)
    return 0;
  movelf = movel;
  movecf = movec;
  return score;
}
