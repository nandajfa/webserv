/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:17:35 by mcl               #+#    #+#             */
/*   Updated: 2023/10/19 10:33:16 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteMethod.hpp"

DeleteMethod::DeleteMethod(void) {}

DeleteMethod::DeleteMethod(HttpRequest request) : AHttpResponse(request) {}

DeleteMethod::~DeleteMethod(void) {}

responseData DeleteMethod::handleMethod(void)
{
    std::string resourcePath = getDir();

    resourcePath = resourcePath + "/" + this->_req.getRoot() + this->_req.getUri();

    Logger::warning << "resourcePath: " << resourcePath << std::endl;

    std::ifstream file(resourcePath.c_str());

    if (file.is_open()) {
        file.close();
        if (std::remove(resourcePath.c_str()) == 0) {
            this->_res = setResponseData(NO_CONTENT, "", "", 0);
            Logger::info << "Resource deleted successfully." << std::endl;
        } else {
            this->_res = this->_errorPage.getErrorPageContent(this->_req.getErrorPageConfig(),
                                                              INTERNAL_SERVER_ERROR,
                                                              this->_req.getUri(),
                                                              this->_req.getRoot());
            Logger::info << "Error deleting resource." << std::endl;
        }
    } else {
        this->_res = this->_errorPage.getErrorPageContent(
            this->_req.getErrorPageConfig(), NOT_FOUND, this->_req.getUri(), this->_req.getRoot());
        Logger::info << "Resource not found." << std::endl;
    }
    return (this->_res);
}
