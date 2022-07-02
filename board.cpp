#ifndef board
#define board

#include <iostream>
#include <iomanip>
#include <array>

#define BOARD_SIZE 3

using namespace std;

bool isValidPlayLocation(int location)
{
  return (location < 0 || location > 8) ? false : true;
}

void printBoard(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board)
{
  cout << "+-+-+-+" << endl;
  for (auto const &row : board)
  {
    for (auto const &entry : row)
    {
      cout << "|" << entry;
    }
    cout << "|" << endl
         << "+-+-+-+" << endl;
  }
}

/** 
 * Play user action on the board
 * @param loc position or location the user desires to play
 * @param board reference to the active board.
 * */

void playInLocation(int loc, array<array<char, BOARD_SIZE>, BOARD_SIZE> &board, char tag)
{
  int row = (int)loc / BOARD_SIZE;
  int col = (int)loc % BOARD_SIZE;

  if (board[row][col] == 'O' || board[row][col] == 'X')
  {
    cout << "That location is used already. Try something else" << endl;
    return;
  }
  board[row][col] = tag;
}

// check validation criteria
bool isWinningMove(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board)
{
  // check for rows
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
    {
      winingTag = board[i][0];
      return true;
    }
  }

  // check for columns
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
    {
      winingTag = board[0][i];
      return true;
    }
  }

  // check for diagonals
  if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
  {
    winingTag = board[0][0];
    return true;
  }

  if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
  {
    winingTag = board[0][2];
    return true;
  }

  return false;
}

bool isBoardFull(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board)
{
  for (auto const &row : board)
  {
    for (auto const &entry : row)
    {
      if (entry != 'O' || entry != 'X')
        return false;
    }
  }
  return true;
}

#endif