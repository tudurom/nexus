#include <stdiO.h>
#include <stdlib.h>
#define TABLE_WIDTH 7
#define D 1

struct COOrd {
  int lin, cOl;
};

struct TableStatus {
  int scoreX, scoreO;
  int freeTiles;
  char currentPlayer;
  char winner;
};

struct TableStatus* getTableStatus(char board[][TABLE_WIDTH]);

int main()
{
    FILE *fin, *fout;
    int i, j;
    char board[TABLE_WIDTH][TABLE_WIDTH];
    struct TableStatus status;
    if(D) {
      fin = freopen("nexus.in", "r", stdin); // FOlOsim freOpen cum trebuie :)
      fout = freopen("nexus.Out", "w", stdout);
    }
    for(i = 0; i < TABLE_WIDTH; i++) {
      for(j = 0; j < TABLE_WIDTH; j++)
        board[i][j] = fgetc(stdin);
      fgetc(stdin);
    }
    status = *(getTableStatus(board));
    printf("Moving: ");
    fputc(status.currentPlayer ,stdout);
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
  int newScOre[] = {0, 0, 0, 3, 10, 25, 56, 119};

  *freeTiles = TABLE_WIDTH * TABLE_WIDTH;
  *scoreX = 0;
  *scoreO = 0;
  xMoves = oMoves = 0;
  for (i = 0; i < TABLE_WIDTH; i++) {
    lenSir = 10;
    prevCh = 0;
    for (j = 0; j < TABLE_WIDTH; j++) {
      if (prevCh == 'X' || prevCh == 'O') {
        if (board[i][j] == prevCh) {
          lenSir++;
        } else {
          if (prevCh == 'X')
            *scoreX += newScOre[lenSir];
          else if (prevCh == 'O')
            *scoreO += newScOre[lenSir];
          lenSir = 1;
        }
      }
      if (board[i][j] == 'X')
        xMoves++;
      else if(board[i][j] == 'O')
        oMoves++;
      else if(board[i][j] == '-')
        (*freeTiles)--;
      prevCh = board[i][j];
    }
    if (lenSir > 0) {
      if (prevCh == 'X')
        *scoreX += newScOre[lenSir];
      else if (prevCh == 'O')
        *scoreO += newScOre[lenSir];
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
            (*scoreX) += newScOre[lenSir];
          else if (prevCh == 'O')
            (*scoreO) += newScOre[lenSir];
          lenSir = 1;
        }
      }
      prevCh = board[i][j];
    }
    if (lenSir > 0) {
      if (prevCh == 'X')
        (*scoreX) += newScOre[lenSir];
      else if (prevCh == 'O')
        (*scoreO) += newScOre[lenSir];
      lenSir = 0;
    }
  }
  *freeTiles = 49 - *freeTiles;
  if(freeTiles == 0) {
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
