/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 21:01:05 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/19 10:04:38 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(std::string port, std::string ip) :
    _socketFd(-1), _port(port), _ip(ip), _serverInfo(NULL)
{
}

Socket::~Socket(void) { this->closeSocket(); }

void Socket::createSocket(void)
{
    struct addrinfo hints;
    std::string     msg;
    int             yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
    int result = getaddrinfo(this->_ip.c_str(), this->_port.c_str(), &hints, &this->_serverInfo);
    if (result != 0) {
        msg = "Error getting address information: " + std::string(gai_strerror(result));
        throw Socket::SocketException(msg.c_str());
    }
    this->_socketFd = socket(this->_serverInfo->ai_family,
                             this->_serverInfo->ai_socktype,
                             this->_serverInfo->ai_protocol);
    if (this->_socketFd < 0) {
        msg = "Error creating server socket: " + std::string(strerror(errno));
        throw Socket::SocketException(msg.c_str());
    }
    setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    return;
}

void Socket::bindSocket(void)
{
    std::string msg;

    if (bind(this->_socketFd, this->_serverInfo->ai_addr, this->_serverInfo->ai_addrlen) < 0) {
        msg = "Error binding socket to port: " + std::string(strerror(errno));
        throw Socket::SocketException(msg.c_str());
    }
    freeaddrinfo(this->_serverInfo);
    this->_serverInfo = NULL;
    return;
}

void Socket::listenForConnections(void)
{
    std::string msg;

    if (listen(this->_socketFd, SOMAXCONN) < 0) {
        msg = "Error listening for connections: " + std::string(strerror(errno));
        throw Socket::SocketException(msg.c_str());
    }
    Logger::info << "Listening at " << this->_ip << ":" << this->_port << std::endl;
}

void *Socket::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int Socket::acceptConnection(int socketFd)
{
    struct sockaddr_storage clientAddr;
    socklen_t               addr_size;
    std::string             msg;
    int                     clientSocket;
    char                    remoteIP[INET6_ADDRSTRLEN];
    std::stringstream       ss;

    addr_size    = sizeof clientAddr;
    clientSocket = accept(socketFd, (struct sockaddr *)&clientAddr, &addr_size);
    if (clientSocket == -1) {
        msg = "Error accepting client connection: " + std::string(strerror(errno));
        throw Socket::SocketException(msg.c_str());
    }
    ss << "New connection from client "
       << inet_ntop(clientAddr.ss_family,
                    get_in_addr((struct sockaddr *)&clientAddr),
                    remoteIP,
                    INET6_ADDRSTRLEN)
       << " on socket " << clientSocket;
    msg = ss.str();
    Logger::info << msg << std::endl;
    return (clientSocket);
}

void Socket::closeSocket(void)
{
    if (this->_socketFd != -1) {
        close(this->_socketFd);
        this->_socketFd = -1;
    }
    if (this->_serverInfo != NULL) {
        freeaddrinfo(this->_serverInfo);
        this->_serverInfo = NULL;
    }
}

int Socket::getSocketFd(void) const { return (this->_socketFd); }

std::string Socket::getPort(void) const { return (this->_port); }

std::string Socket::getIp(void) const { return (this->_ip); }

Socket::SocketException::SocketException(const char *msg) : _msg(msg) {}

Socket::SocketException::~SocketException() throw() {}

const char *Socket::SocketException::what() const throw() { return (_msg.c_str()); }
