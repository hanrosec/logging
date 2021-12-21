/**
 * FILE TO TEST LIBRARY
 */

// #include <iostream>
#include "lib/logging.h"


int main(int argc, char** argv) {
    Logger *logger = new Logger("logger 1", __FILE__);

    logger->set_level("debug");
    
    std::string format = "{file} == {hour}:{minute}:{second} [{level}]: \t{message}\n";
    logger->set_format(format);
    
    logger->file_handler("logs_file.log", "{hour}:{minute}:{second} [{level}]: \t{message}\n", "debug");
    

    // logger->exclude_level("debug");
    // logger->exclude_level("info");

    logger->error("asdf");
    
    logger->debug("debug test messgage");
    
    logger->debug("debug test messgage2");
    
    logger->info("info test messgage");
    
    logger->info("info test messgage2");
    
    logger->critical("Critical test message");
    
    logger->warning("Warning test message");
    
    logger->error("ążćśół!@#$%^&*(){}[]");
    
    logger->debug("ążćśół!@#$%^&*(){}[]");
    
}
