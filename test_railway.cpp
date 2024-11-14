#include "lib/railway_lib.h"

#include <fstream>
using namespace std;

vector<string> readTokens(const string &line) {
    istringstream iss(line);
    vector<string> tokens;
    string token;

    while (getline(iss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }

    return tokens;
}

bool is_number(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void file_input() {
    vector<Station<int> > int_stations;
    vector<Station<string> > str_stations;
    ifstream ifile;
    StationBase *st_ptr;

    vector<string> command_types = {"s", "alp", "at", "stat", "chs"};
    ifile.open("input.txt");
    if (!ifile.is_open()) {
        cerr << "Error opening input file" << endl;
        exit(1);
    }
    string line;
    int lineindex = 0;
    int station_index = 0;
    int str_station = 0;
    while (getline(ifile, line)) {
        // cout << line << endl;
        lineindex++;
        auto tokens = readTokens(line);
        int cmd_ind = find(command_types.begin(), command_types.end(), tokens[0]) - command_types.begin();
        // cout << cmd_ind << endl;
        switch (cmd_ind) {
            case 0: {
                cout << "Trying to add station: " << tokens[1] << endl;
                bool isnum = is_number(tokens[1]);
                if (isnum) {
                    int int_station = atoi(tokens[1].c_str());
                    auto *temp_st = new Station<int>(int_station, atoi(tokens[2].c_str()));
                    int_stations.push_back(*temp_st);
                    str_station = 0;
                    station_index = int_stations.size() - 1;
                    st_ptr = &int_stations[station_index];
                } else {
                    Station<string> *temp_st = new Station<string>(tokens[1], atoi(tokens[2].c_str()));
                    str_stations.push_back(*temp_st);
                    str_station = 1;
                    station_index = str_stations.size() - 1;
                    st_ptr = &str_stations[station_index];
                }
            }
            break;
            case 1: {
                string line_name = tokens[1];
                int platform_id = atoi(tokens[2].c_str());
                st_ptr->addLineAndPlatform(line_name, platform_id);
            }
            break;
            case 2: {
                int platform_id = atoi(tokens[1].c_str());
                int time = atoi(tokens[2].c_str());
                bool is_through = (tokens[3].c_str() == "true") ? true : false;
                st_ptr->addTrain(platform_id, time, is_through);
            }
            break;
            case 3: {
                st_ptr->stationStats();
            }
            break;
            case 4: {
                bool is_num = is_number(tokens[1]);
                bool st_found = false;
                if (is_num) {
                    int int_station = atoi(tokens[1].c_str());
                    for (auto &station: int_stations) {
                        if (station.get_station() == int_station) {
                            st_ptr = &station;
                            st_found = true;
                            break;
                        }
                    }
                    if (!st_found) {
                        cerr << "Station not found - " << int_station << endl;
                    }
                } else {
                    string str_station = tokens[1];
                    for (auto &station: str_stations) {
                        if (station.get_station() == str_station) {
                            st_ptr = &station;
                            st_found = true;
                            break;
                        }
                    }
                    if (!st_found) {
                        cerr << "Station not found - " << str_station << endl;
                    }
                }
            }
            break;
            default: {
                cout << "Something unusual in input line " << lineindex << endl;
            }
        }
        cout << "Current Station : ";
        st_ptr->printStationID();
    }
    exit(0);
}

int main() {
    file_input();
    Station st1(10, 10);
    st1.addLineAndPlatform("South", 1);
    st1.addTrain(1, 100, true);
    st1.addLineAndPlatform("NW", 2);
    st1.addTrain(2, 200, true);
    st1.addTrain(1, 120, false);
    st1.addTrain(1, 130, true);
    st1.addTrain(2, 190, false);
    st1.stationStats();
}
