#pragma once
#include <winsock2.h>

/*
 * This class handles all the interaction with the client,
 * including maneging the opening and closing of socket,
 * and the logic of reciving/sending messages.
 *
 * Note that this client has no state, but this is possible to add in the future.
 */
class SimpleClientHandle {
public:
    SimpleClientHandle(SOCKET clientSocket);
    /*
     * Closes the connection with the client.
     */
    ~SimpleClientHandle();

    /*
     * This function is called whenever the server recived a message
     * from the client, it handles the response back to the server.
     */
    void recivedMessage(char* buffer);

private:
    SOCKET clientSocket;
};
