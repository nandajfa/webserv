/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcl <mcl@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 03:18:36 by mcl               #+#    #+#             */
/*   Updated: 2023/10/11 19:03:37 by mcl              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"
#include "Logger.hpp"

typedef std::map<std::string, std::vector<std::string> > params;

typedef struct {
    params  *server;
    params **locations;
} conf_servers;

class Parser
{
    private:
        conf_servers*       _cservers;
        int                 _servers;
        std::vector<int>    _locs;
        bool                _verbose;
    public:
        Parser();
        Parser(const char* fileconf);
        ~Parser();

        void init(const char* fileconf, bool verbose = false);
        void setConfs(const char* fileconf);
        void allocateServers(conf_servers* server, int locs);
        void deallocateServers(conf_servers* server, int locs);
        void clearParams();
        void populateConfs(std::vector<std::vector<std::string> > servers, std::vector<std::vector<std::string> > locations);
        std::vector<std::string> getServerParam (int server, std::string param);
        std::vector<std::string> getLocationParam (int server, int location, std::string param);
        std::vector<int> getSizeServers ();
        int getServers( void ) const;
};

std::vector<std::string> splitTokens(const std::string str);
std::string removeExtraSpaces(const std::string& input);
params* setParams(const std::string str, params* vconfs);
bool verifyBlockEnd(const std::string& text);
bool verifyLineEmpty(const std::string& text);