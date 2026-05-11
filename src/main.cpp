#include "ide_interface.h"
#include "logger.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Инициализация логера
        Logger& logger = Logger::getInstance();
        if (!logger.initialize("wolf3dc.log")) {
            std::cerr << "Warning: Failed to initialize logger" << std::endl;
        }
        logger.info("Wolf3DC IDE started");
        
        IDEInterface ide;
        
        if (!ide.initialize()) {
            logger.error("Failed to initialize IDE. Check compiler installation.");
            std::cerr << "Fatal: Failed to initialize IDE. Check compiler installation." << std::endl;
            return 1;
        }
        
        logger.info("IDE initialized successfully");
        ide.run();
        ide.shutdown();
        
        logger.info("Wolf3DC IDE closed");
        logger.close();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}
