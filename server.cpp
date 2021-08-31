// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "iostream"
using namespace std;
#define PORT 8080

void controller(string);
void add(string , string);
void value(string);
void save();
void load();
int server_fd, new_socket; long valread;
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        cout <<"message from client: "<< buffer << endl;
        controller(buffer);
        
        close(new_socket);
    }
    return 0;
}
void controller(string command)
{
    if(command == "add ali password")
    {
        add("ali" , "password");
    }else if(command == "value ali")
    {
        value("ali");
    }else if(command == "save")
    {
        save();
    }else if(command == "load")
    {
        load();
    }
}
void add(string key , string value)
{
    cout << "add function\n";
    char *callBack = "data added successfully";
    write(new_socket , callBack , strlen(callBack));
}
void value(string key)
{
    cout << "value function\n";
    char *callBack = "value in map";
    write(new_socket , callBack , strlen(callBack));
}
void save()
{
    cout << "save function\n";
    char *callBack = "map saved in database";
    write(new_socket , callBack , strlen(callBack));
}
void load()
{
    cout << "load function\n";
    char *callBack = "database loaded";
    write(new_socket , callBack , strlen(callBack));
}