#include "definitions.h"

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);


static bool gameStarted;

int currField[2];

int skipCount;

bool doDraw;

bool loadedSaveFile;

int grid[9][9];

string saveGamePath;

string executablePath;

void print(int arr[9][9])
{
    int current = 0;
    bool highlighted = false;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++) {
        if (j == 2 || j == 5) {
            if (arr[i][j] == 0) {

                if (!highlighted && current == skipCount)
                    SetConsoleTextAttribute(hStdout, 23);

                cout << arr[i][j];
                if (!highlighted && current == skipCount) {
                    SetConsoleTextAttribute(hStdout, 7);
                    currField[0] = i;
                    currField[1] = j;
                    highlighted = true;
                }
                cout << " | ";

                current++;
            }
            else {
                cout << arr[i][j] << " | ";

            }
        }
        else {

        if (arr[i][j] == 0) {

            if (!highlighted && current == skipCount)
                SetConsoleTextAttribute(hStdout, 23);


            cout << arr[i][j];
            if (!highlighted && current == skipCount) {
                SetConsoleTextAttribute(hStdout, 7);
                currField[0] = i;
                currField[1] = j;
                highlighted = true;
            }
            cout << " ";
            current++;
        }
        else {
                cout << arr[i][j] << " ";

            }
        }

        }
        if (i == 2 || i == 5)
            cout << endl << "------+-------+------";
        cout << endl;
    }
    cout << endl;
}


void getInput() {

    std::string input;

    if (!gameStarted) {
        cin >> input;
        if (input == "S" || input == "s") {
            readMapFromFile("map.txt", grid);
            doDraw = true;
            gameStarted = true;
        }
        else if (input == "L" || input == "l") {
            TCHAR buffer[MAX_PATH] = { 0 };
            GetModuleFileName(NULL, buffer, MAX_PATH);
            

            std::filesystem::path cwd = std::filesystem::current_path();
            std::string path = cwd.string() + "\\saves";

            int counter = 0;
            for (const auto& entry : fs::directory_iterator(path)) {
                counter++;
                std::cout << counter << ": " << entry.path().filename() << std::endl;
            }
            bool error = true;
            while (error) {
                string input;
                cout << "Choose savefile: ", cin >> input;

                if (isInputNum(input) && atoi(input.c_str()) <= counter && atoi(input.c_str()) >= 1) {

                    int _counter = 0;
                    for (const auto& entry : fs::directory_iterator(path)) {
                        _counter++;
                      
                        if (atoi(input.c_str()) == _counter) {
                            readMapFromFile("saves/" + entry.path().filename().string(), grid);
                            saveGamePath = entry.path().filename().string();
                            error = false;
                            doDraw = true;
                            gameStarted = true;
                        }
                    }
                }
            }
        }
        else {
            getInput();
        }

    }


    DWORD events;
    INPUT_RECORD buffer;
    PeekConsoleInput(hStdin, &buffer, 1, &events);


    if (events > 0)
    {
        ReadConsoleInput(hStdin, &buffer, 1, &events);
        if (buffer.Event.KeyEvent.wVirtualKeyCode == VK_TAB) { // Next available field
            if (buffer.Event.KeyEvent.bKeyDown) {
                int unsolvedFields = getUnsolvedFields(grid);

                if (skipCount < unsolvedFields - 1) {
                    skipCount += 1;
                }
                else
                {
                    skipCount = 0;
                }
            }
        }
        else { // Number input
            keybd_event((BYTE)buffer.Event.KeyEvent.wVirtualKeyCode, 0, 0, 0); //ugly hack, works tho
            keybd_event((BYTE)buffer.Event.KeyEvent.wVirtualKeyCode, 0, KEYEVENTF_KEYUP, 0); // Tell the compiler we know what we're doing

            cin >> input;
            if (isInputNum(input)) {
                int num = atoi(input.c_str());

                if (num >= 1 && num <= 9)
                    if (isSafe(grid, currField[0], currField[1], num)) {
                        grid[currField[0]][currField[1]] = num;
                    }
            }
            else {
                if (input == "Q" || input == "q") {
                    exit(0);
                }
                else if (input == "S" || input == "s") {
                    //Save
                    bool error = true;
                    
                    while (error) {
                        string saveName;
                        cout << "Save as (\"cancel\" to cancel): ", cin >> saveName;
                        

                        if (saveName.find("\\") != string::npos || saveName.find("/") != string::npos || saveName.find(":") != string::npos || saveName.find("*") != string::npos || saveName.find("?") != string::npos || saveName.find("\"") != string::npos || saveName.find("<") != string::npos || saveName.find(">") != string::npos || saveName.find("|") != string::npos) {
                            printf("Name contains forbidden characters.\n");
                            continue;
                        }
                        if (saveName != "cancel") {
                            if (!saveMap("saves/" + saveName + ".txt", grid)) {
                                printf("Failed to save.\n");
                                continue;
                            }
                            error = false;
                        }
                        else {
                            error = false;
                        }
                    }

                }
                else if (input == "R" || input == "r") {
                    //Restart
                    if (!loadedSaveFile) {
                        readMapFromFile("map.txt", grid);
                    }
                    else {
                        //Load saved game
                        readMapFromFile("saves/" + saveGamePath, grid);
                    }
                }
                doDraw = true;
                return;
            }
            
        }
        doDraw = true;
    }
    
}


int main(int argc, char** argv) {
    system("title Sudoku");

    Initialize();

    while (true) {
   
 
        if (doDraw) {
            system("cls");
            printf("[R]estart\n[S]ave\n\n[Q]uit\n\n");
            print(grid);

            if (getUnsolvedFields(grid) == 0) {
                printf("Game won!\n");
                Initialize();
                continue;
            }
            cout << "Move: ";
            doDraw = false;
        }


        getInput();

       Sleep(50);
    }
    return 0;

}

void Initialize() {
    
    printf("Sudoku\n\n");

    loadedSaveFile = false;
    gameStarted = false;
    doDraw = false;
    skipCount = 0;

    printf("[S]tart\n[L]oad\n\n[Q]uit\n\n");
    getInput();
}