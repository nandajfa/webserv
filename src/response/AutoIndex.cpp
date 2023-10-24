/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 08:46:45 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/24 14:23:35 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

AutoIndex::AutoIndex() {}

AutoIndex::AutoIndex(HttpRequest request) : _request(request) {}

AutoIndex::~AutoIndex() {}

responseData AutoIndex::autoIndex(std::string root, std::string path, std::string port)
{
    std::string dirPath = root + path;

    DIR *dir = opendir(dirPath.c_str());
    if (dir == NULL) {
        _responseData = _errorPage.getErrorPageContent(this->_request.getErrorPageConfig(),
                                                       NOT_FOUND,
                                                       this->_request.getUri(),
                                                       this->_request.getRoot());
        return (_responseData);
    }

    std::string    content = "<html><body><h2>Index of: " + dirPath + "</h2><ul>";
    struct dirent *entry;
    for (entry = readdir(dir); entry; entry = readdir(dir)) {
        std::string entryPath
            = port + path + (path[path.size() - 1] != '/' ? "/" : "") + std::string(entry->d_name);
        content += "<li><a href=\"http://localhost:" + entryPath + "\">"
            + std::string(entry->d_name) + "</a></li>\n";
    }

    content += "</ul></body></html>";
    _responseData = setResponseData(OK, "text/html", content, content.length());

    closedir(dir);
    return (_responseData);
}
