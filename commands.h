//
// Created by DanMa on 30/12/2021.
//

#ifndef ANOMALY_DETECTION_UTIL_CPP_COMMANDS_H
#define ANOMALY_DETECTION_UTIL_CPP_COMMANDS_H



#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "timeseries.h"
#include "map"

using namespace std;

typedef struct compactAnomaly{
    int start;
    int end;
    bool TP = false;
    string des;

    compactAnomaly(int s, int e){
        this->start = s;
        this->end = e;
        this->TP = false;
        this->des = "";
    }
}compactAnomaly;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO()= default;

    // you may add additional methods here
    virtual void close() = 0;
    void readAndCreate(string fileName, const char* delim){
        ofstream out(fileName);
        string row;
        while ((row = read())!= delim){
            out << row << endl;
        }
        out.close();
    }

};

// you may add here helper classes
typedef struct CommandsDataBase {
    TimeSeries *train{};
    TimeSeries *test{};
    HybridAnomalyDetector had;
    float threshold = 0.9;
    vector<AnomalyReport> reports;
    vector<compactAnomaly> compactReports;
} CommandsDataBase;

// you may edit this class
class Command{
public:
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute(CommandsDataBase* cdb)=0;
    virtual ~Command(){}

protected:
    DefaultIO* dio;
};

class uploadCSVCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit uploadCSVCommand(DefaultIO* dio) : Command(dio){};
};

class updateThresholdCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit updateThresholdCommand(DefaultIO* dio) : Command(dio){};
};

class anomalyDetectionCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit anomalyDetectionCommand(DefaultIO* dio) : Command(dio){};
    void generateCompactReports(CommandsDataBase* cdb, vector<compactAnomaly>& ca);
};

class printAnomalyCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit printAnomalyCommand(DefaultIO* dio) : Command(dio){};
};

class countAnomalyCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit countAnomalyCommand(DefaultIO *dio) : Command(dio){};
    void generateAnomaliesVector(vector<compactAnomaly> &vec);

};

class terminateCommand : public Command{
    void execute(CommandsDataBase* cdb) override;
public:
    explicit terminateCommand(DefaultIO* dio) : Command(dio){};
};

class commandFactory {
    map<string, Command*> cm;
    DefaultIO* dio;
public:
    commandFactory(DefaultIO* dio) : dio(dio){};
    Command* getCommand(string& key);
};
#endif /* COMMANDS_H_ */
#endif //ANOMALY_DETECTION_UTIL_CPP_COMMANDS_H
