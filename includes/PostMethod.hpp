/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 18:31:10 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/17 15:56:21 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AResponse.hpp"

class PostMethod : public AHttpResponse {

    public:
    PostMethod();
    PostMethod(HttpRequest request);
    ~PostMethod();

    responseData    handleMethod();
    bool            verifyLimit();
    bool            handleMultipart();
    void            handleForm();
    void            print();
    void            saveFile(std::string &fileName, const std::string &value);
    void            parseMultipartFormData(size_t pos, size_t endPos);

    bool created;

    private:
    bool                               _file;
    size_t                                 _bodySize;
    std::map<std::string, std::string> _formData;
};
