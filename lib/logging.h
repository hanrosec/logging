/**
 * @file logging.h
 * @author Hanrose
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * FORMATS:
 *  {hour} - hour
 *  {second} - second
 *  {minute} - minute
 *  {year} - year
 *  {month} - month
 *  {day} - day of month
 *  {level} - critical / error / warning / info / debug
 *  {logger_name} - logger name
 *  {message} - message
 */

#include <stdio.h>
#include <cstring>
#include <regex.h>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

int len(const char * data) {
    int counter=0;
    while (data[counter] != '\0'){
        counter++;
    }
    return counter;
}

const char* intToStr(int data) {
    std::string strData = std::to_string(data);

    char* temp = new char[strData.length() + 1];
    strcpy(temp, strData.c_str());

   return temp;
}

string replace_first(string str, int begin, int end, string to) {
    string out;
    begin -= 1;
    end += 1;

    for(int i=0; i<=begin; i++)
        out.push_back(str[i]);
    
    out.append(to);

    for(int i=end; i<=str.length(); i++)
        out.push_back(str[i]);
    
    return out;
}


class Logger {
    public:
        Logger(string name, string self);

        int critical(string message);
        int error(string message);
        int warning(string message);
        int debug(string message);
        int info(string message);

        void set_level(string level);
        void set_format(string format);
        void fill_with_spaces(bool fill);
        void file_handler(string filename, string format, string level);
        void exclude_level(string level);
        void set_logging_to_screen(bool log);
        
    private:
        string name;
        string format;
        int level;
        string process_format(string message, string level, string format);
        string filename;
        bool log_to_file = false;
        bool log_to_screen = true;
        string file_format;
        int file_level;
        string file;
        map<string, bool> excluded_levels{
            {"critical", false},
            {"error", false},
            {"warning", false},
            {"info", false},
            {"debug", false}
        };
};

Logger::Logger(string name="", string self=__FILE__) {
    /**
     * @brief constructor
     * 
     */
    this->name = name;
    this->file = self;
}


void Logger::file_handler(string filename, string format, string level) {
    /**
     * @brief log file handler
     * you need to specify filename, format of logs in file and level
     */
    this->log_to_file = true;
    this->filename = filename;    
    this->file_format = format;
    
    if(level == "error") {
        this->file_level = 0;
    }else if(level == "info") {
        this->file_level = 3;
    }else if (level == "debug") {
        this->file_level = 4;
    }

}

void Logger::exclude_level(string level) {
    /**
     * @brief exclude levels to log
     * 
     */
    this->excluded_levels[level] = true;
}

void Logger::set_level(string level) {
    /**
     * @brief set level of logging to screen
     * 
     */
    if(level == "error") {
        this->level = 0;
    }else if(level == "info") {
        this->level = 3;
    }else if (level == "debug") {
        this->level = 4;
    }
}

void Logger::set_format(string format) {
    /**
     * @brief set format of logging to screen
     * 
     */
    this->format = format;
}

void Logger::set_logging_to_screen(bool log) {
    /**
     * @brief set logging to screen (default true)
     * 
     */
    this->log_to_screen = log;
}

