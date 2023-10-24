/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:14:20 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/19 12:18:48 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _verbose(false) {}

Server::~Server(void) {}

void Server::initParser(const char *configFile) { this->_parser.init(configFile, this->_verbose); }

void Server::initSockets(void)
{
    int                      serverSize;
    std::vector<int>         serverInfo;
    std::vector<std::string> port;

    serverInfo = this->_parser.getSizeServers();
    if (serverInfo.empty()) {
        Logger::error << "The server was not configured correctly" << std::endl;
        this->closeServer();
        exit(1);
    }
    serverSize = serverInfo[0];
    try {
        for (int i = 0; i != serverSize; ++i) {
            Socket *socket;

            port = this->_parser.getServerParam(i, "listen");
            if (port.empty())
                socket = new Socket();
            else
                socket = new Socket(port[0]);
            socket->createSocket();
            socket->bindSocket();
            socket->listenForConnections();
            _sockets.push_back(socket);
        }
    } catch (const std::exception &e) {
        Logger::error << e.what() << std::endl;
        this->closeServer();
        exit(1);
    }
}

void Server::initPoll(void)
{
    for (std::vector<Socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        this->_poll.init((*it)->getSocketFd());
    }
}

int Server::run(void)
{
    while (true) {
        if (this->_poll.execute() == -1) {
            Logger::error << "Error in poll()" << std::endl;
            return (1);
        }
        for (size_t i = 0; i < this->_poll.getSize(); ++i) {
            if (this->_poll.isReadable(i)) {
                if (this->_poll.isListeningSocketMatch(i)) {
                    if (!this->_acceptNewConnection(i))
                        continue;
                } else {
                    int clientSocket = this->_poll.getPollFd(i);
                    if (clientSocket < 0) {
                        Logger::error << "Index out of bounds of vector _pollFds" << std::endl;
                        continue;
                    }
                    _processClientData(clientSocket);
                }
            }
        }
        this->_poll.removeMarkedElements();
    }
    return (0);
}

void Server::closeServer(void)
{
    for (std::vector<Socket *>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        delete *it;
    }
    this->_sockets.clear();
    this->_parser.clearParams();
    this->_poll.closePoll();
}

void Server::setVerbose(bool verbose) { this->_verbose = verbose; }

bool Server::_acceptNewConnection(size_t i)
{
    try {
        Socket *client;
        int     clientSocketFd;

        client         = new Socket();
        clientSocketFd = client->acceptConnection(this->_poll.getListeningSocket(i));
        this->_poll.addPoll(clientSocketFd, POLLIN | POLLOUT);
        delete client;

        return (true);
    } catch (const std::exception &e) {
        Logger::error << e.what() << std::endl;
        return (false);
    }
}

std::string Server::_readClientData(int clientSocket)
{
    char        buffer[1024] = {0};
    int         bytes        = 0;
    std::string clientReq;

    while ((_bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        clientReq.append(buffer, _bytesRead);
        if (clientReq.find("Expect: 100-continue") != std::string::npos) {
            sleep(2);
            continue;
        }
        buffer[_bytesRead] = '\0';
        bytes += _bytesRead;
        if (clientReq.find("multipart/form-data") != std::string::npos) {
            std::string boundary;
            size_t      contentTypePos = clientReq.find("Content-Type: ");

            if (contentTypePos != std::string::npos) {
                size_t lineEnd = clientReq.find("\r\n", contentTypePos);
                if (lineEnd != std::string::npos) {
                    std::string contentTypeLine
                        = clientReq.substr(contentTypePos, lineEnd - contentTypePos);
                    size_t boundaryPos = contentTypeLine.find("boundary=");
                    if (boundaryPos != std::string::npos) {
                        boundary = contentTypeLine.substr(boundaryPos + 9);
                    }
                }
            }
            std::string boundaryEnd = "\r\n--" + boundary + "--";
            if (clientReq.find(boundaryEnd) != std::string::npos)
                break;
        } else if (clientReq.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }
    return (clientReq);
}

void Server::_processClientData(int clientSocket)
{
    std::string  clientReq;
    responseData res;

    res       = setResponseData(0, "", "", 0);
    clientReq = this->_readClientData(clientSocket);
    if (_bytesRead == -1) {
        Logger::info << "Client connection closed"
                     << " on socket " << clientSocket << std::endl;
        this->_poll.addFdToClose(clientSocket);
        return;
    }
    if (this->_request.requestHttp(clientReq, this->_parser)) {
        res = _errorPage.getErrorPageStandard(_request.statusCode);
    } else {
        res = this->_responseHandlers.exec(this->_parser, this->_request);
    }
    this->_sendClientData(clientSocket, res);
    return;
}

void Server::_sendClientData(int clientSocket, responseData res)
{
    char responseHeader[1024];

    if (res.contentLength < 0) {
        return;
    }
    if (res.status == PERMANENT_REDIRECT || res.status == TEMPORARY_REDIRECT) {
        sprintf(responseHeader,
                "HTTP/1.1 %s\r\nlocation: "
                "%s\r\n\r\n",
                res.statusCode.c_str(),
                res.location.c_str());
    } else if (res.contentType.empty()) {
        sprintf(responseHeader, "HTTP/1.1 %s\r\n\r\n", res.statusCode.c_str());
    } else if (res.status == METHOD_NOT_ALLOWED) {
        sprintf(responseHeader,
                "HTTP/1.1 %s\r\nAllow: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",
                res.statusCode.c_str(),
                vector_join(this->_request.getLimitExcept(), " ").c_str(),
                res.contentType.c_str(),
                res.contentLength);
    } else {
        sprintf(responseHeader,
                "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",
                res.statusCode.c_str(),
                res.contentType.c_str(),
                res.contentLength);
    }

    int bytes_sent = send(clientSocket, responseHeader, strlen(responseHeader), MSG_NOSIGNAL);
    if (bytes_sent == -1) {
        Logger::info << "Client connection closed"
                     << " on socket " << clientSocket << std::endl;
        this->_poll.addFdToClose(clientSocket);
        return;
    }
    if (res.contentLength) {
        int bytes_sent = send(clientSocket, res.content.c_str(), res.contentLength, MSG_NOSIGNAL);
        if (bytes_sent == -1) {
            Logger::info << "Client connection closed"
                    << " on socket " << clientSocket << std::endl;
            this->_poll.addFdToClose(clientSocket);
            return;
        }
    }
}
