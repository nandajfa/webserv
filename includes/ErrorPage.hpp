/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 05:35:10 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/16 18:35:18 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "utils.hpp"

class ErrorPage {
    private:
    bool        _hasErrorPageConfig(std::vector<std::string> errorPage, int statusCode);
    std::string _getPath(std::string uri);

    public:
    ErrorPage(void);
    ~ErrorPage(void);

    responseData getErrorPageContent(std::vector<std::string> errorPage,
                                     int                      statusCode,
                                     std::string              uri,
                                     std::string              root);
    responseData getErrorPageStandard(int statusCode);
};
