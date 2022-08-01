#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

int main()
{
    //vars
    const char *ip = "127.0.0.1";
    int port = 56000;

    //client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //socket structure
    sockaddr_in clientStruct;
    clientStruct.sin_family = AF_INET;
    clientStruct.sin_port = htons(port);
    inet_pton(AF_INET, ip, &clientStruct.sin_addr);

    //wait for connection and check if it was made with success
    int connection = connect(clientSocket, (sockaddr*)&clientStruct, sizeof(clientStruct));
    if (connection < 0)
    {
        printf("\nCouldn't connect :(");
        return -1;
    }

    //receive buffer
    char buff[1024];
    //input string
    char *input[1024];

    //send loop
    while (true)
    {
        //clears the buffers
        memset(&input, 0, sizeof input);

        //receive and store user input
        printf("\n» ");
        fflush(stdout);
        int bytesSent = read(STDIN_FILENO, input, sizeof input);

        //send the input over the socket
        int sendOver = send(clientSocket, input, sizeof input, 0);
        if (sendOver < 0)
        {
            printf("\nCouldn't send message :(");
            continue;
        }
    }

    return 0;
}