//
// Created by thegyi on 2022. 05. 21..
//

#ifndef TASKSERVER_SERVERCONFIG_H
#define TASKSERVER_SERVERCONFIG_H
#include <string>
#include "nlohmann/json.hpp"

class ServerConfig {
public:
    ServerConfig(std::string fileName);

    int getPort();
private:
    nlohmann::json rootItem;
};


#endif//TASKSERVER_SERVERCONFIG_H
