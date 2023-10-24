/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:24:07 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/24 20:47:19 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Request.hpp"

HttpRequest::HttpRequest() { init(); }

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getMethod(void) const { return (this->_method); }

std::string HttpRequest::getPort(void) const { return (this->_port); }

std::string HttpRequest::getHost(void) const { return (this->_host); }

std::string HttpRequest::getUri(void) const { return (this->_uri); }

std::string HttpRequest::getHttp(void) const { return (this->_httpVersion); }

std::string HttpRequest::getBody(void) const { return (this->_body); }

std::string HttpRequest::getBoundary(void) const { return (this->_boundary); }

std::vector<std::string> HttpRequest::getQuery(void) const { return (this->_paramQuery); }

std::map<std::string, std::string> HttpRequest::getHeaders() const { return (_header); }

int HttpRequest::getServerIndex(void) const { return (this->_serverIndex); }

int HttpRequest::getLocationIndex(void) const { return (this->_locationIndex); }

int HttpRequest::getLocationSize(void) const { return (this->_locationSize); }

size_t HttpRequest::getMaxBodySize(void) const { return (this->_maxBodySize); }

size_t HttpRequest::getContentLength(void) const { return (this->_contentLength); }

std::string HttpRequest::getRoot(void) const { return (this->_root); }

std::string HttpRequest::getPath(void) const { return (this->_path); }

std::vector<std::string> HttpRequest::getErrorPageConfig(void) const
{
    return (this->_errorPageConfig);
}

std::vector<std::string> HttpRequest::getLimitExcept(void) const { return (this->_limitExcept); }

void HttpRequest::init()
{
    _allowMethods.push_back("GET");
    _allowMethods.push_back("POST");
    _allowMethods.push_back("DELETE");
    has_body       = false;
    has_form       = false;
    has_multipart  = false;
    statusCode     = 0;
    content        = "";
    _uri           = "";
    _port          = "";
    _method        = "";
    _boundary      = "";
    _httpVersion   = "";
    _maxBodySize   = 0;
    _contentLength = 0;
    _paramQuery.clear();
    _header.clear();
}

bool HttpRequest::requestHttp(std::string request, Parser &parser)
{
    size_t firstLineEnd = request.find("\r\n");

    if (firstLineEnd == std::string::npos) {
        this->statusCode = BAD_REQUEST;
        return (true);
    }

    std::string requestLine = request.substr(0, firstLineEnd);
    std::string headersPart = request.substr(firstLineEnd + 2);

    if (_parseFirstLine(requestLine))
        return (true);
    _parseHeaders(headersPart);
    _getMaxBody(parser);
    _getServerParam(parser);
    _setAutoIndex(parser);
    _getHost();

    if (has_body) {
        has_multipart = false;
        has_form      = false;
        std::map<std::string, std::string>::const_iterator it;
        it = _header.find("Content-Type");
        if (it->second.find("multipart/form-data") != std::string::npos)
            has_multipart = true;
        if (it->second.find("application/x-www-form-urlencoded") != std::string::npos)
            has_form = true;
    }

    if (has_multipart) {
        if (_getMultipartData(request))
            return (true);
    } else if (has_body) {
        if (_getBody(request))
            return (true);
    }
    return (false);
}

bool HttpRequest::_parseFirstLine(std::string &requestLine)
{
    std::istringstream iss(requestLine);

    if (!(iss >> this->_method >> this->_uri >> this->_httpVersion)
        || requestLine != this->_method + " " + this->_uri + " " + this->_httpVersion
        || this->_uri[0] != '/') {
        this->statusCode = BAD_REQUEST;
        return (true);
    }

    if (std::find(_allowMethods.begin(), _allowMethods.end(), _method) == _allowMethods.end()){
        this->statusCode = METHOD_NOT_ALLOWED;
        return (true);
    }
    if (this->_httpVersion != "HTTP/1.1") {
        this->statusCode = HTTP_VERSION_NOT_SUPPORTED;
        return (true);
    }
    size_t pos = this->_uri.find('?');
    if (pos != std::string::npos) {
        _parseQuery();
        this->_uri.erase(pos);
    }
    return (false);
}

