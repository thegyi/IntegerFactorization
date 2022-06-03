//
// Created by thegyi on 2022. 05. 21..
//

#include "ServerConfig.h"
#include "fstream"
#include "nlohmann/json.hpp"


ServerConfig::ServerConfig(std::string fileName) {
    std::ifstream configFile (fileName);
    std::string fileContent((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
    rootItem = nlohmann::json::parse(fileContent);
}

int ServerConfig::getPort() {
    return rootItem.find("port").value();
}
