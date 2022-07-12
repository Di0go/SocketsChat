#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h> //for later use, trust me
#include <arpa/inet.h>
#include <string.h>

using namespace std;

int main()
{
    // server socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the ip address and port to the socket
    sockaddr_in sockStruct;
    sockStruct.sin_family = AF_INET;

    //htons is used to convert between little endian and big endian.
    sockStruct.sin_port = htons(46000);

    //pton receives a string and converts it to an array of chars that is used to define the IP Address
    inet_pton(AF_INET, "0.0.0.0", &sockStruct.sin_addr);

    bind(listening, (sockaddr*)&sockStruct, sizeof(sockStruct));

    listen(listening, SOMAXCONN);

    //client struct
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    //wait for connection
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    // Close listening socket
    close(listening);

    //rcv buffer
    char buf[1024];

    while (true)
    {
        memset(buf, 0, 1024);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, sizeof buf, 0);
        if (bytesReceived <= 0)
        {
            printf("Client disconnected!");
            break;
        }

        //print buffer
        write(1, buf, bytesReceived);
        //fflush(stdout);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}