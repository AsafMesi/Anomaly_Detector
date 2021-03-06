//
// Created by DanMa on 30/12/2021.
//
#include "CLI.h"
#include <vector>
#define TERMINATE_KEY_1 "6"
#define TERMINATE_KEY_2 "6\n"

using namespace std;

static void getMenu(vector<string> &menu){
    menu.emplace_back("Welcome to the Anomaly Detection Server.\n");
    menu.emplace_back("Please choose an option:\n");
    menu.emplace_back("1.upload a time series csv file\n");
    menu.emplace_back("2.algorithm settings\n");
    menu.emplace_back("3.detect anomalies\n");
    menu.emplace_back("4.display results\n");
    menu.emplace_back("5.upload anomalies and analyze results\n");
    menu.emplace_back("6.exit\n");
}

static void writeMenu(DefaultIO* dio, vector<string> &menu){
    for(auto &opt: menu){
        dio->write(opt);
    }
}

void CLI::start(){
    auto* cdb = new CommandsDataBase();
    commandFactory cf(this->dio);

    vector<string> menu;
    getMenu(menu);

    string key;
    Command* c;
    while(true) {
        writeMenu(this->dio, menu);
        key = dio->read();
        if (key == TERMINATE_KEY_1 || key == TERMINATE_KEY_2) {
            break;
        }
        c = cf.getCommand(key);
        c->execute(cdb);
    }
}

CLI::~CLI() = default;

