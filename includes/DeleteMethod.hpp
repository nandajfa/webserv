/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 07:12:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/14 09:10:52 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"
#include "Constants.hpp"
#include "utils.hpp"

class DeleteMethod : public AHttpResponse {
    public:
    DeleteMethod(void);
    DeleteMethod(HttpRequest request);
    ~DeleteMethod(void);

    responseData handleMethod(void);
};
