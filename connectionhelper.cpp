#include "connectionhelper.h"

using namespace std;
bool ConnectionHelper::sendData(string message){
  // message.
    cout<<"sent: "<<message<<endl;
    if( send(sock , (message).c_str() ,message.size() , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
}

int ConnectionHelper::initialize(){

    message = (char*)malloc(sizeof(char)*1000);

    puts("Creating socket\n");
    //this->a=4;
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    // sockt=&sock;
    if (sock == -1)
    {
        printf("Could not create socket");
        return -1;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("10.42.0.84");
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -2;
    }

    cout<<"connected"<<endl;

    //keep communicating with server
    //    while(1)
    //    {
    //        cout<<"Enter message : "<<endl;
    //        string m;
    //        m="hello";
    //       // cin>>m;
    //        cout<<"message: "<<endl;

    //        //Send some data
    //        if( send(sock , m.c_str() , m.size() , 0) < 0)
    //        {
    //            puts("Send failed");
    //            return 1;
    //        }

    //        //Receive a reply from the server
    //        if( recv(sock , server_reply , 2000 , 0) < 0)
    //        {
    //            puts("recv failed");
    //            break;
    //        }

    //        puts("Server reply : ");
    //        cout<<server_reply<<endl;
    //        break;
    //    }

    //   send(sock , "message" , strlen("message") , 0);
    //   send(sock , "message" , strlen("message") , 0);
    //   recv(sock , server_reply , 2000 , 0);
    //   puts(server_reply);
    //   send(sock , "message" , strlen("message") , 0);
    //   recv(sock , server_reply , 2000 , 0);
    //   puts(server_reply);
    // close(sock);
    return sock;
}
string ConnectionHelper::recieve(){
    char* server_reply=new char[2000];
    // string server_reply;
    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
        return "faild";
    }

    puts("Server reply : ");
    cout<<server_reply<<endl;
    return server_reply;
}
void ConnectionHelper::disconnect(){
    close(sock);
}
