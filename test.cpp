#include <iostream>
#include <iomanip>
#include <array>

#define BOARD_SIZE 3

using namespace std;

bool isValidPlayLocation(int loc);
void printBoard(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);
void playInLocation(int, array<array<char, BOARD_SIZE>, BOARD_SIZE> &board, char tag);
bool isWinningMove(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);
bool isBoardFull(array<array<char, BOARD_SIZE>, BOARD_SIZE> &board);

char winingTag = 'O';
struct Points
{
  int p1 = 0;
  int p2 = 0;
};

int main()
{
  static struct Points points;
  const char playTag = 'O';
  int location; // Indicates the position where the player has to place tag on the game board
  array<array<char, BOARD_SIZE>, BOARD_SIZE> board = {{'.', '1', '2', '3', '4', '5', '6', '7', '8'}};

  printf("waiting for named pipes open ... \n");

  printf("named pipes opened and ready\n");

  // player 1: engage first
  while (true)
  {
    printBoard(board);
    printf("Where do you want to play? Select between cells 0 through 8 (Otherwise to quit): ");
    cin >> location;

    if (!cin) // check if user input does not matche an integer value
    {
      cin.clear();
      cout << "You entered a non-digit character." << endl;
      cout << "Quiting appliction" << endl;
      break;
    }
    else
    {
      if (isValidPlayLocation(location))
      {
        playInLocation(location, board, playTag);
        cout << "You have played in " << location << " cell" << endl;
        if (isWinningMove(board))
        {
          if (winingTag == 'O')
          {
            points.p1++;
            cout << "Matching wining condition for player 1" << endl;
          }
          if (winingTag == 'X')
          {
            points.p2++;
            cout << "Matching wining condition for player 2" << endl;
          }
        }
        else
        {
          if (isBoardFull(board))
          {
            cout << "The game has ended in a tie." << endl;
          }
        }
      }
      else
      {
        cout << "Please select a valid location from 0 to 8" << endl;
        printf("Where do you want to play? Select between cells 0 through 8 (Otherwise to quit): ");
        cin >> location;
      }
    }
  }

  return 0;
}

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