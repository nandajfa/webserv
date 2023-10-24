/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:38:53 by mcl               #+#    #+#             */
/*   Updated: 2023/10/20 23:50:00 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"

CGI::CGI() { _is_cgi = false; }

CGI::~CGI() {}

std::map<std::string, std::string> CGI::assembleCGIHeaders(const HttpRequest &request)
{

    std::map<std::string, std::string> map_envs;

    map_envs["AUTH_TYPE"]         = "";
    map_envs["REDIRECT_STATUS"]   = "200";
    map_envs["GATEWAY_INTERFACE"] = "CGI/1.1";
    map_envs["SCRIPT_NAME"]       = _cgi_path;
    map_envs["SCRIPT_FILENAME"]   = _cgi_path;
    map_envs["REQUEST_METHOD"]    = request.getMethod();
    map_envs["CONTENT_LENGTH"]    = request.getBody().length();
    map_envs["CONTENT_TYPE"]      = "text/html";
    map_envs["PATH_INFO"]         = _cgi_path;
    map_envs["PATH_TRANSLATED"]   = _cgi_path;
    map_envs["QUERY_STRING"]      = queryToString(request.getQuery());
    map_envs["REQUEST_BODY"]      = request.getBody();
    map_envs["REMOTE_ADDR"]       = "";
    map_envs["REMOTE_IDENT"]      = "";
    map_envs["REMOTE_USER"]       = "";
    map_envs["REQUEST_URI"]       = _cgi_path;
    map_envs["SERVER_NAME"]       = request.getHost();
    map_envs["SERVER_PROTOCOL"]   = "HTTP/1.1";
    map_envs["SERVER_SOFTWARE"]   = "Webserv/1.0";

    return (map_envs);
}

char **CGI::assembleCGIEnv(std::map<std::string, std::string> map_envs)
{

    int    size = map_envs.size();
    char **envs = new char *[size + 1];
    int    i    = 0;

    for (std::map<std::string, std::string>::iterator it = map_envs.begin(); it != map_envs.end();
         it++, i++) {
        std::string env = it->first + "=" + it->second;
        envs[i]         = new char[env.length() + 1];
        strcpy(envs[i], env.c_str());
    }

    envs[size] = 0;
    return (envs);
}

bool CGI::programExists(const std::string &path, const HttpRequest &request)
{
    struct stat info;

    _bin = getBin(request.getUri());

    if (stat(path.c_str(), &info) != 0 || _bin == "") {
        return (false);
    }
    return (true);
}

bool CGI::isCGI(const HttpRequest &request, Parser &parser)
{
    std::string defaultRoot = DEFAULT_ROOT_CGI;
    std::string rootTmp     = getDir();
    std::string tmp_path;

    _is_cgi = false;

    int serverIndex                    = getSIndex(parser, parser.getServers(), request.getPort());
    std::vector<std::string> rootParam = parser.getServerParam(serverIndex, "root");
    size_t                   pos       = defaultRoot.find("/");
    if (pos != std::string::npos && rootParam.size() > 0) {
        tmp_path = rootTmp + "/" + rootParam[0] + defaultRoot.substr(pos) + request.getUri();
        if (programExists(tmp_path, request)) {
            _cgi_path = tmp_path;
            _is_cgi   = true;
            return (_is_cgi);
        }
    }
    tmp_path = rootTmp + "/" + defaultRoot + request.getUri();
    if (programExists(tmp_path, request)) {
        _cgi_path = tmp_path;
        _is_cgi   = true;
    }
    return (_is_cgi);
}

std::string CGI::executeCGI(const HttpRequest &request)
{
    pid_t           pid;
    int             pipe_fd[2];
    std::string     body;
    unsigned int    timeout = 10000;
    struct timespec startTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    if (_bin.empty())
        Logger::info << "There are not this program." << std::endl;

    _envs = assembleCGIEnv(assembleCGIHeaders(request));

    char *cbin    = const_cast<char *>(_bin.c_str());
    char *pathBin = const_cast<char *>(_cgi_path.c_str());

    if (pipe(pipe_fd) == -1) {
        throw std::runtime_error("pipe() failed");
    }

    pid = fork();

    if (pid == -1) {
        throw std::runtime_error("fork() failed");
    } else if (!pid) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);

        char *argv[] = {cbin, pathBin, NULL};
        execve(cbin, argv, _envs);
        write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n\r\n", 38);
    } else {
        close(pipe_fd[1]);
        struct timespec currentTime;

        while (true) {
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                break;
            }
            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            if (currentTime.tv_sec - startTime.tv_sec > timeout / 1000) {
                kill(pid, SIGTERM);
                break;
            }
            usleep(500);
        }

        char buffer[8192] = {0};
        int  n            = 0;
        while ((n = read(pipe_fd[0], buffer, 8191)) > 0) {
            body.append(buffer, n);
        }
        close(pipe_fd[0]);
    }

    int i = 0;
    while (_envs[i]) {
        delete[] _envs[i];
        i++;
    }
    delete[] _envs;

    return (body);
}

std::string queryToString(std::vector<std::string> query)
{

    std::string resp;

    if (query.empty()) {
        return (resp);
    }

    for (std::vector<std::string>::iterator it = query.begin(); it != query.end(); it++) {
        resp += *it;
        if (it != query.end() - 1) {
            resp += "&";
        }
    }
    return (resp);
}

int getSIndex(Parser &parser, int serverSize, std::string port)
{
    std::vector<std::string> listenParam;
    int                      i = 0;

    for (i = 0; i != serverSize; ++i) {
        listenParam = parser.getServerParam(i, "listen");
        if (listenParam.empty()) {
            continue;
        }
        if (port == listenParam[0]) {
            break;
        }
    }
    return (i);
}