string Logger::process_format(string message, string level, string format) {
    /**
     * @brief Format parser
     * 
     */
    if(format == "")
        return "";

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    string out = format;
    out.resize(format.size());

    if(out.find("{hour}") != -1){
        if(len(intToStr(timeinfo->tm_hour)) == 1)
            out = replace_first(out, out.find("{hour}"), out.find("{hour}")+5, "0" + (string)intToStr(timeinfo->tm_hour));
        else
            out = replace_first(out, out.find("{hour}"), out.find("{hour}")+5, (string)intToStr(timeinfo->tm_hour));
    }
    if(out.find("{second}") != -1){
        if(len(intToStr(timeinfo->tm_sec)) == 1)
            out = replace_first(out, out.find("{second}"), out.find("{second}")+7, "0" + (string)intToStr(timeinfo->tm_sec));
        else
            out = replace_first(out, out.find("{second}"), out.find("{second}")+7, (string)intToStr(timeinfo->tm_sec));
    }
    if(out.find("{minute}") != -1)
        if(len(intToStr(timeinfo->tm_min)) == 1)
            out = replace_first(out, out.find("{minute}"), out.find("{minute}")+7, "0" + (string)intToStr(timeinfo->tm_min));
        else
            out = replace_first(out, out.find("{minute}"), out.find("{minute}")+7, (string)intToStr(timeinfo->tm_min));

    if(out.find("{month}") != -1)
        if(len(intToStr(timeinfo->tm_mon)) == 1)
            out = replace_first(out, out.find("{month}"), out.find("{month}")+6, "0" + (string)intToStr(timeinfo->tm_mon));
        else
            out = replace_first(out, out.find("{month}"), out.find("{month}")+6, (string)intToStr(timeinfo->tm_mon));

    if(out.find("{year}") != -1)
        out = replace_first(out, out.find("{year}"), out.find("{year}")+5, intToStr(1900 + timeinfo->tm_year));

    if(out.find("{day}") != -1)
        if(len(intToStr(timeinfo->tm_mday)) == 1)
            out = replace_first(out, out.find("{day}"), out.find("{day}")+4, "0" + (string)intToStr(timeinfo->tm_mday));
        else
            out = replace_first(out, out.find("{day}"), out.find("{day}")+4, "0" + (string)intToStr(timeinfo->tm_mday));

    if(out.find("{message}") != -1)
        out = replace_first(out, out.find("{message}"), out.find("{message}")+8, message);

    if(out.find("{level}") != -1)
        out = replace_first(out, out.find("{level}"), out.find("{level}")+6, level);
    
    if(out.find("{logger_name}") != -1) {
        if(this->name == "")
            out = replace_first(out, out.find("{logger_name}"), out.find("{logger_name}")+12, "");
        else
            out = replace_first(out, out.find("{logger_name}"), out.find("{logger_name}")+12, this->name);
    }
    if(out.find("{file}") != -1)
        out = replace_first(out, out.find("{file}"), out.find("{file}")+5, this->file);

    return out;
}

int Logger::critical(string message) {
    /**
     * @brief critical level
     * message logged to stderr
     */
    if(this->excluded_levels["critical"]){
        return 0;
    }

    if(this->log_to_screen) {
        if(this->level >= 0) {
            fprintf(stderr, "%s", this->process_format(message, "critical", this->format).c_str());
        }
    }
    if(this->log_to_file) {
        if(this->file_level >= 0) {
            ofstream log_file(this->filename, ios_base::app);
            log_file << this->process_format(message, "critical", this->file_format).c_str();
        }
    }
    return 0;
}

int Logger::error(string message) {
    /**
     * @brief error level
     * message logged to stderr
     * 
     */
    if(this->excluded_levels["error"]){
        return 10;
    }

    if(this->log_to_screen) {
        if(this->level >= 0) {
           fprintf(stderr, "%s", this->process_format(message, "error", this->format).c_str());
        }
    }

    if(this->log_to_file) {
        if(this->file_level >= 1) {
            ofstream log_file(this->filename, ios_base::app);
            log_file << this->process_format(message, "error", this->file_format).c_str();
        }
    }
    return 10;
}

int Logger::warning(string message) {
    /**
     * @brief warning level
     * message logged to stdout
     */
    if(this->excluded_levels["warning"]){
        return 20;
    }

    if(this->log_to_screen) {
        if(this->level >= 2) {
           fprintf(stdout, "%s", this->process_format(message, "warning", this->format).c_str());
        }   
    }

    if(this->log_to_file) {
        if(this->file_level >= 2) {
            ofstream log_file(this->filename, ios_base::app);
            log_file << this->process_format(message, "warning", this->file_format).c_str();
        }
    }

    return 20;
}

int Logger::info(string message) {
    /**
     * @brief info level
     * message logged to stdout
     * 
     */
    if(this->excluded_levels["info"]) {
        return 30;
    }

    if(this->log_to_screen) {
        if(this->level >= 3) {
            fprintf(stdout, "%s", this->process_format(message, "info", this->format).c_str());
        }
    }

    if(this->log_to_file) {
        if(this->file_level >= 3) {
            ofstream log_file(this->filename, ios_base::app);
            log_file << this->process_format(message, "info", this->file_format).c_str();
        }
    }
    return 30;
}

int Logger::debug(string message) {
    /**
     * @brief info level
     * message logged to stdout
     * 
     */
    if(this->excluded_levels["debug"]) {
        return 40;
    }

    if(this->log_to_screen) {
        if(this->level >= 4) {
           fprintf(stdout, "%s", this->process_format(message, "debug", this->format).c_str());
        }
    }

    if(this->log_to_file) {
        if(this->file_level >= 4) {
            ofstream log_file(this->filename, ios_base::app);
            log_file << this->process_format(message, "debug", this->file_format).c_str();
        }
    }
    return 40;
}
