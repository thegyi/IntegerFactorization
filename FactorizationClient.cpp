//
// Created by thegyi on 2022. 05. 24..
//

#include "FactorizationClient.h"

#include "integer.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <nbtheory.h>
#include <osrng.h>

FactorizationClient::FactorizationClient(std::string configFileName) {
    std::ifstream configFile (configFileName);
    std::string fileContent((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    nlohmann::json rootItem = nlohmann::json::parse(fileContent);
    host = rootItem["server"]["host"].get<std::string>();
    port = rootItem["server"]["port"].get<int>();
    authUrl = rootItem["task"]["init"].get<std::string>();
    nextTaskUrl = rootItem["task"]["next"].get<std::string>();
    solveTaskUrl = rootItem["task"]["solve"].get<std::string>();
    httpClient = new SimpleWeb::Client<SimpleWeb::HTTP>(host+":"+std::to_string(port));
}

FactorizationClient::~FactorizationClient() {
    delete httpClient;
}

std::tuple<std::string, std::string> FactorizationClient::authenticate() {
    auto r1 = httpClient->request("GET", authUrl);
    std::string response = r1->content.string();
    std::cout << "response from authenticate:"  <<response << std::endl;
    nlohmann::json rootItem = nlohmann::json::parse(response);
    authId = rootItem["client"].get<std::string>();
    targetNumber = rootItem["number"].get<std::string>();
    std::tuple<std::string, std::string> result = std::make_tuple(authId,targetNumber);
    primeFactor="";
    return result;
}

std::tuple<std::string, std::string> FactorizationClient::getNextTask() {
    auto r1 = httpClient->request("GET", nextTaskUrl+"?clientId="+authId);
    std::string response = r1->content.string();
    std::cout << "response from getnexttask:"  <<response << std::endl;
    nlohmann::json rootItem = nlohmann::json::parse(response);
    std::string min = rootItem["min"].get<std::string>();
    std::string max = rootItem["max"].get<std::string>();
    std::tuple<std::string, std::string> result = std::make_tuple(min,max);
    return result;
}

bool FactorizationClient::solveTask(std::string min, std::string max) {
    CryptoPP::Integer minInt(min.c_str());
    CryptoPP::Integer maxInt(max.c_str());
    CryptoPP::Integer targetInt(targetNumber.c_str());

    CryptoPP::BlockingRng rng;

    if(maxInt.IsEven()) {
        maxInt -= 1;
    }
    CryptoPP::Integer factor = maxInt;
    int primeCounter = 0;
    std::cout <<std::endl;
    bool findFactor = false;
    while(factor >= minInt){
        if(CryptoPP::VerifyPrime(rng, factor)) {
            std::cout << std::setfill('0') << std::setw(13) << ++primeCounter << " prime found" << '\r';
            if(targetInt.Modulo(factor).IsZero()) {
                std::cout << "FACTOR" << std::endl;
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<std::endl;
                std::cout << factor << std::endl;
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<std::endl;
                findFactor = true;
                break;
            }
        }
        factor -= 2;
    }
    std::stringstream ss;
    ss << factor;
    primeFactor = ss.str();
    return findFactor;
}

void FactorizationClient::sendTaskIsSolved() {
    auto r1 = httpClient->request("GET", solveTaskUrl+"?clientId="+authId+"&primeFactor="+primeFactor);
}

std::string FactorizationClient::getPrimeFactor() {
    return primeFactor;
}

int main()
{
    FactorizationClient client("/home/thegyi/projects/CLionProjects/TaskServer/config/factorizationClient.json");
    std::tuple<std::string, std::string> authResponse = client.authenticate();
    std::cout << std::get<0>(authResponse) << " " <<  std::get<1>(authResponse) << std::endl;
    std::tuple<std::string, std::string> taskResponse = client.getNextTask();
    std::cout << std::get<0>(taskResponse) << " " <<  std::get<1>(taskResponse) << std::endl;
    while(client.solveTask(std::get<0>(taskResponse), std::get<1>(taskResponse)) == false) {
        taskResponse = client.getNextTask();
    }
    client.sendTaskIsSolved();
    return 0;
}