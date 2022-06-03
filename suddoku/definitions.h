#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>


using namespace std;
namespace fs = std::filesystem;

int getUnsolvedFields(int arr[9][9]);
bool isSafe(int grid[9][9], int row, int col, int num);
bool isInputNum(std::string input);
void readMapFromFile(std::string path, int grid[9][9]);
bool saveMap(std::string path, int grid[9][9]);
bool solveSuduko(int _grid[9][9], int row, int col);
void Initialize();