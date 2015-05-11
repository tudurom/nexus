#include <stdio.h>
#include <stdlib.h>
#define TABLE_WIDTH 7
#define D 0
#define DEPTH 2

struct TableStatus { // Structura care tine minte in ce stadiu este tabla
  int scoreX, scoreO;
  int freeTiles;
  char currentPlayer;
  char winner;
};

struct TableStatus* getTableStatus(char board[][TABLE_WIDTH]); // Returneaza starea unei table
int minimax(char board[][TABLE_WIDTH], int depth, int maximizingPlayer); // Logica
char oppositePlayer(char player);

int movelf, movecf;
int computerPlayer;

int main()
{
    FILE *fin, *fout;
    int i, j;
    char board[TABLE_WIDTH][TABLE_WIDTH];
    struct TableStatus *status;
    int moveLine, moveCol;
    int score, tempScore;
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
    computerPlayer = status->currentPlayer;
    moveLine = moveCol = 0;
    score = -1667;
    for(i = 0; i < TABLE_WIDTH; i++) {
      for(j = 0; j < TABLE_WIDTH; j++) {
        if(board[i][j] == '-') {
          board[i][j] = computerPlayer;
          tempScore = -minimax(board, DEPTH, oppositePlayer(computerPlayer));
          board[i][j] = '-';
          if(tempScore > score) {
            score = tempScore;
            moveLine = i;
            moveCol = j;
          }
        }
      }
    }
    board[moveLine][moveCol] = computerPlayer;
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

int minimax(char board[][TABLE_WIDTH], int depth, int maximizingPlayer) {
  struct TableStatus *status = getTableStatus(board);
  char player = maximizingPlayer ? computerPlayer : oppositePlayer(computerPlayer);
  if(depth == 0 || status->freeTiles == 0) {
    if(maximizingPlayer)
      return (computerPlayer == 'X' ? status->scoreX : status->scoreO);
    else
      return -(computerPlayer == 'X' ? status->scoreX : status->scoreO);
  }

  int bestScore = -1667;
  int i, j;
  for(i = 0; i < TABLE_WIDTH; i++) {
    for(j = 0; j < TABLE_WIDTH; j++) {
      if(board[i][j] == '-') {
        board[i][j] = oppositePlayer(player);
        int score = -minimax(board, depth - 1, !maximizingPlayer);
        board[i][j] = '-';
        if(score > bestScore)
          bestScore = score;
      }
    }
  }
  return bestScore;
}

char oppositePlayer(char player) {
  return (player == 'X' ? 'O' : 'X');
}
