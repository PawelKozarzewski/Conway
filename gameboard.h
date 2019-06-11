#ifndef CONWAY_BOARD_H
#define CONWAY_BOARD_H

#include <iostream>
#include <time.h>
#include <vector>
#include <windows.h>
#include <math.h>
#include <string>

#include "settings.h"

using namespace std;

class Game_board {
    public:
        Settings settings_board;
        //stats
        int starting_cells, alive_cells, dead_cells;
        long long total_births, total_deaths;
        long generation;
        //settings
        double speed_multiplier;
        int display_style;
        long number_of_generations;

    private:
        int height, width, size_rs, size_rb, periodic;
        double lower_percentage, upper_percentage;
        int* rules_survival;
        int* rules_birth;
        int** board;
        int** new_board;
        char** screenBufferArray;


    public:
        Game_board(Settings);
        ~Game_board();
        void randomize();
        void load_initial_state_from_file(string);
        void save_state_to_file(string);
        void update();
        int count_neighbors(int, int);
        void reset_stats();
        void print();
};

Game_board::Game_board(Settings s) {
    starting_cells = 0;
    alive_cells = 0;
    dead_cells = 0;
    total_births = 0;
    total_deaths = 0;
    generation = 0;

    settings_board = s;

    for(int i=0; i<settings_board.settings.size(); i++) {
        string option = settings_board.settings[i][0];
        string value = settings_board.settings[i][1];

        if(option.compare("height") == 0) {
            height = atoi(value.c_str());
        }
        else if(option.compare("width") == 0) {
            width = atoi(value.c_str());
        }
        else if(option.compare("lower_p") == 0) {
            lower_percentage = atof(value.c_str());
        }
        else if(option.compare("upper_p") == 0) {
            upper_percentage = atof(value.c_str());
        }
        else if(option.compare("rules_survival") == 0) {
            size_rs = value.length();
            rules_survival = new int[size_rs];

            for(int k=0; k<value.length(); k++)
                rules_survival[k] = (int)value[k] - 48;
        }
        else if(option.compare("rules_birth") == 0) {
            size_rb = value.length();
            rules_birth = new int[size_rb];

            for(int k=0; k<value.length(); k++)
                rules_birth[k] = (int)value[k] - 48;
        }
        else if(option.compare("number_of_generations") == 0) {
            number_of_generations = atoi(value.c_str());
        }
        else if(option.compare("speed_multiplier") == 0) {
            speed_multiplier = atof(value.c_str());
        }
        else if(option.compare("periodic") == 0) {
            periodic = atoi(value.c_str());
        }
        else if(option.compare("display_style") == 0) {
            display_style = atoi(value.c_str());
        }
    }

    board = new int*[height];
    for(int i = 0; i<height; i++) {
        board[i] = new int[width];
    }

    new_board = new int*[height];
    for(int i = 0; i<height; i++) {
        new_board[i] = new int[width];
    }

    screenBufferArray = new char*[height+2];
    for (int i = 0; i<height+2; i++) {
        screenBufferArray[i] = new char[2*width + 1];
    }
}

Game_board::~Game_board() {
    delete[] rules_survival;
    delete[] rules_birth;

    for(int i = 0; i<height; i++) {
        delete[] board[i];
    }
    delete[] board;

    for(int i = 0; i<height; i++) {
        delete[] new_board[i];
    }
    delete[] new_board;

    for(int i = 0; i<height; i++) {
        delete[] screenBufferArray[i];
    }
    delete[] screenBufferArray;
}

