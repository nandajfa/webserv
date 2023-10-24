/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:56:35 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 20:46:33 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Constants.hpp"

const std::map<std::string, std::string> Constants::_mimeTypes    = Constants::setMimeTypes();
const std::map<std::string, std::string> Constants::_statusCodes  = Constants::setStatusCodes();
const std::set<std::string>              Constants::_cgiSupported = Constants::setCgiSupported();

Constants::Constants() {}

Constants::~Constants() {}

bool Constants::isCgi(std::string extension)
{
    return (Constants::_cgiSupported.find(extension) != Constants::_cgiSupported.end());
}

std::string Constants::getStatusCodes(std::string status)
{
    std::map<std::string, std::string>::const_iterator it = Constants::_statusCodes.find(status);
    std::string                                        statusText;
    std::string                                        statusCode = "";

    if (it != Constants::_statusCodes.end()) {
        statusText = it->second;
        statusCode = status + " " + statusText;
    }
    return (statusCode);
}

std::string Constants::getMimeTypes(std::string extension)
{
    std::map<std::string, std::string>::const_iterator it = Constants::_mimeTypes.find(extension);

    if (it != Constants::_mimeTypes.end()) {
        return (it->second);
    } else {
        return ("application/octet-stream");
    }
}

std::map<std::string, std::string> Constants::setMimeTypes()
{
    std::map<std::string, std::string> mimeTypes;

    mimeTypes[".aac"]  = "audio/aac";
    mimeTypes[".abw"]  = "application/x-abiword";
    mimeTypes[".arc"]  = "application/x-freearc";
    mimeTypes[".avif"] = "image/avif";
    mimeTypes[".avi"]  = "video/x-msvideo";
    mimeTypes[".azw"]  = "application/vnd.amazon.ebook";
    mimeTypes[".bin"]  = "application/octet-stream";
    mimeTypes[".bmp"]  = "image/bmp";
    mimeTypes[".bz"]   = "application/x-bzip";
    mimeTypes[".bz2"]  = "application/x-bzip2";
    mimeTypes[".cda"]  = "application/x-cdf";
    mimeTypes[".csh"]  = "application/x-csh";
    mimeTypes[".css"]  = "text/css";
    mimeTypes[".csv"]  = "text/csv";
    mimeTypes[".doc"]  = "application/msword";
    mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mimeTypes[".eot"]  = "application/vnd.ms-fontobject";
    mimeTypes[".epub"] = "application/epub+zip";
    mimeTypes[".gz"]   = "application/gzip";
    mimeTypes[".gif"]  = "image/gif";
    mimeTypes[".htm"]  = "text/html";
    mimeTypes[".html"] = "text/html";
    mimeTypes[".ico"]  = "image/vnd.microsoft.icon";
    mimeTypes[".ics"]  = "text/calendar";
    mimeTypes[".jar"]  = "application/java-archive";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".jpg"]  = "image/jpeg";
    mimeTypes[".js"]   = "text/javascript";
    mimeTypes[".json"] = "application/json";
    mimeTypes[".jsonld"] = "application/ld+json";
    mimeTypes[".mid"]    = "audio/midi, audio/x-midi";
    mimeTypes[".midi"]   = "audio/midi, audio/x-midi";
    mimeTypes[".mjs"]    = "text/javascript";
    mimeTypes[".mp3"]    = "audio/mpeg";
    mimeTypes[".mp4"]    = "video/mp4";
    mimeTypes[".mpeg"]   = "video/mpeg";
    mimeTypes[".mpkg"]   = "application/vnd.apple.installer+xml";
    mimeTypes[".odp"]    = "application/vnd.oasis.opendocument.presentation";
    mimeTypes[".ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
    mimeTypes[".odt"]    = "application/vnd.oasis.opendocument.text";
    mimeTypes[".oga"]    = "audio/ogg";
    mimeTypes[".ogv"]    = "video/ogg";
    mimeTypes[".ogx"]    = "application/ogg";
    mimeTypes[".opus"]   = "audio/opus";
    mimeTypes[".otf"]    = "font/otf";
    mimeTypes[".png"]    = "image/png";
    mimeTypes[".pdf"]    = "application/pdf";
    mimeTypes[".php"]    = "application/x-httpd-php";
    mimeTypes[".ppt"]    = "application/vnd.ms-powerpoint";
    mimeTypes[".pptx"]
        = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mimeTypes[".rar"]   = "application/vnd.rar";
    mimeTypes[".rtf"]   = "application/rtf";
    mimeTypes[".sh"]    = "application/x-sh";
    mimeTypes[".svg"]   = "image/svg+xml";
    mimeTypes[".tar"]   = "application/x-tar";
    mimeTypes[".tif"]   = "image/tiff";
    mimeTypes[".tiff"]  = "image/tiff";
    mimeTypes[".ts"]    = "video/mp2t";
    mimeTypes[".ttf"]   = "font/ttf";
    mimeTypes[".txt"]   = "text/plain";
    mimeTypes[".vsd"]   = "application/vnd.visio";
    mimeTypes[".wav"]   = "audio/wav";
    mimeTypes[".weba"]  = "audio/webm";
    mimeTypes[".webm"]  = "video/webm";
    mimeTypes[".webp"]  = "image/webp";
    mimeTypes[".woff"]  = "font/woff";
    mimeTypes[".woff2"] = "font/woff2";
    mimeTypes[".xhtml"] = "application/xhtml+xml";
    mimeTypes[".xls"]   = "application/vnd.ms-excel";
    mimeTypes[".xlsx"]  = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mimeTypes[".xml"]   = "application/xml";
    mimeTypes[".xul"]   = "application/vnd.mozilla.xul+xml";
    mimeTypes[".zip"]   = "application/zip";
    mimeTypes[".3gp"]   = "video/3gpp; audio/3gpp ";
    mimeTypes[".3g2"]   = "video/3gpp2; audio/3gpp2";
    mimeTypes[".7z"]    = "application/x-7z-compressed";
    return (mimeTypes);
}

