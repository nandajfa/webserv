/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 08:47:30 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/17 09:54:28 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorPage.hpp"
#include "Libs.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "ResponseHandlers.hpp"
#include "utils.hpp"

class AutoIndex {
    public:
    AutoIndex(void);
    AutoIndex(HttpRequest request);
    ~AutoIndex(void);

    responseData autoIndex(std::string root, std::string path, std::string port);

    private:
    // void			setResponse(std::string statusCode, std::string content);
    responseData _responseData;
    HttpRequest  _request;
    ErrorPage    _errorPage;
};
