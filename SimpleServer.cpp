#include "SimpleServer.h"
#include <iostream>
#include <ostream>


SimpleServer::SimpleServer() : m_listenSocket(INVALID_SOCKET) {
    m_recvbuf = new CHAR[DEFAULT_BUFLEN];
}

BOOL SimpleServer::startServer() {
    WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return FALSE;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return FALSE;
    }

    // Create a SOCKET for the server to listen for client connections.
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return FALSE;
    }

    // Setup the TCP listening socket
    iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(m_listenSocket);
        WSACleanup();
        return FALSE;
    }

    freeaddrinfo(result);

    iResult = listen(m_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed with error:" << WSAGetLastError() << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

SimpleServer::~SimpleServer() {
    delete m_recvbuf;
    if (m_listenSocket != INVALID_SOCKET) {
        closesocket(INVALID_SOCKET);
    }
    WSACleanup();
}
