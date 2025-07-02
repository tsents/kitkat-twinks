#pragma once

#include <list>

// TYPES
#include <basetsd.h>
#include <windef.h>
#include <winnt.h>

#include <winsock2.h>
#include <ws2tcpip.h>

const unsigned int DEFAULT_BUFLEN = 2048;
const char* const DEFAULT_PORT = "27015";

/*
 * Create a TCP server that has no actuall functionality.
 * the main idea is to make the class a nice interface to
 * add functionality for later parts.
 *
 * It implements accepting and handeling of requests using
 * select and multiple threads.
 */
class SimpleServer {
public:
    // Allocate m_recvbuf and initizlized m_listenSocket to default invalid socket.
    SimpleServer();
    ~SimpleServer();

    /*
     * Starts the server, initizlizing a listening socket, and listening on it.
     * the socket is defined to be non-blocking, allowing the use of select.
     * in case of failure, the user can try to run this function again.
     *
     * return [OUT]  true if the initizlizing succeded, and false if failed.
     */
    bool initServer();

    /*
     * Enters the server into an infinite reciving loop, accepting new clients
     * and serving them. this function returns false if some error occured,
     * and after it the user is advized to delete the Server.
     *
     * return [OUT]  false if existed without error, true return doesn't exist yet but might in the future.
     */
    bool runServer();

    /*
     * Handles the client request, returning true if the conversasion
     * with the client ended, and false if further handling of him
     * is needed. such as more recovering of data.
     *
     */
    bool handleClient(SOCKET clientSocket);

private:
    CHAR* m_recvbuf;
    SOCKET m_listenSocket;
    struct fd_set m_readfds;
    struct fd_set m_writefds;
    std::list<SOCKET> m_clientList;
};