void Game_board::randomize() {
    srand(time(NULL));
    int total = (height)*(width);

    double tmp_l = lower_percentage * 0.01 * total;
    double tmp_u = upper_percentage * 0.01 * total;
    int low = round(tmp_l);
    int up = round(tmp_u);
    int number_of_cells = rand() % (up-low+1) + low;
    starting_cells = number_of_cells;

    for(int x=0; x<height; x++)
        for(int y=0; y<width; y++)
            board[x][y] = 0;

    vector <int> all_coords;
    for(int i=0; i<total; i++) {
        all_coords.push_back(i);
    }

    int x, y;
    for(int i=0; i<number_of_cells; i++) {
        int coords =  rand() % all_coords.size();
        x = all_coords[coords] / (width);
        y = all_coords[coords] % (width);
        board[x][y] = 1;
        all_coords.erase(all_coords.begin() + coords);
    }
}

void Game_board::load_initial_state_from_file(string filename) {
    vector<vector<string>> loaded_board;
    string line;
    ifstream file(filename);
    if(file.is_open()) {
        while(getline(file, line)) {
            vector <string> board_row;
            string word = "";

            for(int i=0; i<line.length(); i++) {
                if(line[i] == ',') {
                    board_row.push_back(word);
                    word = "";
                }
                else {
                    word += line[i];
                }
            }
            board_row.push_back(word);
            loaded_board.push_back(board_row);
        }
        file.close();
    }
    else {
        cout << "Could not load the initial state" << endl;
        return;
    }

    for(int i = 0; i<height; i++) {
        delete[] board[i];
    }
    delete[] board;

    for(int i = 0; i<height; i++) {
        delete[] new_board[i];
    }
    delete[] new_board;

    for(int i = 0; i<height; i++) {
        delete[] screenBufferArray[i];
    }
    delete[] screenBufferArray;

    height = loaded_board.size();
    width = loaded_board[0].size();

    board = new int*[height];
    for(int i = 0; i<height; i++) {
        board[i] = new int[width];
    }

    new_board = new int*[height];
    for(int i = 0; i<height; i++) {
        new_board[i] = new int[width];
    }

    screenBufferArray = new char*[height+2];
    for (int i = 0; i<height+2; i++) {
        screenBufferArray[i] = new char[2*width + 1];
    }

    for(int x=0; x<loaded_board.size(); x++) {
        for(int y=0; y<loaded_board[x].size(); y++) {
            if(loaded_board[x][y].compare("0")) {
                board[x][y] = 1;
            }
            else if(loaded_board[x][y].compare("1")) {
                board[x][y] = 0;
            }
        }
    }

    cout << "Initial state loaded.\n";
}

void Game_board::save_state_to_file(string savename) {
    string tmp_buffer;

    for(int x=0; x<height-1; x++) {
        for(int y=0; y<width-1; y++) {
            tmp_buffer += to_string(board[x][y]);
            tmp_buffer += ",";
        }
        tmp_buffer += to_string(board[x][width-1]);
        tmp_buffer += "\n";
    }

    for(int y=0; y<width-1; y++) {
            tmp_buffer += to_string(board[height-1][y]);
            tmp_buffer += ",";
    }
    tmp_buffer += to_string(board[height-1][width-1]);

    ofstream file(savename);
    if(file.is_open()) {
        file << tmp_buffer;
        file.close();
        cout << "Saved!\n";
    }
    else {
        cout << "Could not save" << endl;
        return;
    }
}

void Game_board::update() {
    generation++;

    //initializing a board for next generation
    for(int x=0; x<height; x++)
        for(int y=0; y<width; y++)
            new_board[x][y] = 0;

    //determining states of all cells in the next generation
    alive_cells = 0;
    dead_cells = 0;
    for(int x=0; x<height; x++) {
        for(int y=0; y<width; y++) {

            bool is_alive_next_generation = false;
            int neighbors = count_neighbors(x, y);

            if(board[x][y] == 1) {
                if(rules_survival[0] != 9) {
                    for(int i=0; i<size_rs; i++) {
                        if(neighbors == rules_survival[i]) {
                            is_alive_next_generation = true;
                            break;
                        }
                    }
                }
            }
            else {
                if(rules_birth[0] != 9) {
                    for(int i=0; i<size_rb; i++) {
                        if(neighbors == rules_birth[i]) {
                            total_births++;
                            is_alive_next_generation = true;
                            break;
                        }
                    }
                }
            }

            if(is_alive_next_generation == true) {
                new_board[x][y] = 1;
                alive_cells++;
            }
            else {
                new_board[x][y] = 0;
                total_deaths++;
            }
        }
    }
    dead_cells = height*width - alive_cells;

    int** tmp_p = board;
    board = new_board;
    new_board = tmp_p;
}

