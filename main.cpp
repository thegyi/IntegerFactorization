#include <iostream>
#include "ServerConfig.h"
#include "server_http.hpp"
#include "IntegerFactorizationTask.h"
#include <thread>

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
std::vector<std::string> registeredClients;

int main() {
    ServerConfig conf("./config/server.json");

    SimpleWeb::Server<SimpleWeb::HTTP> server;
    server.config.port = conf.getPort();

    IntegerFactorizationTask task;
    task.load("/./skServer/config/server.json");

    std::thread server_thread([&server]() {
      server.start();
    });

    server.resource["^/Task/get$"]["GET"] = [&task](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
        SimpleWeb::CaseInsensitiveMultimap query = request->parse_query_string();
        auto clientIdIt = query.find("clientId");

        //std::find(registeredClients.begin(), registeredClients.end(), clientIdIt->second) == registeredClients.end()
        if(clientIdIt == query.end() || std::find(registeredClients.begin(), registeredClients.end(), clientIdIt->second) == registeredClients.end()){
            std::cout << "Request from unauthenticated client!" << std::endl;
            response->write(SimpleWeb::StatusCode::client_error_not_found);
            return;
        }

        //std::string clientId = clientIdIt->second;

        std::string responseMessage;
        try{
            std::cout << "Request from authenticated client: " << clientIdIt->second << " ("<<task.getLastPackage() << "/" << task.getTotalPackages() << ")"<< std::endl;
            responseMessage = task.getNextTask();
        } catch(std::runtime_error &ex){
            std::cerr << ex.what() << std::endl;
            response->write(SimpleWeb::StatusCode::client_error_not_found);
            return;
        };
        *response << "HTTP/1.1 200 OK\r\n"
                << "Content-Type: application/json" << "\r\n"
                << "Content-Length: " << responseMessage.length() << "\r\n\r\n"
                << responseMessage;
    };

    server.resource["^/Task/initialize$"]["GET"] = [&task](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      std::tuple<std::string, std::string> initTask = task.getInitTask();
      std::string responseMessage = std::get<1>(initTask);
      std::string clientId = std::get<0>(initTask);
      registeredClients.push_back(clientId);
      *response << "HTTP/1.1 200 OK\r\n"
                << "Content-Type: application/json" << "\r\n"
                << "Content-Length: " << responseMessage.length() << "\r\n\r\n"
                << responseMessage;
      std::cout << "Initialize client " + request->remote_endpoint->address().to_string() + ", with id:" << clientId << std::endl;
    };

    server.resource["^/Task/solve$"]["GET"] = [&task](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      SimpleWeb::CaseInsensitiveMultimap query = request->parse_query_string();
      auto clientIdIt = query.find("clientId");
      auto integerFactorIt = query.find("primeFactor");

      if(clientIdIt == query.end() || std::find(registeredClients.begin(), registeredClients.end(), clientIdIt->second) == registeredClients.end()){
          std::cout << "Solution from unauthenticated client!" << std::endl;
          response->write(SimpleWeb::StatusCode::client_error_not_found);
          return;
      }

      std::cout << "Solution from client: " << clientIdIt->second << ", factor:" << integerFactorIt->second << std::endl;

      std::string responseMessage = "";
      *response << "HTTP/1.1 200 OK\r\n"
                << "Content-Type: application/json" << "\r\n"
                << "Content-Length: " << responseMessage.length() << "\r\n\r\n"
                << responseMessage;
    };

    server_thread.join();
    return 0;
}
