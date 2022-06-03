//
// Created by thegyi on 2022. 05. 21..
//

#include <fstream>
#include "nlohmann/json.hpp"
#include "IntegerFactorizationTask.h"
#include "integer.h"
#include <iostream>
#include "uuid.h"

IntegerFactorizationTask::IntegerFactorizationTask():number("1"), interval("1"),lastPackage("0") {}

IntegerFactorizationTask::IntegerFactorizationTask(std::string number, std::string interval)
{
    this->number = CryptoPP::Integer(number.c_str());
    this->interval = CryptoPP::Integer(interval.c_str());
    this->lastPackage = CryptoPP::Integer::One();
}

void IntegerFactorizationTask::load(std::string fileName) {
    std::ifstream configFile (fileName);
    std::string fileContent((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    nlohmann::json rootItem = nlohmann::json::parse(fileContent);
    std::string num = rootItem["task"].find("number").value();
    this->number = CryptoPP::Integer(num.c_str());
    std::string interv = rootItem["task"].find("interval").value();
    this->interval =  CryptoPP::Integer(interv.c_str());
    this->lastPackage = CryptoPP::Integer::One();
    this->countOfPackages = this->number.SquareRoot().DividedBy(this->interval)+1;
}

void IntegerFactorizationTask::save(std::string fileName) {
    //TODO
}

std::string IntegerFactorizationTask::getLastPackage() {
    std::stringstream ss;
    ss << this->lastPackage;
    return ss.str();
}

std::string IntegerFactorizationTask::getTotalPackages() {
    std::stringstream ss;
    ss << this->countOfPackages;
    return ss.str();
}

std::string IntegerFactorizationTask::getNextTask() {
    std::string packageMin;
    std::string packageMax;
    std::string packageCounter;

    CryptoPP::Integer upperLimit = number.SquareRoot()+1;

    std::stringstream ss;
    ss << (upperLimit-(lastPackage-1)*interval);
    packageMax = ss.str();
    //std::cout << "lower:" << lowerLimit << std::endl;
    ss.str("");
    ss.clear();

    ss << (upperLimit-lastPackage*interval);
    packageMin = ss.str();
    //std::cout << "upper: " << upperLimit << std::endl;
    ss.str("");
    ss.clear();
    ss << lastPackage ;
    packageCounter = ss.str();
    ss.str("");
    ss.clear();

    if( (upperLimit-(lastPackage-1)*interval).IsNegative() ){
        throw std::runtime_error("Package queue is empty");
    }

    lastPackage+=1;
    ss.str("");
    ss.clear();

    ss << "{ \"min\":\"" << packageMin << "\", \"max\": \"" << packageMax << "\", \"package\":\""<< packageCounter <<"\" }";

    return ss.str();
}

std::tuple<std::string, std::string> IntegerFactorizationTask::getInitTask() {
    std::stringstream ss;
    //TODO: guid as a client Id
    std::string clientId = uuid::generate_uuid_v4();
    ss << "{ \"number\":\"" << this->number <<"\", \"client\":\""<< clientId << "\" }";
    return std::tuple<std::string, std::string>(clientId, ss.str());
}