std::map<std::string, std::string> Constants::setStatusCodes()
{
    std::map<std::string, std::string> statusCodes;

    statusCodes["100"] = "Continue";
    statusCodes["101"] = "Switching Protocols";
    statusCodes["102"] = "Processing";
    statusCodes["103"] = "Early Hints";
    statusCodes["200"] = "OK";
    statusCodes["201"] = "Created";
    statusCodes["202"] = "Accepted";
    statusCodes["203"] = "Non-Authoritative Information";
    statusCodes["204"] = "No Content";
    statusCodes["205"] = "Reset Content";
    statusCodes["206"] = "Partial Content";
    statusCodes["207"] = "Multi-Status";
    statusCodes["208"] = "Already Reported";
    statusCodes["226"] = "IM Used";
    statusCodes["300"] = "Multiple Choices";
    statusCodes["301"] = "Moved Permanently";
    statusCodes["302"] = "Found";
    statusCodes["303"] = "See Other";
    statusCodes["304"] = "Not Modified";
    statusCodes["307"] = "Temporary Redirect";
    statusCodes["308"] = "Permanent Redirect";
    statusCodes["400"] = "Bad Request";
    statusCodes["401"] = "Unauthorized";
    statusCodes["402"] = "Payment Required";
    statusCodes["403"] = "Forbidden";
    statusCodes["404"] = "Not Found";
    statusCodes["405"] = "Method Not Allowed";
    statusCodes["406"] = "Not Acceptable";
    statusCodes["407"] = "Proxy Authentication Required";
    statusCodes["408"] = "Request Timeout";
    statusCodes["409"] = "Conflict";
    statusCodes["410"] = "Gone";
    statusCodes["411"] = "Length Required";
    statusCodes["412"] = "Precondition Failed";
    statusCodes["413"] = "Payload Too Large";
    statusCodes["414"] = "URI Too Long";
    statusCodes["415"] = "Unsupported Media Type";
    statusCodes["416"] = "Range Not Satisfiable";
    statusCodes["417"] = "Expectation Failed";
    statusCodes["418"] = "I'm a teapot";
    statusCodes["421"] = "Misdirected Request";
    statusCodes["422"] = "Unprocessable Content";
    statusCodes["423"] = "Locked";
    statusCodes["424"] = "Failed Dependency";
    statusCodes["425"] = "Too Early";
    statusCodes["426"] = "Upgrade Required";
    statusCodes["428"] = "Precondition Required";
    statusCodes["429"] = "Too Many Requests";
    statusCodes["431"] = "Request Header Fields Too Large";
    statusCodes["451"] = "Unavailable For Legal Reasons";
    statusCodes["500"] = "Internal Server Error";
    statusCodes["501"] = "Not Implemented";
    statusCodes["502"] = "Bad Gateway";
    statusCodes["503"] = "Service Unavailable";
    statusCodes["504"] = "Gateway Timeout";
    statusCodes["505"] = "HTTP Version Not Supported";
    statusCodes["506"] = "Variant Also Negotiates";
    statusCodes["507"] = "Insufficient Storage";
    statusCodes["508"] = "Loop Detected";
    statusCodes["510"] = "Not Extended";
    statusCodes["511"] = "Network Authentication Required";
    return (statusCodes);
}

std::set<std::string> Constants::setCgiSupported()
{
    std::set<std::string> data;

    data.insert(".php");
    data.insert(".py");
    data.insert(".rb");
    data.insert(".pl");
    return (data);
}
