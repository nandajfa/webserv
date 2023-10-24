/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:49:52 by pmitsuko          #+#    #+#             */
/*   Updated: 2023/10/10 05:42:55 by pmitsuko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libs.hpp"

class Logger {
    public:
    enum LogLevel { INFO, WARNING, ERROR, VERBOSE };

    Logger(void);
    ~Logger(void);

    class LogEntry {
        public:
        LogEntry(LogLevel level);
        ~LogEntry(void);

        template <typename T>
        LogEntry &operator<<(T const &value)
        {
            _stream << value;
            return (*this);
        };

        LogEntry &operator<<(std::ostream &(*manipulator)(std::ostream &));

        private:
        LogLevel          _level;
        std::stringstream _stream;

        std::string _timestamp(void);
        std::string _addHeader(void);
    };

    static LogEntry info;
    static LogEntry warning;
    static LogEntry error;
    static LogEntry verbose;

    private:
    static const std::string _colors[4];
    static const std::string _levelStrings[4];
};
