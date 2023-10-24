/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 05:34:14 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/19 10:03:44 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPage.hpp"
#include "Logger.hpp"

ErrorPage::ErrorPage(void) {}

ErrorPage::~ErrorPage(void) {}

responseData ErrorPage::getErrorPageContent(std::vector<std::string> errorPage,
                                            int                      statusCode,
                                            std::string              uri,
                                            std::string              root)
{
    std::string  filePath;
    responseData res;

    if (this->_hasErrorPageConfig(errorPage, statusCode)) {
        filePath = this->_getPath(uri);
        if (filePath[filePath.length() - 1] != '/') {
            filePath += '/';
        }
        filePath += errorPage.back();
        res = getContent(root, filePath, statusCode);
        if (res.contentLength) {
            return (res);
        }
    }
    res = this->getErrorPageStandard(statusCode);
    return (res);
}

responseData ErrorPage::getErrorPageStandard(int statusCode)
{
    responseData res;

    std::string standardErrorFile = "/";
    standardErrorFile += to_string(statusCode);
    standardErrorFile += ".html";
    res = getContent(DEFAULT_ERROR_ROOT, standardErrorFile, statusCode);
    if (res.contentLength) {
        return (res);
    }
    res = getJson("{\"error\": \"Error page not configured\"}", statusCode);
    return (res);
}

bool ErrorPage::_hasErrorPageConfig(std::vector<std::string> errorPage, int statusCode)
{
    for (std::vector<std::string>::iterator it = errorPage.begin(); it != errorPage.end(); ++it) {
        if (*it == to_string(statusCode)) {
            return (true);
        }
    }
    return (false);
}

std::string ErrorPage::_getPath(std::string uri)
{
    size_t firstSlashPos;
    size_t lastSlashPos;
    size_t dotPos;

    if (uri.length() == 1)
        return ("/");
    firstSlashPos = uri.find('/', 1);
    if (firstSlashPos == std::string::npos) {
        return ("/");
    }
    dotPos = uri.rfind('.');
    if (dotPos == std::string::npos) {
        return (uri);
    }
    lastSlashPos = uri.rfind('/');
    return (uri.substr(0, lastSlashPos));
}
