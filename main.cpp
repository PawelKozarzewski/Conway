#include <iostream>
#include <fstream>
#include <sstream>

#include "settings.h"
#include "gameboard.h"

using namespace std;

void print_file_content(string filename) {
    ifstream file(filename);
    string file_content = "";
    string line;

    if(file.is_open()) {
        while(getline(file, line)) {
            file_content += line;
            file_content += "\n";
        }
        file.close();
    }
    else {
        return;
    }

    cout << file_content;
}

string format(string str, int n, char c=0) {
    string s = "";
    if(n > str.length())
        for(int i=str.length(); i<n; i++)
            s += c;
    return str+=s;
}

string generate_simulation_info(int initial, int generation, int total_b, int total_d, int alive, int dead) {
    string simulation_info = "\n";
    string ic="", g="", a="", tb="", d="", td="";

    string border = format("+", 92, '-');
    border += "+";

    g = format(("Generation:    " + to_string(generation)), 30);
    a = format(("Alive: " + to_string(alive)), 30);
    tb = format(("Total Births: " + to_string(total_b)), 30);

    ic = format(("Initial Cells: " + to_string(initial)), 30);
    d = format(("Dead:  " + to_string(dead)), 30);
    td = format(("Total Deaths: " + to_string(total_d)), 30);

    simulation_info += border + "\n| " + g + a + tb + "|\n| " + ic + d + td + "|\n" + border;

    return simulation_info;
}

void countdown() {
    for(int i=3; i>=1; i--) {
        cout << i << "... ";
        Sleep(1000);
    }
    cout << "GO!";
}

void run_simulation(Game_board& gb, bool ctd) {
    //print initial state
    system("CLS");
    gb.print();
    string simulation_info = "";
    simulation_info = generate_simulation_info(gb.starting_cells, gb.generation, gb.total_births, gb.total_deaths, gb.alive_cells, gb.dead_cells);
    cout << simulation_info << endl;

    double speed = 100;
    if(gb.speed_multiplier == 0)
        speed = 31536000;
    else
        speed /= gb.speed_multiplier;

    if(ctd == true)
        countdown();

    //simulation
    char pause;
    for(int i=0; i<gb.number_of_generations; i++) {
        gb.update();
        gb.print();

        simulation_info = "";
        simulation_info = generate_simulation_info(gb.starting_cells, gb.generation, gb.total_births, gb.total_deaths, gb.alive_cells, gb.dead_cells);
        cout << simulation_info;

        Sleep(speed);
    }
    cout << "\nEVOLUTION FINISHED!\n";
}

int main() {
    print_file_content("title_art.txt");
    Settings s;
    s.initialize();
    s.print();
    cout << endl;

    Game_board gb(s);

    bool load_from_file = false;
    string filename = "";
    cout << "Load initial state from file? [y/n]: ";
    char lyn;
    cin >> lyn;
    if(lyn == 'y') {
        cout << "Path to file: ";
        cin >> filename;

        if(ifstream(filename).good()) {
            gb.load_initial_state_from_file(filename);
            load_from_file = true;
        }
        else {
            cout << filename << " not found" << endl;
            load_from_file = false;
        }

    }

    cout << "Start? [y/n]: ";
    char syn;
    cin >> syn;


    if(syn == 'y') {
        if(load_from_file == true)
            gb.load_initial_state_from_file(filename);
        else
            gb.randomize();

        bool ctd = true;
        for(int i=0; i<1; i++) {


            run_simulation(gb, ctd);

            cout << "Restart[r], Continue[c], Save&Exit[s], Exit[e]: ";
            char rce;
            cin >> rce;

            if(rce == 'r') {
                ctd = true;
                gb.reset_stats();

                if(load_from_file == true)
                    gb.load_initial_state_from_file(filename);
                else
                    gb.randomize();

                i--;
            }
            else if(rce == 'c') {
                ctd = false;
                i--;
            }
            else if(rce == 's') {
                string fn = "";
                cout << "Save path: ";
                cin >> fn;
                gb.save_state_to_file(fn);
                cout << "Exiting...\n";
                return 0;
            }
            else if(rce == 'e') {
                cout << "Exiting...\n";
                return 0;
            }
        }
    }
    else if(syn == 'n') {
        cout << "Exiting...\n";
        return 0;
    }
}
