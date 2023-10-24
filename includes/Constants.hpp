/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:55:15 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 20:32:13 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"

class Constants {
    private:
    static const std::map<std::string, std::string> _mimeTypes;
    static const std::map<std::string, std::string> _statusCodes;
    static const std::set<std::string>              _cgiSupported;
    static std::map<std::string, std::string>       setMimeTypes();
    static std::map<std::string, std::string>       setStatusCodes();
    static std::set<std::string>                    setCgiSupported();

    public:
    Constants();
    ~Constants();

    static std::string getMimeTypes(std::string extension);
    static std::string getStatusCodes(std::string status);
    static bool        isCgi(std::string extension);
};