void HttpRequest::_parseQuery()
{
    size_t      posQuery = this->_uri.find("?");
    std::string query;

    if (posQuery != std::string::npos) {
        query                        = this->_uri.substr(posQuery + 1);
        std::string::size_type start = 0;
        while (start < query.length()) {
            std::string::size_type equal = query.find('=', start);
            if (equal != std::string::npos) {
                std::string::size_type ampersand = query.find('&', equal);
                if (ampersand != std::string::npos) {
                    _paramQuery.push_back(query.substr(equal + 1, ampersand - equal - 1));
                    start = ampersand + 1;
                } else {
                    _paramQuery.push_back(query.substr(equal + 1));
                    break;
                }
            } else
                break;
        }
    }
}

void HttpRequest::_parseHeaders(const std::string &request)
{
    std::istringstream iss(request);
    std::string        headerLine;
    has_body = false;

    while (std::getline(iss, headerLine, '\r')) {
        headerLine.erase(std::remove(headerLine.begin(), headerLine.end(), '\n'), headerLine.end());

        if (headerLine.empty())
            break;

        if (!headerLine.empty()) {
            size_t colonPos = headerLine.find(": ");

            if (colonPos != std::string::npos) {
                std::string key    = headerLine.substr(0, colonPos);
                std::string value  = headerLine.substr(colonPos + 1);
                this->_header[key] = value;
                this->_findHeaders(key, value);
            }
        }
    }
}

void HttpRequest::_findHeaders(std::string key, std::string value)
{
    if (key == "Host") {
        size_t pos = value.find(":");
        if (pos != std::string::npos) {
            std::string tmp = value.substr(pos + 1);
            this->_port     = tmp;
        }
    }
    if (key == "Content-Length") {
        int length = atoi(value.c_str());
        if (length > 0) {
            has_body             = true;
            _contentLength = length;
        }
    }
}

void HttpRequest::_getMaxBody(Parser &parser)
{
    int servers = parser.getServers();

    for (int i = 0; i < servers; i++) {
        std::vector<std::string> listen = parser.getServerParam(i, "listen");

        if (!listen.empty() && !listen[0].empty() && _port == listen[0]) {
            std::vector<std::string> maxBody = parser.getServerParam(i, "client_max_body_size");
            if (!maxBody.empty() && !maxBody[0].empty())
                this->_maxBodySize = std::atoi(maxBody[0].c_str());
            break;
        }
    }
}

void HttpRequest::_getHost()
{
    std::string       uri = _header["Host"];
    std::stringstream ss(uri);

    if (std::getline(ss, _host, ':'))
        _host.erase(std::remove_if(_host.begin(), _host.end(), ::isspace), _host.end());
}

bool HttpRequest::_getMultipartData(std::string request)
{
    std::string contentType = _header["Content-Type"];

    size_t pos = contentType.find("boundary=");
    if (pos != std::string::npos) {
        this->_boundary = contentType.substr(pos + 9);
        this->_boundary = "--" + _boundary;
    } else {
        this->statusCode = BAD_REQUEST;
        return (true);
    }

    size_t startBody = request.find("\r\n\r\n") + 4;

    if (startBody != std::string::npos)
        _body = request.substr(startBody);
    else {
        this->statusCode = BAD_REQUEST;
        return (true);
    }
    return (false);
}

bool HttpRequest::_getBody(std::string request)
{
    std::size_t bodyStart = request.find("\r\n\r\n") + 4;

    if (bodyStart != std::string::npos)
        this->_body = request.substr(bodyStart);
    if (_maxBodySize > 0) {
        if ((_body.size() / 1024) > _maxBodySize) {
            this->statusCode = ENTITY_TOO_LARGE;
            return (true);
        }
    } else {
        Logger::error << "Invalid client_max_body_size." << std::endl;
        return (true);
    }
    return (false);
}

