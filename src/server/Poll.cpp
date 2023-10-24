/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:48:24 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/19 10:04:18 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll(void) {}

Poll::~Poll(void) { this->closePoll(); }

void Poll::init(int socketFd)
{
    this->_listeningSockets.push_back(socketFd);
    this->addPoll(socketFd, POLLIN);
}

void Poll::addPoll(int socketFd, short events)
{
    struct pollfd clientPoolFd;

    clientPoolFd.fd      = socketFd;
    clientPoolFd.events  = events;
    clientPoolFd.revents = 0;
    this->_pollFds.push_back(clientPoolFd);
}

int Poll::execute(void) { return poll(&this->_pollFds[0], this->_pollFds.size(), 0); }

bool Poll::isReadable(size_t i) { return (this->_pollFds[i].revents & POLLIN); }

bool Poll::isListeningSocketMatch(size_t i)
{
    return (i < this->_listeningSockets.size()
            && this->_pollFds[i].fd == this->_listeningSockets[i]);
}

void Poll::addFdToClose(int socket) { this->_fdToClose.push_back(socket); }

void Poll::removeListeningSocket(int socketFd)
{
    // Encontre e remova o socketFd do vetor _listeningSockets
    std::vector<int>::iterator it
        = std::find(_listeningSockets.begin(), _listeningSockets.end(), socketFd);
    if (it != _listeningSockets.end()) {
        _listeningSockets.erase(it);
    }
}

void Poll::removePollFd(int socketFd)
{
    std::vector<struct pollfd>::iterator it = _pollFds.begin();
    while (it != _pollFds.end()) {
        if (it->fd == socketFd) {
            it = _pollFds.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Poll::removeMarkedElements(void)
{
    for (std::vector<int>::iterator it = this->_fdToClose.begin(); it != this->_fdToClose.end();
         ++it) {
        int fdToRemove = *it;

        removeListeningSocket(fdToRemove);
        removePollFd(fdToRemove);
        close(fdToRemove);
    }
    this->_fdToClose.clear();
}

void Poll::closePoll(void)
{
    for (size_t i = 0; i < this->_pollFds.size(); ++i) {
        int fd = this->_pollFds[i].fd;
        if (fd != -1) {
            close(fd);
            this->_pollFds[i].fd = -1;
        }
    }
    this->_pollFds.clear();
}

size_t Poll::getSize(void) const { return (this->_pollFds.size()); }

int Poll::getListeningSocket(size_t i) const { return (this->_listeningSockets[i]); }

int Poll::getPollFd(size_t i) const
{
    if (i < this->_pollFds.size()) {
        return this->_pollFds[i].fd;
    }
    return (-1);
}
