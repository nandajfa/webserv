/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 13:41:36 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/19 10:04:09 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandlers.hpp"

ResponseHandlers::ResponseHandlers(void) { CGI _cgi; }

ResponseHandlers::~ResponseHandlers(void) {}

responseData ResponseHandlers::exec(Parser &parser, HttpRequest &request)
{
    this->_res = setResponseData(0, "", "", -1);

    if (!this->_verifyServerName(request, parser)) {
        return (this->_errorPage.getErrorPageContent(
            request.getErrorPageConfig(), BAD_REQUEST, request.getUri(), request.getRoot()));
    }
    if (this->_verifyRedirection(request, parser)) {
        return (this->_res);
    }
    if (!this->_methodAllowed(request)) {
        return (this->_errorPage.getErrorPageContent(
            request.getErrorPageConfig(), METHOD_NOT_ALLOWED, request.getUri(), request.getRoot()));
    }
    switch (this->_resolveOption(request.getMethod())) {
        case GET:
            this->_getHandler(request, parser);
            break;
        case POST:
            this->_postHandler(request, parser);
            break;
        case DELETE:
            this->_deleteHandler(request);
            break;
        default:
            break;
    }
    return (this->_res);
}

bool ResponseHandlers::_methodAllowed(HttpRequest &request)
{
    if (request.getLimitExcept().empty()) {
        return (true);
    }
    for (size_t i = 0; i < request.getLimitExcept().size(); i++) {
        if (request.getLimitExcept()[i] == request.getMethod()) {
            return (true);
        }
    }
    return (false);
}

bool ResponseHandlers::_verifyServerName(HttpRequest &request, Parser &parser)
{
    std::vector<std::string> server_names
        = parser.getServerParam(request.getServerIndex(), "server_name");
    if (server_names.empty())
        return (true);
    for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end();
         ++it) {
        if (*it == request.getHost() || request.getHost() == "127.0.0.1")
            return (true);
    }
    return (false);
}

bool ResponseHandlers::_verifyRedirection(HttpRequest &request, Parser &parser)
{
    std::vector<std::string> server_redirection
        = parser.getServerParam(request.getServerIndex(), "redirect");
    std::vector<std::string>::iterator it_s = server_redirection.begin();
    if (!server_redirection.empty()) {
        this->_res = setResponseData(std::atoi(it_s[0].c_str()), "", "", 0, it_s[1]);
        return (true);
    }
    std::vector<std::string> location_redirection
        = parser.getLocationParam(request.getServerIndex(), request.getLocationIndex(), "redirect");
    std::vector<std::string>::iterator it_p = location_redirection.begin();
    if (!location_redirection.empty()) {
        this->_res = setResponseData(std::atoi(it_p[0].c_str()), "", "", 0, it_p[1]);
        return (true);
    }
    return (false);
}

int ResponseHandlers::_resolveOption(std::string method)
{
    std::string option[] = {"GET", "POST", "DELETE"};
    int         i        = 0;

    while (i < 3 && method != option[i])
        i++;
    return (i);
}

responseData ResponseHandlers::_getCgi(HttpRequest &request)
{
    responseData res;

    std::string cgi_response = _cgi.executeCGI(request);
    res = setResponseData(OK, "text/html", cgi_response.c_str(), (int)cgi_response.length());
    return (res);
}

void ResponseHandlers::_getHandler(HttpRequest &request, Parser &parser)
{
    Location  location(request);
    AutoIndex autoIndex(request);

    if (request.autoIndexServer && request.getUri() == "/autoindex")
        this->_res = autoIndex.autoIndex(request.getRoot(), "/", request.getPort());
    else if (request.autoIndexLoc)
        this->_res = autoIndex.autoIndex(request.getRoot(), request.getPath(), request.getPort());
    else if (Constants::isCgi(extractFileExtension(request.getUri()))
             && _cgi.isCGI(request, parser)) {
        this->_res = this->_getCgi(request);
    } else {
        location.setup(parser);
        this->_res = location.getLocationContent();
    }
}

void ResponseHandlers::_postHandler(HttpRequest &request, Parser &parser)
{
    PostMethod post_method(request);

    if (Constants::isCgi(extractFileExtension(request.getUri())) && _cgi.isCGI(request, parser)) {
        this->_res = this->_getCgi(request);
    } else {
        this->_res = post_method.handleMethod();
    }
}

void ResponseHandlers::_deleteHandler(HttpRequest &request)
{
    DeleteMethod delete_method(request);

    Logger::warning << "Delete Method" << std::endl;

    this->_res = delete_method.handleMethod();
}