void HttpRequest::_getServerParam(Parser &parser)
{
    std::vector<int> serverSize = parser.getSizeServers();

    this->_serverIndex   = this->_findServerIndex(parser, serverSize[0], this->_port);
    this->_locationSize  = serverSize[this->_serverIndex + 1];
    this->_locationIndex = this->_findLocationIndex(parser);
    this->_setRoot(parser);
    this->_setLimitExcept(parser);
    this->_setErrorPage(parser);
}

int HttpRequest::_findServerIndex(Parser &parser, int serverSize, std::string port)
{
    std::vector<std::string> listenParam;
    int                      i = 0;

    for (i = 0; i != serverSize; ++i) {
        listenParam = parser.getServerParam(i, "listen");
        if (listenParam.empty()) {
            continue;
        }
        if (port == listenParam[0]) {
            break;
        }
    }
    return (i);
}

int HttpRequest::_findLocationIndex(Parser &parser)
{
    std::vector<std::string>           locationParam;
    std::vector<std::string>::iterator it;
    int                                i = 0;
    std::string                        found, path;

    path  = this->_extractPathFromURI();
    found = path.empty() ? this->_uri : path;
    for (i = 0; i < this->_locationSize; i++) {
        locationParam = parser.getLocationParam(this->_serverIndex, i, "location");
        it            = std::find(locationParam.begin(), locationParam.end(), found);
        if (it != locationParam.end()) {
            break;
        }
    }
    return (i);
}

std::string HttpRequest::_extractPathFromURI(void)
{
    size_t      pos;
    std::string path;

    if (this->_uri.length() == 1)
        return ("");
    pos = this->_uri.find("/", 1);
    if (pos == std::string::npos) {
        path = this->_uri;
    } else {
        path = this->_uri.substr(0, pos);
    }
    return (path);
}

void HttpRequest::_setRoot(Parser &parser)
{
    std::vector<std::string> rootParam;

    this->_root = DEFAULT_ROOT;
    rootParam   = parser.getServerParam(this->_serverIndex, "root");
    if (!rootParam.empty()) {
        this->_root = rootParam[0];
    }
    if (this->_locationSize != this->_locationIndex) {
        rootParam = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "root");
        if (!rootParam.empty()) {
            this->_root = rootParam[0];
        }
    }
    return;
}

void HttpRequest::_setErrorPage(Parser &parser)
{
    if (this->_locationSize != this->_locationIndex) {
        this->_errorPageConfig
            = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "error_page");
        if (!this->_errorPageConfig.empty()) {
            return;
        }
    }
    this->_errorPageConfig = parser.getServerParam(this->_serverIndex, "error_page");
    return;
}

void HttpRequest::_setLimitExcept(Parser &parser)
{
    this->_limitExcept.clear();
    if (this->_locationSize != this->_locationIndex) {
        this->_limitExcept
            = parser.getLocationParam(this->_serverIndex, this->_locationIndex, "limit_except");
    }
    return;
}

void HttpRequest::_setAutoIndex(Parser &parser)
{
    std::vector<int>                   serverSize = parser.getSizeServers();
    std::vector<std::string>           server;
    std::vector<std::string>           locationParam;
    std::vector<std::string>::iterator it;

    autoIndexServer = false;
    autoIndexLoc    = false;
    server          = parser.getServerParam(this->_serverIndex, "autoindex");
    if (!server.empty() && server[0] == "on")
        autoIndexServer = true;
    else {
        int loc = 1;
        for (int i = 0; i < serverSize[0]; i++) {
            for (int j = 0; j < serverSize[loc]; j++) {
                std::vector<std::string> locationPath = parser.getLocationParam(i, j, "location");
                if (!locationPath.empty() && _uri == locationPath[0]) {
                    std::vector<std::string> autoindexParam
                        = parser.getLocationParam(i, j, "autoindex");
                    for (size_t k = 0; k < autoindexParam.size(); k++) {
                        if (autoindexParam[k] == "on") {
                            _path        = locationPath[0];
                            autoIndexLoc = true;
                        }
                    }
                }
            }
            loc++;
        }
    }
}
