// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include "iostream"
using namespace std;
#define PORT 8080
//hello there 2
void controller(string);
void add(string, string);
void getValue(string);
void selectDB(int);
void save();
void load();
int server_fd, new_socket;
long valread;
int dbNumber;
vector<map<string, string>> container;
mutex myMutex;

int main(int argc, char const *argv[])
{

    map<string, string> db1;
    map<string, string> db2;
    container.push_back(db1);
    container.push_back(db2);

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
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        cout << "message from client: " << buffer << endl;
        controller(buffer);

        close(new_socket);
    }
    return 0;
}

void controller(string command)
{
    int start = 0;
    int count = 0;
    string fstr, mstr, lstr;
    string s = command;
    string spc = " ";
    int end = s.find(spc);
    while (end != -1)
    {
        if (count == 0)
        {
            fstr = s.substr(start, end - start);
            start = end + spc.size();
            end = s.find(spc, start);
            count++;
        }
        else if (count == 1)
        {
            mstr = s.substr(start, end - start);
            start = end + spc.size();
            end = s.find(spc, start);
            count++;
        }
    }
    if (count == 0)
    {
        fstr = s.substr(start, end - start);
    }
    if (count == 1)
    {
        mstr = s.substr(start, end - start);
    }
    if (count == 2)
    {
        lstr = s.substr(start, end - start);
    }

    if (fstr == "add" || fstr == "Add")
    {
        // add(mstr , lstr);
        thread t(add, mstr, lstr);
        if (t.joinable() == true)
            t.join();
    }
    else if (fstr == "get" || fstr == "Get")
    {
        // get(mstr);
        thread t1(getValue, mstr);
        if (t1.joinable() == true)
            t1.join();
    }
    else if (fstr == "select")
    {
        if (mstr == "db1" || mstr == "DB1")
        {
            // select(1);
            thread t2(selectDB, 1);
            if (t2.joinable() == true)
                t2.join();
        }
        if (mstr == "db2" || mstr == "DB2")
        {
            // select(2);
            thread t3(selectDB, 2);
            if (t3.joinable() == true)
                t3.join();
        }
    }
    else if (fstr == "save")
    {
        // save();
        thread t4(save);
        if (t4.joinable() == true)
            t4.join();
    }
    else if (fstr == "load")
    {
        // load();
        thread t5(load);
        if (t5.joinable() == true)
            t5.join();
    }
    else
    {
        cout << "command is not valid\n";
    }
}
void add(string key, string value)
{
    lock_guard<std::mutex> guard(myMutex);
    char *callBack;
    if (container[dbNumber - 1].find(key) == container[dbNumber - 1].end())
    {
        container[dbNumber - 1].insert(pair<string, string>(key, value));
        callBack = "data added successfully";
    }
    else
    {
        callBack = "key is already exist";
    }
    write(new_socket, callBack, strlen(callBack));
}
void getValue(string key)
{
    lock_guard<std::mutex> guard(myMutex);
    auto it = container[dbNumber - 1].find(key);
    string s;
    if (it != container[dbNumber - 1].end())
        s = it->second;
    else
        s = " this key doesnt exist \n ";
    char *callBack = &s[0];
    write(new_socket, callBack, strlen(callBack));
}
void selectDB(int _dbNumber)
{
    lock_guard<std::mutex> guard(myMutex);
    dbNumber = _dbNumber;
    char *callBack = "database selected successfully";
    write(new_socket, callBack, strlen(callBack));
}
void save()
{
    cout << "save function\n";
    char *callBack = "map saved in database";
    write(new_socket, callBack, strlen(callBack));
}
void load()
{
    cout << "load function\n";
    char *callBack = "database loaded";
    write(new_socket, callBack, strlen(callBack));
}
