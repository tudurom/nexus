#include <stdio.h>
#include <stdlib.h>
#define TABLE_WIDTH 7
#define DEPTH 3
#define INFINIT 1667
#define D 0
#define X 1
#define O -1
#define PLUS -2
#define GOL 0


struct TableStatus { // Structura returnata de getTableStatus()
  int scores[3];
  int freeTiles;
  int currentPlayer;
};

struct TableStatus* getTableStatus( char board[TABLE_WIDTH][TABLE_WIDTH] ); // Returneaza scorul lui X, al lui O si
                                                                            // Numarul de patratele libere
int minimax( char board[TABLE_WIDTH][TABLE_WIDTH], char depth, short alpha, short beta, char player, char computerPlayer ); // Logica jocului
inline int max( int a, int b );
inline int min( int a, int b );
int thisPlayer;

int main() {
  FILE *fin, *fout;
  int lin, col;
  char board[TABLE_WIDTH][TABLE_WIDTH];
  struct TableStatus *status;
  int moveLine, moveCol;
  int score, tempScore;

  if(D) {
    fin = freopen( "nexus.in", "r", stdin );
    fout = freopen( "nexus.out", "w", stdout );
  }
  for ( lin = 0; lin < TABLE_WIDTH; lin++ ) {
    for ( col = 0; col < TABLE_WIDTH; col++ ) {
      char c = fgetc( stdin );
      char nr;
      switch ( c ) {
      case 'X':
        nr = X;
        break;
      case 'O':
        nr = O;
        break;
      case '+':
        nr = PLUS;
        break;
      default:
        nr = GOL;
        break;
      }
      board[lin][col] = nr;
    }
    fgetc( stdin );
  }
  status = getTableStatus(board);
  thisPlayer = status->currentPlayer;
  moveLine = moveCol = -1;

  score = -INFINIT;
  for ( lin = 0; lin < TABLE_WIDTH; lin++ ) {
    for ( col = 0; col < TABLE_WIDTH; col++) {
      if ( board[lin][col] == GOL ) {
        board[lin][col] = thisPlayer;
        tempScore = -minimax(board, DEPTH, -INFINIT, +INFINIT, -thisPlayer, thisPlayer);
        board[lin][col] = GOL;
        if ( tempScore > score ) {
          score = tempScore;
          moveLine = lin;
          moveCol = col;
        }
      }
    }
  }
  board[moveLine][moveCol] = thisPlayer;

  for ( lin = 0; lin < TABLE_WIDTH; lin++ ) {
    for ( col = 0; col < TABLE_WIDTH; col++ ) {
      switch ( board[lin][col] ) {
        case X: fputc( 'X', stdout ); break;
        case O: fputc( 'O', stdout ); break;
        case PLUS: fputc( '+', stdout ); break;
        default: fputc( '-', stdout ); break;
      }
    }
    fputc( '\n', stdout );
  }

  if(D) {
    fclose( fin );
    fclose( fout );
  }
  return 0;
}

inline int max( int a, int b ) {
  if(a > b)
    return a;
  else
    return b;
}

inline int min( int a, int b ) {
  if (a < b)
    return a;
  else
    return b;
}

struct TableStatus* getTableStatus(char board[][TABLE_WIDTH]) {
  struct TableStatus* status = malloc(sizeof(struct TableStatus));
  int scoreX, scoreO;
  int freeTiles;
  int i, j;
  int lenSir;
  int xPlies, oPlies;
  int newScore[] = {0, 0, 2, 3, 10, 25, 56, 119};

  xPlies = oPlies = 0;
  scoreX = scoreO = 0;
  freeTiles = 0;

  for ( i = 0; i < TABLE_WIDTH; i++ ) {
    for ( j = 0; j < TABLE_WIDTH; j++ ) {
      if ( j > 0 ) {
        if ( board[i][j - 1] == board[i][j] ) {
          lenSir++;
        } else if ( board[i][j - 1] == X ) {
          scoreX += newScore[lenSir];
        } else if ( board[i][j - 1] == O ) {
          scoreO += newScore[lenSir];
        }
      } else {
        lenSir = 1;
      }
      switch(board[i][j]) {
        case X: xPlies++; break;
        case O: oPlies++; break;
        case GOL: freeTiles++; break;
      }
    }
    if (board[i][j - 1] == X)
      scoreX += newScore[lenSir];
    else
      scoreO += newScore[lenSir];
  }
  for ( j = 0; j < TABLE_WIDTH; j++ ) {
    for ( i = 0; i < TABLE_WIDTH; i++ ) {
      if ( i > 0 ) {
        if ( board[i - 1][j] == board[i][j] ) {
          lenSir++;
        } else if ( board[i - 1][j] == X ) {
          scoreX += newScore[lenSir];
        } else if ( board[i - 1][j] == O ) {
          scoreO += newScore[lenSir];
        }
      } else {
        lenSir = 1;
      }
    }
    if (board[i - 1][j] == X)
      scoreX += newScore[lenSir];
    else
      scoreO += newScore[lenSir];
  }

  status->scores[O + 1] = scoreO;
  status->scores[X + 1] = scoreX;
  status->currentPlayer = (xPlies > oPlies ? O : X);
  status->freeTiles = freeTiles;

  return status;
}

int minimax(char board[TABLE_WIDTH][TABLE_WIDTH], char depth, short alpha, short beta, char player, char computerPlayer) {
  struct TableStatus *status = getTableStatus(board);
  if( depth == 0 || status->freeTiles == 0 ) {
//    return status->scores[player + 1] > status->scores[-player + 1] ? 1 :
//           status->scores[-player + 1] > status->scores[player + 1] ? -1 : 0;
      return status->scores[computerPlayer + 1];
  }

  int scor = -INFINIT;
  int lin = 0;
  while (lin < TABLE_WIDTH && alpha < beta) {
    int col = 0;
    while (col < TABLE_WIDTH && alpha < beta) {
        if ( board[lin][col] == GOL ) {
            board[lin][col] = player;
            int miniScor = -minimax(board, depth - 1, -beta, -alpha, -player, computerPlayer);
            alpha = max(alpha, miniScor);
            board[lin][col] = GOL;
        }
        col++;
    }
    lin++;
  }
//  for ( lin = 0; lin < TABLE_WIDTH; lin++ ) {
//    for ( col = 0; col < TABLE_WIDTH; col++ ) {
//      if ( board[lin][col] == GOL ) {
//        board[lin][col] = player;
//        int miniScor = -minimax(board, depth - 1, -player);
//        scor = max(scor, miniScor);
//        board[lin][col] = GOL;
//      }
//    }
//  }

  return min(alpha, beta);
}
