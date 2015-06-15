/*
   Nume program   : nexus.c
   Nume concurent : Tudor Roman, Andrei Cioltan
   E-mail         : romantudorel@gmail.com, cioltanandrei@yahoo.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TABLE_WIDTH 7
#define DEPTH 4
#define INFINIT 2147483647 // Limita pozitiva a intului
#define D 0 // Debug
#define X 1
#define O -1
#define PLUS -2
#define GOL 0
#define STOP_PROGRAM 980000 // Timpul pe care il are programul ca sa gaseasca o mutare


struct TableStatus { // Structura returnata de getTableStatus()
  int scores[3];
  int freeTiles;
  int currentPlayer;
};

void getTableStatus( char board[TABLE_WIDTH][TABLE_WIDTH],
                    struct TableStatus *status ); // Returneaza scorul lui X, al lui O si
                                                                            // Numarul de patratele libere
int minimax( char board[TABLE_WIDTH][TABLE_WIDTH], char depth, int alpha,
             int beta, char player, char computerPlayer ); // Logica jocului
inline int max( int a, int b );
inline int min( int a, int b );
inline int playerWon( int player, int scores[3] ); // Returneaza 1 daca player castiga, -1 daca -player castiga, 0 pt. remiza
inline int positionalEval( int lin, int col ); // Returneaza un scor in functie de pozitia mutarii
inline int positionalEval2( int lin, int col, char player );
void rebuildPositionalEval2Scores( char board[TABLE_WIDTH][TABLE_WIDTH] ); // Recalculeaza potentialul de extindere pentru ambii jucatori in matricea newPosScore2
// Functie care returneaza timpul in microsecunde de la 1 ian 1970
// sursa: http://algopedia.ro
inline long long getTime() {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000LL + tv.tv_usec;
}

long long programStart; // Timpul la care programul incepe executia
long long plyCounter = 0; // Nr. de semi-mutari procesate. Pt. statistica
int thisPlayer; // Jucatorul cu care joaca programul

int newMatScore[] = {0, 0, 0, 3, 10, 25, 56, 119};

int newPosScore[TABLE_WIDTH][TABLE_WIDTH] = {
  {0, 1, 2, 3, 2, 1, 0},
  {1, 4, 5, 6, 5, 4, 1},
  {2, 5, 7, 8, 7, 5, 2},
  {3, 6, 8, 9, 8, 6, 3},
  {2, 5, 7, 8, 7, 5, 2},
  {1, 4, 5, 6, 5, 4, 1},
  {0, 1, 2, 3, 2, 1, 0}
};
int newPosScore2[3][TABLE_WIDTH][TABLE_WIDTH];

int main() {
  FILE *fin, *fout;
  int lin, col;
  char board[TABLE_WIDTH][TABLE_WIDTH];
  struct TableStatus status;
  int moveLine, moveCol;
  int score, tempScore;

  // Pt. debug redeschidem stdin si stdout, ca sa nu complicam codul
  if(D) {
    fin = freopen( "nexus.in", "r", stdin );
    fout = freopen( "nexus.out", "w", stdout );
  }
  programStart = getTime(); // Asta este pusa dupa deschiderea fiserelor, deoarece timpul acela nu se pune
  // Citeste tabla dupa formatul nostru
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
  // Ia informatii despre tabla
  memset(&status, 0, sizeof(status));
  getTableStatus(board, &status);
  thisPlayer = status.currentPlayer;
  moveLine = moveCol = -1; // Nu mutam nicaieri, inca
  rebuildPositionalEval2Scores(board);
  score = -INFINIT;
  for ( lin = 0; lin < TABLE_WIDTH; lin++ ) { // Luam toate mutarile la rand
    for ( col = 0; col < TABLE_WIDTH; col++) {
      if ( board[lin][col] == GOL ) {
        board[lin][col] = thisPlayer;
        tempScore = (-minimax(board, DEPTH, -INFINIT, +INFINIT, -thisPlayer, thisPlayer) * 100  // Scorul material +
                    + positionalEval2(lin, col, -thisPlayer)) * 100                             // Potential de extindere al adversarului -
                    - positionalEval2(lin, col, thisPlayer);                                    // Potential de extindere al jucatorului
        board[lin][col] = GOL;
        if ( tempScore > score ) {
          score = tempScore;
          moveLine = lin;
          moveCol  = col;
        }
      }
    }
  }
  board[moveLine][moveCol] = thisPlayer; // Muta

  //Afiseaza
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
    fprintf(stderr, "%lld\n%f", plyCounter, (double)(getTime() - programStart) / 1000000.0f);
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

void rebuildPositionalEval2Scores( char board[TABLE_WIDTH][TABLE_WIDTH] ) {
  int pozitiv, lin, col, i, j, player, sum;

  for (pozitiv = 0; pozitiv <= 1; pozitiv++) {
    for (lin = 0; lin < TABLE_WIDTH; lin++) {
      for (col = 0; col < TABLE_WIDTH; col++) {
        if (board[lin][col] == GOL) {
          player = (pozitiv == 0 ? X : O);
          sum = 0;
          i = lin; // Sus
          while (i > 0 && (board[i - 1][col] == GOL || board[i - 1][col] == player))
            i--;
          sum += lin - i;
          i = lin; // Jos
          while (i < TABLE_WIDTH - 1 && (board[i + 1][col] == GOL || board[i + 1][col] == player))
            i++;
          sum += i - lin;
          j = col; // Stanga
          while (j > 0 && (board[lin][j - 1] == GOL || board[lin][j - 1] == player))
            j--;
          sum += col - j;
          j = col; // Dreapta
          while (j < TABLE_WIDTH - 1 && (board[lin][j + 1] == GOL || board[lin][j + 1] == player))
            j++;
          sum += j - col;
          newPosScore2[player + 1][lin][col] = sum;
        }
      }
    }
  }
}

inline int positionalEval( int lin, int col ) {
  return newPosScore[lin][col];
}

inline int positionalEval2( int lin, int col, char player ) {
  return newPosScore2[player + 1][lin][col];
}

void getTableStatus(char board[TABLE_WIDTH][TABLE_WIDTH],
                                   struct TableStatus* stat ) {
  int scoreX, scoreO;
  int freeTiles;
  int i, j;
  int lenSir = 0; // Lungimea unui sir cu caractere egale
  int xPlies, oPlies;

  xPlies = oPlies = 0;
  scoreX = scoreO = 0;
  freeTiles = 0;

  for ( i = 0; i < TABLE_WIDTH; i++ ) {
    for ( j = 0; j < TABLE_WIDTH; j++ ) {
      if ( j > 0 ) {
        if ( board[i][j - 1] == board[i][j] ) {
          lenSir++;
        } else if ( board[i][j - 1] == X ) {
          scoreX += newMatScore[lenSir];
          lenSir = 1;
        } else if ( board[i][j - 1] == O ) {
          scoreO += newMatScore[lenSir];
          lenSir = 1;
        } else {
          lenSir = 1;
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
    if (board[i][TABLE_WIDTH - 1] == X)
      scoreX += newMatScore[lenSir];
    else if (board[i][TABLE_WIDTH - 1] == O)
      scoreO += newMatScore[lenSir];
    lenSir = 0;
  }
  for ( j = 0; j < TABLE_WIDTH; j++ ) {
    for ( i = 0; i < TABLE_WIDTH; i++ ) {
      if ( i > 0 ) {
        if ( board[i - 1][j] == board[i][j] ) {
          lenSir++;
        } else if ( board[i - 1][j] == X ) {
          scoreX += newMatScore[lenSir];
          lenSir = 1;
        } else if ( board[i - 1][j] == O ) {
          scoreO += newMatScore[lenSir];
          lenSir = 1;
        } else {
          lenSir = 1;
        }
      } else {
        lenSir = 1;
      }
    }
    if (board[TABLE_WIDTH - 1][j] == X)
      scoreX += newMatScore[lenSir];
    else if (board[TABLE_WIDTH - 1][j] == O)
      scoreO += newMatScore[lenSir];
    lenSir = 0;
  }

  stat->scores[O + 1] = scoreO;
  stat->scores[X + 1] = scoreX;
  stat->currentPlayer = (xPlies > oPlies ? O : X);
  stat->freeTiles = freeTiles;
}

// Negamax cu alpha-beta
// http://algopedia.ro/wiki/index.php/Note_de_curs,_clasele_9-10,_22_mai_2014#Alpha-beta
int minimax(char board[TABLE_WIDTH][TABLE_WIDTH], char depth, int alpha, int beta, char player, char computerPlayer) {
  plyCounter++;
  int rc;
  int lin, col;
  int val;
  struct TableStatus status;
  getTableStatus(board, &status);

  if (depth == 0 || status.freeTiles == 0 || getTime() - programStart > STOP_PROGRAM) {
    rc = status.scores[player + 1] - status.scores[-player + 1];
    //printf("%d\t", rc);
    return rc;
  }

  lin = 0;
  while (lin < TABLE_WIDTH && alpha < beta) {
    col = 0;
    while (col < TABLE_WIDTH && alpha < beta) {
      if (board[lin][col] == GOL) {
        board[lin][col] = player;
        val = -minimax(board, depth - 1, -beta, -alpha, -player, computerPlayer);
        alpha = max(alpha, val);
        board[lin][col] = GOL;
      }
      col++;
    }
    lin++;
  }

  rc = min(alpha, beta);
  return rc;
}
