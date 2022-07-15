#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>

using namespace std;

void newConnection(int newSocket)
{
    //rcv buffer
    char buf[1024];

    while (true)
    {
        memset(buf, 0, 1024);

        // Wait for client to send data
        int bytesReceived = recv(newSocket, buf, sizeof buf, 0);
        if (bytesReceived <= 0)
        {
            printf("Client disconnected!");
            break;
        }

        //print buffer
        write(1, buf, bytesReceived);
        fflush(stdout); //!
    }
}

//main method (constantly listening)
int main()
{
    // server socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the ip address and port to the socket
    sockaddr_in sockStruct;
    sockStruct.sin_family = AF_INET;

    //htons is used to convert between little endian and big endian.
    sockStruct.sin_port = htons(56000);

    //pton receives a string and converts it to an array of chars that is used to define the IP Address
    inet_pton(AF_INET, "127.0.0.1", &sockStruct.sin_addr);

    bind(listening, (sockaddr*)&sockStruct, sizeof(sockStruct));

    listen(listening, SOMAXCONN);

    //client struct
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    //this loop makes sure the server is constantly accepting new clients
    while (true)
    {
        //wait for connection
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

        //this thread will now handle all the communication with the client from now on
        thread openChat(newConnection, clientSocket);
        openChat.join();
    }

    return 0;
}
