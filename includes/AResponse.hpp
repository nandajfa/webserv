/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 06:59:01 by mcl               #+#    #+#             */
/*   Updated: 2023/10/13 16:01:11 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorPage.hpp"
#include "Libs.hpp"
#include "Request.hpp"

class AHttpResponse {
    protected:
    HttpRequest  _req;
    responseData _res;
    ErrorPage    _errorPage;

    public:
    AHttpResponse(void);
    AHttpResponse(HttpRequest request);
    virtual ~AHttpResponse(void);

    virtual responseData handleMethod(void) = 0;
};
