//
// Created by thegyi on 2022. 05. 21..
//

#ifndef TASKSERVER_INTEGERFACTORIZATIONTASK_H
#define TASKSERVER_INTEGERFACTORIZATIONTASK_H

#include <string>
#include "integer.h"

class IntegerFactorizationTask {
private:
    CryptoPP::Integer number;
    CryptoPP::Integer interval;
    CryptoPP::Integer lastPackage;
    CryptoPP::Integer countOfPackages;
public:
    IntegerFactorizationTask();
    IntegerFactorizationTask(std::string number, std::string interval);
    std::string getNextTask();
    std::tuple<std::string, std::string> getInitTask();
    void load(std::string fileName);
    void save(std::string fileName);
    std::string getLastPackage();
    std::string getTotalPackages();
};


#endif//TASKSERVER_INTEGERFACTORIZATIONTASK_H