int Game_board::count_neighbors(int x, int y) {
    int number_of_neighbors = 0;

    if(periodic == 0) {
        for(int i=max(0, x-1); i<=min(height-1, x+1); i++)
            for(int j=max(0, y-1); j<=min(width-1, y+1); j++)
                number_of_neighbors += board[i][j];
    }
    else if(periodic == 1) {
        for(int i=x-1; i<= x+1; i++)
            for(int j=y-1; j<=y+1; j++)
                number_of_neighbors += board[(i+height)%height][(j+width)%width];
    }

    if(board[x][y] == 1)
        number_of_neighbors -= 1;

    return number_of_neighbors;
}

void Game_board::reset_stats() {
    starting_cells = 0;
    alive_cells = 0;
    dead_cells = 0;
    total_births = 0;
    total_deaths = 0;
    generation = 0;
}

void Game_board::print() {
    char NULL_C, DEAD_CELL, ALIVE_CELL, NE_CORNER, SW_CORNER, SE_CORNER, NW_CORNER, H_BORDER, V_BORDER;

    switch(display_style) {
        case 0: {
            NULL_C = 0;
            DEAD_CELL = 46;
            ALIVE_CELL = 79;
            NE_CORNER = 43;
            SW_CORNER = 43;
            SE_CORNER = 43;
            NW_CORNER = 43;
            H_BORDER = 45;
            V_BORDER = 124;
            break;
        }

        case 1: {
            NULL_C = 0;
            DEAD_CELL = 250;
            ALIVE_CELL = 79;
            NE_CORNER = 187;
            SE_CORNER = 188;
            SW_CORNER = 200;
            NW_CORNER = 201;
            V_BORDER = 186;
            H_BORDER = 205;

            //change color of background in console
            HANDLE  hConsole;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 0x70);
            break;
        }
    }

    //emptying screenBufferArray
    for(int x=0; x<height+2; x++)
        for(int y=0; y<2*width+1; y++)
            screenBufferArray[x][y] = NULL_C;

    //corners
    screenBufferArray[0][2*width] = NE_CORNER;
    screenBufferArray[height+1][2*width] = SE_CORNER;
    screenBufferArray[height+1][0] = SW_CORNER;
    screenBufferArray[0][0] = NW_CORNER;

    //vertical borders
    for(int i=1; i<height+1; i++) {
        screenBufferArray[i][0] = V_BORDER;
        screenBufferArray[i][2*width] = V_BORDER;
    }

    //horizontal borders
    for(int i=1; i<2*width; i++) {
        screenBufferArray[0][i] = H_BORDER;
        screenBufferArray[height+1][i] = H_BORDER;
    }

    //cells
    for(int x=0; x<height; x++) {
        for(int y=0; y<width; y++) {
            if(board[x][y] == 1) {
                screenBufferArray[x+1][2*y + 1] = ALIVE_CELL;
            }
            else {
                screenBufferArray[x+1][2*y + 1] = DEAD_CELL;
            }
        }
    }

    //buffer the entire board as string to avoid flickering in console
    string screenBuffer = "";
    for(int x=0; x<height+2; x++) {
        for(int y=0; y<2*width+1; y++) {
            screenBuffer += screenBufferArray[x][y];
        }
        screenBuffer += "\n";
    }

    //set cursor position to the top left and draw the string
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(output, pos);
    cout << screenBuffer;
}

#endif
