#include "definitions.h"

void readMapFromFile(std::string path, int grid[9][9]) {
    string myText;
    // Read from the text file
    ifstream MyReadFile(path);
    int _grid[9][9];
    int line = -1;
    // Use a while loop together with the getline() function to read the file line by line

    if (!MyReadFile.good()) {
        printf("Map couldnt be found.\n");
        cout << path << endl;
        system("pause");
        exit(0);
    }

    while (std::getline(MyReadFile, myText)) {
        line++;
        if (isInputNum(myText)) {

            if (myText.length() == 9) {
                for (int i = 0; i < 9; i++) {
                    if (myText.at(i) < 0 || myText.at(i) > 9) {

                        _grid[line][i] = atoi(std::string(1, myText.at(i)).c_str());


                    }
                }
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = _grid[i][j];
        }
    }
    if (!solveSuduko(_grid, 0, 0)) {
        printf("Map impossible to finish or malformated.\n");
        system("pause");
        exit(0);
    }

    // Close the file
    MyReadFile.close();

}


bool FindUnassignedLocation(int grid[9][9], int& row, int& col) {
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (grid[row][col] == 0)
                return true;
    return false;
}

bool UsedInRow(int grid[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

bool UsedInCol(int grid[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}


int getUnsolvedFields(int arr[9][9]) {
    int temp = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++) {
            if (arr[i][j] == 0) {
                temp++;
            }
        }
    }

    return temp;
}


bool saveMap(string path, int grid[9][9]) {
    ofstream MyFile(path);

    if (!MyFile.good() || MyFile.bad())
        return false;

    for (int i = 0; i < 9; i++) {
        if (i != 0)
            MyFile << "\n";
        for (int j = 0; j < 9; j++) {

            MyFile << grid[i][j];
        }
    }

    // Close the file
    MyFile.close();
    return true;
}

bool UsedInBox(int grid[9][9], int boxStartRow,
    int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow]
                [col + boxStartCol] ==
                num)
                return true;
    return false;
}

bool isSafe(int grid[9][9], int row, int col, int num) {

    return !UsedInRow(grid, row, num)
        && !UsedInCol(grid, col, num)
        && !UsedInBox(grid, row - row % 3,
            col - col % 3, num)
        && grid[row][col] == 0;
}

bool isInputNum(string input) {
    return !input.empty() && std::find_if(input.begin(),
        input.end(), [](unsigned char c) { return !std::isdigit(c); }) == input.end();
}
bool solveSuduko(int _grid[9][9], int row, int col)
{
    if (row == 9 - 1 && col == 9)
        return true;

    if (col == 9) {
        row++;
        col = 0;
    }
    if (_grid[row][col] > 0)
        return solveSuduko(_grid, row, col + 1);

    for (int num = 1; num <= 9; num++)
    {

        if (isSafe(_grid, row, col, num))
        {
            _grid[row][col] = num;

            if (solveSuduko(_grid, row, col + 1))
                return true;
        }

        _grid[row][col] = 0;
    }
    return false;
}