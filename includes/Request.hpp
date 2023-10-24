/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/24 14:26:15 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "ErrorPage.hpp"
#include "utils.hpp"

class HttpRequest {
    public:
    HttpRequest();
    ~HttpRequest();

    bool requestHttp(std::string request, Parser &parser);
    void init();

    std::string                        getMethod(void) const;
    std::string                        getPort(void) const;
    std::string                        getHost(void) const;
    std::string                        getUri(void) const;
    std::string                        getHttp(void) const;
    std::string                        getBody(void) const;
    std::string                        getBoundary(void) const;
    std::vector<std::string>           getQuery(void) const;
    std::map<std::string, std::string> getHeaders(void) const;
    int                                getServerIndex(void) const;
    int                                getLocationIndex(void) const;
    int                                getLocationSize(void) const;
    size_t                             getMaxBodySize(void) const;
    size_t                             getContentLength(void) const;
    std::string                        getRoot(void) const;
    std::string                        getPath(void) const;
    std::vector<std::string>           getErrorPageConfig(void) const;
    std::vector<std::string>           getLimitExcept(void) const;

    bool        has_body;
    bool        has_form;
    bool        has_multipart;
    bool        autoIndexServer;
    bool        autoIndexLoc;
    int         statusCode;
    std::string content;

    private:
    void        _parseHeaders(const std::string &request);
    bool        _parseFirstLine(std::string &requestLine);
    void        _findHeaders(std::string key, std::string value);
    void        _parseQuery(void);
    bool        _getMultipartData(std::string request);
    void        _getMaxBody(Parser &parser);
    void        _getServerParam(Parser &parser);
    void        _getHost();
    bool        _getBody(std::string request);
    int         _findServerIndex(Parser &parser, int serverSize, std::string port);
    int         _findLocationIndex(Parser &parser);
    std::string _extractPathFromURI(void);
    void        _setRoot(Parser &parser);
    void        _setErrorPage(Parser &parser);
    void        _setLimitExcept(Parser &parser);
    void        _setAutoIndex(Parser &parser);

    size_t                              _maxBodySize;
    size_t                              _contentLength;
    std::string                        _uri;
    std::string                        _body;
    std::string                        _port;
    std::string                        _host;
    std::string                        _method;
    std::string                        _boundary;
    std::string                        _httpVersion;
    std::vector<std::string>           _paramQuery;
    std::map<std::string, std::string> _header;
    int                                _serverIndex;
    int                                _locationIndex;
    int                                _locationSize;
    std::string                        _root;
    std::string                        _path;
    std::vector<std::string>			_allowMethods;
    std::vector<std::string>           _errorPageConfig;
    std::vector<std::string>           _limitExcept;
};
