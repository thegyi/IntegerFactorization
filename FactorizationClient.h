//
// Created by thegyi on 2022. 05. 24..
//

#ifndef TASKSERVER_INTERGERFACTORIZATIONCLIENT_H
#define TASKSERVER_INTERGERFACTORIZATIONCLIENT_H

#include <string>
#include "client_http.hpp"

class FactorizationClient {
public:
    FactorizationClient(std::string configFileName);
    ~FactorizationClient();
    std::tuple<std::string, std::string> authenticate();
    std::tuple<std::string, std::string> getNextTask();
    void  sendTaskIsSolved();
    bool solveTask(std::string min, std::string max);
    std::string getPrimeFactor();

private:
    SimpleWeb::Client<SimpleWeb::HTTP>* httpClient;
    std::string host;
    int port;
    std::string authUrl;
    std::string nextTaskUrl;
    std::string solveTaskUrl;
    std::string authId;
    std::string targetNumber;
    std::string primeFactor;
};


#endif//TASKSERVER_INTERGERFACTORIZATIONCLIENT_H
