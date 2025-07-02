#include "SimpleServer.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <winsock.h>
#include <ws2tcpip.h>

typedef struct addrinfo addrinfo;

/*
 * A wrapper to getaddrinfo that returns a unique ptr to the object.
 * MAKE SURE TO CHECK IF NULL. if the action fails it returns NULL ptr.
 */
std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> safeGetAddr(addrinfo* hintsPtr) {
    addrinfo* result;
    int iResult = getaddrinfo(NULL, DEFAULT_PORT, hintsPtr, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << WSAGetLastError();
        WSACleanup();
        return std::unique_ptr<addrinfo, decltype(&freeaddrinfo)>(NULL, NULL);
    }
    return std::unique_ptr<addrinfo, decltype(&freeaddrinfo)>(result, &freeaddrinfo);
}

bool SimpleServer::handleClient(SOCKET clientSocket) {
    if (!FD_ISSET(clientSocket, &m_readfds)) {
        return false;
    }
    memset(m_recvbuf, 0, DEFAULT_BUFLEN);
    int bytesRecived = recv(clientSocket, m_recvbuf, DEFAULT_BUFLEN, 0);
    if (bytesRecived == 0) {
        int iResult = shutdown(clientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return true;
        }
    }
    if (bytesRecived < 0) {
        int iResult = closesocket(clientSocket);
        if (iResult == SOCKET_ERROR) {
            std::cout << "Failed to close clientSocket!" << std::endl;
            return true;
        }
    }
    std::cout << "Recovered " << bytesRecived << "bytes: " << m_recvbuf << std::endl;
    return true; // Remove client
}

SimpleServer::SimpleServer() : m_listenSocket(INVALID_SOCKET) {
    m_recvbuf = new CHAR[DEFAULT_BUFLEN];
    FD_ZERO(&m_readfds);
    FD_ZERO(&m_writefds);
}

bool SimpleServer::initServer() {
    m_listenSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int iResult;

    addrinfo hints;

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
    auto result = safeGetAddr(&hints);
    if (result.get() == NULL) {
        return false;
    }

    // Create a SOCKET for the server to listen for client connections.
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    // Setup the TCP listening socket
    iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }

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
    for (;;) {
        FD_ZERO(&m_readfds);
        FD_SET(m_listenSocket, &m_readfds);
        for (SOCKET clientSock : m_clientList) {
            // TODO check if we dont get over the limit of fd. if so then split to multiple selects.
            FD_SET(clientSock, &m_readfds);
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
