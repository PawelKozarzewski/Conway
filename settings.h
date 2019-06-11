#ifndef CONWAY_SETTINGS_H
#define CONWAY_SETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Settings {
    public:
        vector<vector<string>> settings;

    public:
        Settings();
        void initialize();
        void print();
    private:
        void split(string&, string&, string&, char);
        void generate_default_settings();
        void load_settings();

};

Settings::Settings() {
}

void Settings::initialize() {
    if(ifstream("settings.txt").good()) {
        load_settings();
    }
    else {
        cout << "No settings.txt file found" << endl;
        generate_default_settings();
        load_settings();
    }
}

void Settings::split(string& line, string& s_name, string& value, char delimiter='=') {
    bool second_word = false;
    for(int i=0; i<line.length(); i++) {
        if(line[i] == delimiter) {
            second_word = true;
            continue;
        }

        if(second_word == true)
            value += line[i];
        else
            s_name += line[i];
    }
}

void Settings::generate_default_settings() {
    cout << "Generating a file with default settings..." << endl;

    string default_settings = "height=25\n"
                              "width=25\n"
                              "lower_p=25\n"
                              "upper_p=30\n"
                              "rules_survival=23\n"
                              "rules_birth=3\n"
                              "number_of_generations=10000\n"
                              "speed_multiplier=1\n"
                              "periodic=0\n"
                              "display_style=0";

    ofstream file("settings.txt");
    if(file.is_open()) {
        file << default_settings;
        file.close();
    }

    cout << "Done!" << endl;
}

void Settings::load_settings() {
    cout << "Loading settings..." << endl;

    string line;
    ifstream file("settings.txt");
    if(file.is_open()) {
        while(getline(file, line)) {
            string word_1="", word_2="";
            split(line, word_1, word_2, '=');

            vector <string> option;
            option.push_back(word_1);
            option.push_back(word_2);

            settings.push_back(option);
        }
        file.close();
    }
    else {
        cout << "Could not load the settings" << endl;
        return;
    }
}

void Settings::print() {
    cout << "\nSettings:" << endl;
    for(int i=0; i<settings.size(); i++) {
        cout << "\t- " << settings[i][0] << " = " << settings[i][1] << endl;
    }
}

#endif
