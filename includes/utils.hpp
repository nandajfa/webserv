/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:09:00 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/15 08:57:37 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Constants.hpp"
#include "Libs.hpp"
#include "Parser.hpp"

responseData getJson(std::string body, int status);
responseData getContent(std::string root, std::string file, int status);
std::string  extractFileExtension(std::string file);
responseData setResponseData(int         status,
                             std::string contentType,
                             std::string content,
                             int         contentLength,
                             std::string location = "");
std::string  vector_join(std::vector<std::string> vec, std::string delimiter);

template <typename ValueType>
std::string to_string(ValueType v);
std::string getDir();
std::string getBin(const std::string &url);
