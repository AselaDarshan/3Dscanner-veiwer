#ifndef CONNECTIONHELPER_H
#define CONNECTIONHELPER_H
#include <iostream>
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>




class ConnectionHelper{
protected:
    int sock;
    struct sockaddr_in server;

    char* message;
public:
    bool sendData(std::string message);
   std::string recieve();
    void disconnect();
    int initialize();

};

#endif // CONNECTIONHELPER_H
