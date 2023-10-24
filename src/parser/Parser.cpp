/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:49:31 by mcl               #+#    #+#             */
/*   Updated: 2023/10/11 19:08:52 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser() {}

Parser::Parser(const char* fileconf) {
    setConfs(fileconf);
}

void Parser::init(const char* fileconf, bool verbose)
{
    this->_verbose = verbose;
    setConfs(fileconf);
}

Parser::~Parser() {}

int Parser::getServers( void ) const{
	return (this->_servers);
}

void Parser::allocateServers(conf_servers* server, int locs) {

    server->server = new params;
    server->locations = new params*[locs];
    for (int i = 0; i < locs; i++)
        server->locations[i] = new params;
}

void Parser::deallocateServers(conf_servers* server, int locs) {
    if (server) {
        for (int i = 0; i < locs; i++) {
            if (server->locations[i]) {
                delete server->locations[i];
            }
        }
        if (server->locations) {
            delete[] server->locations;
        }
        delete server->server;
    }
}

void Parser::clearParams() {
    for (int i = 0; i < _servers; i++) {
        deallocateServers(&_cservers[i], _locs[i]);
    }
    delete [] _cservers;
}

std::vector<std::string> Parser::getServerParam(int serverIndex, std::string param) {

    if (serverIndex < 0 || serverIndex >= _servers)
        return std::vector<std::string>();

    if (_cservers[serverIndex].server->find(param) != _cservers[serverIndex].server->end()) {
        std::vector<std::string> vparam = (*_cservers[serverIndex].server)[param];
        if (this->_verbose) {
            for (size_t i = 0; i < vparam.size(); i++) {
                Logger::verbose << param << ": " << vparam[i] << std::endl;
            }
        }
        return vparam;
    }
    return std::vector<std::string>();
}

std::vector<std::string> Parser::getLocationParam (int serverIndex, int location, std::string param) {

    if (serverIndex < 0 || serverIndex >= _servers)
        return std::vector<std::string>();

    if (location < 0 || location >= _locs[serverIndex])
        return std::vector<std::string>();

    if (_cservers[serverIndex].locations[location]->find(param) != _cservers[serverIndex].locations[location]->end()) {
        std::vector<std::string> vparam = (*_cservers[serverIndex].locations[location])[param];
        if (this->_verbose) {
            for (size_t i = 0; i < vparam.size(); i++) {
                Logger::verbose << param << "[" << location << "]" << ": " << vparam[i] << std::endl;
            }
        }
        return vparam;
    }
    return std::vector<std::string>();
}

void Parser::populateConfs(std::vector<std::vector<std::string> > servers, std::vector<std::vector<std::string> > locations) {

    _servers = servers.size();
    _cservers = new conf_servers[_servers];


    for (size_t i = 0; i < servers.size(); i++) {
        allocateServers(&_cservers[i], locations[i].size());
        _cservers[i].server = setParams(servers[i][0], _cservers[i].server);
        for (size_t j = 0; j < locations[i].size(); j++) {
            _cservers[i].locations[j] = setParams(locations[i][j], _cservers[i].locations[j]);
        }
        _locs.push_back(locations[i].size());
    }
}

std::vector<int> Parser::getSizeServers () {
    std::vector<int> sizeServers;
    sizeServers.push_back(_servers);
    for (int i = 0; i < _servers; i++) {
        sizeServers.push_back(_locs[i]);
    }
    if (this->_verbose) {
        for (size_t i = 0; i < sizeServers.size(); i++) {
            if (i == 0) {
                Logger::verbose << "Server size: " << sizeServers[i] << std::endl;
            } else {
                Logger::verbose << "Location size"
                                << "(server[" << i - 1 << "]): " << sizeServers[i] << std::endl;
            }
        }
    }
    return sizeServers;
}

void Parser::setConfs(const char* fileconf) {


    std::string                             line;
    std::vector<std::vector<std::string> >  servers;
    std::vector<std::vector<std::string> >  locations;
    std::vector<std::string>                serverBlocks;
    std::vector<std::string>                locationBlocks;
    std::string                             currentServerBlock;
    std::string                             currentLocationBlock;
    bool                                    insideServerBlock = false;
    bool                                    insideLocationBlock = false;
    bool                                    blockEnd = false;

    std::ifstream conf(fileconf);
    if (!conf.is_open()) {
        Logger::error << "Error opening file" << std::endl;
        exit(1);
    }
    conf.clear();
    conf.seekg(0, std::ios::beg);

    while (!conf.eof()) {
        std::getline(conf, line);

        if (verifyLineEmpty(line))
            continue;

        if (line.find("server ") != std::string::npos)
            insideServerBlock = true;

        if (line.find("location ") != std::string::npos)
            insideLocationBlock = true;


        if(insideServerBlock && !insideLocationBlock)
            currentServerBlock += line + "\n";

        if(insideLocationBlock)
            currentLocationBlock += line + "\n";

        if (verifyBlockEnd(line)){
            if (insideLocationBlock){
                locationBlocks.push_back(currentLocationBlock);
                currentLocationBlock.clear();
                insideLocationBlock = false;
            } else if (insideServerBlock && !insideLocationBlock) {
                serverBlocks.push_back(currentServerBlock);
                currentServerBlock.clear();
                insideServerBlock = false;
                blockEnd = true;
            }
        }

        if (!insideServerBlock && !insideLocationBlock && blockEnd) {
            servers.push_back(serverBlocks);
            locations.push_back(locationBlocks);
            serverBlocks.clear();
            locationBlocks.clear();
            blockEnd = false;
        }
    }
    conf.close();

    populateConfs(servers, locations);
}

std::vector<std::string> splitTokens(const std::string str) {

    std::vector<std::string>    vtokens;
    std::istringstream          iss(str);
    std::string                 token;

    while (iss >> token) {
        if (token == "{" || token == "}")
            continue;
        vtokens.push_back(token);
    }
    return vtokens;
}

std::string removeExtraSpaces(const std::string& input) {
    std::string result;
    bool previousCharWasSpace = false;

    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] != ' ') {
            result += input[i];
            previousCharWasSpace = false;
        } else if (!previousCharWasSpace) {
            result += ' ';
            previousCharWasSpace = true;
        }
    }
    if (result[0] == ' ') {
        result.erase(0, 1);
    }
    if (!result.empty() && result[result.length() - 1] == ' ') {
        result.erase(result.length() - 1);
    }
    return result;
}

params* setParams(const std::string str, params* vconfs) {

    std::vector<std::string>    tokens;
    std::string                 tmp_str = str;
    std::string                 token;

    size_t pos = str.find('\n');
    while (pos != std::string::npos) {
        token = tmp_str.substr(0, pos);
        tokens.push_back(removeExtraSpaces(token));
        tmp_str = tmp_str.substr(pos + 1);
        pos = tmp_str.find('\n');
    }
    for (size_t i = 0; i < tokens.size(); i++) {
        pos = tokens[i].find(' ');
        if (pos != std::string::npos) {
            std::string key = tokens[i].substr(0, pos);
            std::vector<std::string> value = splitTokens(tokens[i].substr(pos + 1));
            if (key != "server")
                (*vconfs)[key] = value;
        }
    }
    return vconfs;
}

bool verifyBlockEnd(const std::string& text) {

    std::string::size_type  pos = 0;
    bool                    onlyClosingBrace = true;
    int                     countBrace = 0;
    int                     countSpace = 0;

    while (pos < text.length() && text[pos] != '\n') {
        if (text[pos] != '}' && text[pos] != ' ') {
            onlyClosingBrace = false;
            break;
        }
        if (text[pos] == '}')
            countBrace++;
        if (text[pos] == ' ')
            countSpace++;
        ++pos;
    }
    if (countBrace != 1)
        onlyClosingBrace = false;
    if (countSpace == 0 && countBrace == 1)
        onlyClosingBrace = true;

    return onlyClosingBrace;
}

bool verifyLineEmpty(const std::string& text) {

    bool emptyLine = true;

    for (size_t i = 0; i < text.length(); i++) {
        if (!isspace(text[i])) {
            emptyLine = false;
            return emptyLine;
        }
    }
    return emptyLine;
}