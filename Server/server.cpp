#include <cstddef>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <vector>

//this struct defines all the components needed about a user (more to be added when needed)
struct User 
{
  const char *userName;
  int userSocket;
  sockaddr_in userAddress;
};

//this function is called everytime a new client joins
void *newConnection(void* user)
{
    //cast from void pointer to user pointer
    auto *newUser = (User *)user;

    //rcv buffer
    char buf[1024];

    while (true)
    {
        memset(buf, 0, 1024);

        // waiting for the client to send anything
        int bytesReceived = recv(newUser->userSocket, buf, sizeof buf, 0);
        if (bytesReceived <= 0)
        {
            printf("\nClient disconnected!");
            fflush(stdout);
            return 0;
        }
        else 
        {
            //print buffer
            write(1, buf, bytesReceived);
            fflush(stdout); //!
        }
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

    //htons is used to convert between little endian and big endian. (!!)
    sockStruct.sin_port = htons(56000);

    //pton receives a string and converts it to an array of chars that is used to define the IP Address
    inet_pton(AF_INET, "127.0.0.1", &sockStruct.sin_addr);

    int bindStatus = bind(listening, (sockaddr*)&sockStruct, sizeof(sockStruct));
    if (bindStatus < 0) 
    {
        printf("\nCould not bind!");
        fflush(stdout);
    };


    listen(listening, SOMAXCONN);

    //client struct
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    //both of his vectors will keep track of the users and the current threads
    std::vector<User> users;
    std::vector<pthread_t> threads;

    //this loop makes sure the server is constantly accepting new clients
    while (true)
    {
        //wait for connection
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket < 0) 
        {
            printf("\nClient failed to connect!");
            fflush(stdout); 
        }

        printf("\nNew client arrived!");
        fflush(stdout);

        //creates the user and adds it to the vector
        users.push_back({NULL, clientSocket, client});

        //creates and initializes new thread
        pthread_t socketThread;
        threads.push_back(socketThread);
        pthread_create(&threads.back(), NULL, *newConnection, &users.back());

        //detatches the thread
        pthread_detach(threads.back());
    }

    return 0;
}
