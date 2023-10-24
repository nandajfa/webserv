/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:49:57 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/13 16:22:37 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

const std::string Logger::_colors[4]       = {"\033[92m", "\033[93m", "\033[95m", "\033[37m"};
const std::string Logger::_levelStrings[4] = {"INFO", "WARNING", "ERROR", "VERBOSE"};

Logger::LogEntry Logger::info(INFO);
Logger::LogEntry Logger::warning(WARNING);
Logger::LogEntry Logger::error(ERROR);
Logger::LogEntry Logger::verbose(VERBOSE);

Logger::Logger(void) {}

Logger::~Logger(void) {}

std::string Logger::LogEntry::_timestamp(void)
{
    std::time_t currentTime;
    std::tm    *localTime;
    char        timeStr[24];

    currentTime = std::time(0);
    localTime   = std::localtime(&currentTime);
    std::strftime(timeStr, sizeof(timeStr), "[%d/%m/%Y - %H:%M:%S]", localTime);
    return (std::string(timeStr));
}

std::string Logger::LogEntry::_addHeader(void)
{
    std::string       header;
    std::stringstream ss;

    ss << _colors[_level] << _timestamp() << "  ";
    ss << std::left << std::setw(8) << _levelStrings[_level] << ": ";
    header = ss.str();
    return (header);
}

Logger::LogEntry::LogEntry(LogLevel level) : _level(level) {}

Logger::LogEntry::~LogEntry(void) {}

Logger::LogEntry &Logger::LogEntry::operator<<(std::ostream &(*manipulator)(std::ostream &))
{
    if (_level == ERROR)
        std::cerr << _addHeader() << _stream.str() << RESET_COLOR << manipulator;
    else
        std::cout << _addHeader() << _stream.str() << RESET_COLOR << manipulator;
    _stream.str("");
    _stream.clear();
    return (*this);
}
