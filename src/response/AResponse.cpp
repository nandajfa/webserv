/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:25:37 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/13 16:02:45 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AResponse.hpp"

AHttpResponse::AHttpResponse(void) {}

AHttpResponse::AHttpResponse(HttpRequest request) : _req(request) {}

AHttpResponse::~AHttpResponse(void) {}
