#pragma once

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

    // Starts up the server, returns true on success and false on fail.
    // Cleans up after itself on failure.
    BOOL startServer();
    ~SimpleServer();

private:
    CHAR* m_recvbuf;
    SOCKET m_listenSocket;
    struct fd_set m_readfds;
    struct fd_set m_writefds;
};
