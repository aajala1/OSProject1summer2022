#include <iostream>
#include <iomanip>
#include <array>

#define BOARD_SIZE 3

using namespace std;

char winingTag = 'O';
struct Points
{
  int p1 = 0;
  int p2 = 0;
};

static struct Points pts;

static array<array<char, BOARD_SIZE>, BOARD_SIZE> board = {{'.', '1', '2', '3', '4', '5', '6', '7', '8'}};

bool isValidPlayLocation(int loc);
void displayBoard(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);
void playInLocation(int, array<array<char, BOARD_SIZE>, BOARD_SIZE> &board, char tag);
bool isWinningMove(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);
bool isBoardFull(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);
