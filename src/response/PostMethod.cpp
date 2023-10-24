/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:27:05 by jefernan          #+#    #+#             */
/*   Updated: 2023/10/19 14:51:17 by jefernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostMethod.hpp"

PostMethod::PostMethod() {}

PostMethod::~PostMethod() {}

PostMethod::PostMethod(HttpRequest request) : AHttpResponse(request) {}

responseData PostMethod::handleMethod()
{
    created = false;

    if (_req.has_body) {
        if (_req.has_multipart) {
            if (handleMultipart())
            {
                _res = _errorPage.getErrorPageContent(_req.getErrorPageConfig(),
                                                      ENTITY_TOO_LARGE,
                                                      _req.getUri(),
                                                      _req.getRoot());
                Logger::error << "Request Entity Too Large." << std::endl;
                return (_res);
            }
            if (created && _file == true) {
                _res = getJson(
                    "{\"status\": \"success\", \"message\": \"Resource created successfully\"}",
                    201);
                Logger::info << "File created." << std::endl;
                return (_res);
            } else if (!created && _file == true) {
                _res = _errorPage.getErrorPageContent(_req.getErrorPageConfig(),
                                                      INTERNAL_SERVER_ERROR,
                                                      _req.getUri(),
                                                      _req.getRoot());
                Logger::error << "Unable to create file." << std::endl;
                return (_res);
            }
        }
        if (_req.has_form)
            handleForm();
        else
            std::cout << "Body: " << _req.getBody() << "\n";

        _res = getJson("{\"status\": \"success\", \"message\": \"Successful operation\"}", OK);
        Logger::info << "Post request completed successfully." << std::endl;
    } else if (!_req.has_body) {
        _res = _errorPage.getErrorPageContent(
            _req.getErrorPageConfig(), BAD_REQUEST, _req.getUri(), _req.getRoot());
        Logger::info << "No content." << std::endl;
    } else {
        _res = _errorPage.getErrorPageContent(
            _req.getErrorPageConfig(), INTERNAL_SERVER_ERROR, _req.getUri(), _req.getRoot());
        Logger::error << "Internal Server Error." << std::endl;
    }
    return (_res);
}

bool PostMethod::handleMultipart()
{
    std::string boundary = _req.getBoundary();
    std::string body     = _req.getBody();

    _formData.clear();
    _file       = false;
    size_t pos  = 0;
    _bodySize   = 0;
    while ((pos = body.find(boundary, pos)) != std::string::npos) {
        pos += boundary.length();
        size_t endPos = body.find(boundary, pos);
        if (endPos != std::string::npos)
            parseMultipartFormData(pos, endPos);
    }
    if (verifyLimit())
        return (true);
    if (_file == false)
        print();
    return (false);
}

bool    PostMethod::verifyLimit()
{
    if ((_bodySize / 1024) > _req.getMaxBodySize() ||
        (_req.getContentLength() / 1024) > _req.getMaxBodySize())
        return (true);
    return (false);
}

std::string setFileName(size_t pos, std::string data)
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    int randomNumber = std::rand() % 500;

    std::ostringstream oss;
    oss << "file" << randomNumber;

    std::size_t filenameEnd = data.find("\"", pos);
    std::string aux         = data.substr(pos, filenameEnd - pos);
    size_t      dot         = aux.find(".");
    if (dot != std::string::npos){
        std::string extension = aux.substr(dot);
        oss << extension;
    }

    std::string fileName = oss.str();
    return (fileName);
}

void PostMethod::parseMultipartFormData(size_t pos, size_t endPos)
{
    std::string body     = _req.getBody();
    std::string partData = body.substr(pos, endPos - pos);
    std::size_t bodyEnd  = partData.find("\r\n\r\n");

    if (bodyEnd != std::string::npos) {
        std::string data  = partData.substr(0, bodyEnd);
        std::string value = partData.substr(bodyEnd + 4);

        std::size_t namePos = data.find("name=\"");
        if (namePos != std::string::npos) {
            namePos += 6;
            std::size_t nameEnd = data.find("\"", namePos);
            if (nameEnd != std::string::npos) {
                std::string name        = data.substr(namePos, nameEnd - namePos);
                std::size_t filenamePos = data.find("filename=\"");

                if (filenamePos != std::string::npos) {
                    filenamePos += 10;
                    std::size_t filenameEnd = data.find("\"", filenamePos);
                    if (filenameEnd != std::string::npos) {
                        std::string fileName = setFileName(filenamePos, data);
                        if ((_req.getContentLength() / 1024) < _req.getMaxBodySize())
                            saveFile(fileName, value);
                        _file = true;
                    }
                } else{
                    _bodySize += value.size();
                    _formData[name] = value;
                }
            }
        }
    }
}

void PostMethod::saveFile(std::string &fileName, const std::string &value)
{
    std::string resourcePath = getDir();

    resourcePath = resourcePath + "/" + this->_req.getRoot() + "/method/" + fileName;

    std::ifstream checkFile(resourcePath.c_str());
    if (checkFile.good()) {
        created = false;
        Logger::info << "The file already exists." << std::endl;
        return;
    }
    std::ofstream file(resourcePath.c_str(), std::ios::binary);
    if (file.is_open()) {
        file.write(value.c_str(), value.length());
        file.close();
        created = true;
        Logger::info << "File path: " << resourcePath << std::endl;
    } else
        created = false;
}

std::string replaceChar(const std::string &input)
{
    std::string output;
    size_t      pos = 0;

    while (pos < input.length()) {
        if (input[pos] == '%') {
            if (pos + 2 < input.length()) {
                char hex[3]      = {input[pos + 1], input[pos + 2], '\0'};
                int  decodedChar = strtol(hex, NULL, 16);
                output.push_back(static_cast<char>(decodedChar));
                pos += 2;
            }
        } else if (input[pos] == '+')
            output.push_back(' ');
        else
            output.push_back(input[pos]);
        pos++;
    }
    return output;
}

void PostMethod::handleForm()
{
    std::string        body = _req.getBody();
    std::istringstream ss(body);
    std::string        pair;

    while (std::getline(ss, pair, '&')) {
        size_t equal = pair.find('=');
        if (equal != std::string::npos) {
            std::string fieldName = replaceChar(pair.substr(0, equal));
            std::string fielddata = replaceChar(pair.substr(equal + 1));
            _formData[fieldName]  = fielddata + "\r\n";
        }
    }
    print();
}

void PostMethod::print()
{
    std::cout << P_BLUE << std::endl;
    std::cout << "      ------------ Content Form ------------" << std::endl;
    std::cout << std::left << std::setw(20) << "Key" << std::setw(30) << "| Value" << std::endl;
    std::cout << std::setfill('-') << std::setw(50) << "-" << std::setfill(' ') << std::endl;

    std::map<std::string, std::string>::iterator it;
    for (it = _formData.begin(); it != _formData.end(); ++it)
        std::cout << std::left << std::setw(19) << it->first << " | " << it->second;
    std::cout << std::setfill('-') << std::setw(50) << "-" << std::setfill(' ') << std::endl;
    std::cout << RESET_COLOR << std::endl;
}
