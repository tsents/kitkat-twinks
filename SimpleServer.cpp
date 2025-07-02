#include "SimpleServer.h"
#include <cstring>
#include <iostream>
#include <psdk_inc/_fd_types.h>
#include <psdk_inc/_socket_types.h>
#include <winsock.h>


bool SimpleServer::handleClient(SOCKET clientSocket) {
    if (!FD_ISSET(clientSocket, &m_readfds)) {
        return false;
    }
    memset(m_recvbuf, 0, DEFAULT_BUFLEN);
    int bytesRecived = recv(clientSocket, m_recvbuf, DEFAULT_BUFLEN, 0);
    if (bytesRecived == 0) {
        int iResult = shutdown(clientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
        }
    }
    if (bytesRecived < 0) {
        int iResult = closesocket(clientSocket);
        if (iResult== SOCKET_ERROR) {
            std::cout << "Failed to close clientSocket!" << std::endl;
        }
    }
    std::cout << "Recovered " << bytesRecived << "bytes: " << m_recvbuf << std::endl;
    return true; //Remove client
}

SimpleServer::SimpleServer() : m_listenSocket(INVALID_SOCKET) {
    m_recvbuf = new CHAR[DEFAULT_BUFLEN];
    FD_ZERO(&m_readfds);
    FD_ZERO(&m_writefds);
}

bool SimpleServer::initServer() {
    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << WSAGetLastError();
        WSACleanup();
        return false;
    }

    // Create a SOCKET for the server to listen for client connections.
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Setup the TCP listening socket
    iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }

    freeaddrinfo(result);

    iResult = listen(m_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed with error:" << WSAGetLastError() << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }
    DWORD optval = 1;
    iResult = ioctlsocket(m_listenSocket, FIONBIO, &optval);
    if (iResult == SOCKET_ERROR) {
        std::cout << "ioctlsocket failed " << WSAGetLastError() << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool SimpleServer::runServer() {
    FD_SET(m_listenSocket, &m_readfds); //This adds the server socket to listen.
    for (;;) {
        FD_ZERO(&m_readfds);
        FD_SET(m_listenSocket, &m_readfds);
        for (SOCKET clientSock : m_clientList) {
            FD_SET(clientSock, &m_readfds); //TODO check if we dont get over the limit of fd. if so then split to multiple selects.
        }
        int nReady = select(0, &m_readfds, &m_writefds, NULL, NULL); // MSDN had bug here :)
        if (nReady == SOCKET_ERROR) {
            std::cout << "Error encountered in select: " << WSAGetLastError() << std::endl;
            return false;
        }
        if (nReady == 0) {
            std::cout << "No connections/data." << std::endl;
            Sleep(3000);
            continue;
        }
        if (FD_ISSET(m_listenSocket, &m_readfds)) {
            SOCKET acceptedClient = accept(m_listenSocket, NULL, NULL);
            if (acceptedClient == INVALID_SOCKET) {
                std::cout << "Failed to accept new client!" << std::endl;
            } else {
                m_clientList.push_front(acceptedClient);
                std::cout << "Accepted new client" << std::endl;
            }
        }
        for (auto it = m_clientList.begin(); it != m_clientList.end(); it++) {
            if (handleClient(*it)) {
                it = m_clientList.erase(it);
            }
        }
        // if (m_readfds.fd_array[i] == m_listenSocket) {
        //
        // } else {
        //     handleClient(m_readfds.fd_array[i]);
        // }
    }
    return true;
}

SimpleServer::~SimpleServer() {
    delete m_recvbuf;
    if (m_listenSocket != INVALID_SOCKET) {
        closesocket(INVALID_SOCKET);
    }
    WSACleanup();
}
