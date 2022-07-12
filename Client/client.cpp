#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h> //for later use, trust me
#include <arpa/inet.h>
#include <string>

using namespace std;

int main()
{
    //vars
    const char *ip = "127.0.0.1";
    int port = 46000;

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
        printf("Couldn't connect :(");
        return -1;
    }

    //receive buffer
    char buff[1024];
    //input string
    char *input;

    //send loop
    while (true)
    {
        //receive and store user input
        printf("» ");
        fflush(stdout);
        int bytesSent = read(STDIN_FILENO, input, sizeof buff);

        //send the input over the socket
        int sendOver = send(clientSocket, input, bytesSent, 0);
        if (sendOver < 0)
        {
            printf("\nCouldn't send message :(");
            continue;
        }
    }

    return 0;
}