// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "iostream"
using namespace std;
#define PORT 8080

int main(int argc, char const *argv[])
{
    while (1)
    {
        int sock = 0;
        long valread;
        struct sockaddr_in serv_addr;
        char Command[100];
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }

        string input;
        cout << "Enter your command\n";
        cin >> input;
        if (input == "close")
            return 0;
        else if (input == "add")
        {
            string key;
            string value;
            cout << "enter key : \n";
            cin >> key;
            cout << "enter value : \n";
            cin >> value;
            string command = input + " " + key + " " + value;
            strcpy(Command, command.c_str());
        }
        else if (input == "get")
        {
            string key;
            cout << "enter key : \n";
            cin >> key;
            strcpy(Command, (input + " " + key).c_str());
        }
        else if(input == "save")
        {
            strcpy(Command ,"save");
        }
        else if(input == "load")
        {
            strcpy(Command ,"load");
        }
        else if (input == "select")
        {
            string dbNumber;
            cout << "enter DB number : \n";
            cin >> dbNumber;
            strcpy(Command, (input + " " + dbNumber).c_str());
        }

        send(sock, Command, strlen(Command), 0);
        cout << "**********\ncall message : " << Command << endl;
        valread = read(sock, buffer, 1024);
        printf("callBack message : %s\n**********\n", buffer);
    }
    return 0;
}