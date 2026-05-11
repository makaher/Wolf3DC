#include "ide_interface.h"
#include "logger.h"
#include "ui.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

IDEInterface::IDEInterface() : running(false) {}

IDEInterface::~IDEInterface() {}

bool IDEInterface::initialize() {
    Logger::getInstance().info("Initializing IDE...");
    return compiler.initialize();
}

void IDEInterface::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void IDEInterface::printHeader() {
    clearScreen();
    std::cout << "╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║     Wolf3DC IDE - Wolfenstein 3D Development Environment      ║" << std::endl;
    std::cout << "║                          v1.0                                 ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void IDEInterface::printFooter() {
    std::cout << std::endl << "───────────────────────────────────────────────────────────────" << std::endl;
}

void IDEInterface::printMenu() {
    printHeader();
    std::cout << "Main Menu:" << std::endl << std::endl;
    std::cout << "  [N] New Project" << std::endl;
    std::cout << "  [O] Open Project" << std::endl;
    std::cout << "  [C] Compile" << std::endl;
    std::cout << "  [R] Run" << std::endl;
    std::cout << "  [S] Settings" << std::endl;
    std::cout << "  [F] File Manager" << std::endl;
    std::cout << "  [H] Help" << std::endl;
    std::cout << "  [Q] Quit" << std::endl;
    printFooter();
}

void IDEInterface::printProjectMenu() {
    std::cout << std::endl << "Project: " << projectManager.getProjectName() << std::endl;
    std::cout << "Path: " << projectManager.getProjectPath() << std::endl;
    auto files = projectManager.getSourceFiles();
    std::cout << "Files: " << files.size() << std::endl;
    for (const auto& file : files) {
        std::cout << "  - " << file << std::endl;
    }
}

void IDEInterface::handleNewProject() {
    clearScreen();
    std::cout << "═ Create New Project ═" << std::endl << std::endl;
    
    std::string projName, projPath;
    
    std::cout << "Project name: ";
    std::getline(std::cin, projName);
    
    std::cout << "Project path [./projects]: ";
    std::getline(std::cin, projPath);
    if (projPath.empty()) projPath = "./projects";
    
    if (projectManager.createNewProject(projName, projPath)) {
        std::cout << "[+] Project created successfully!" << std::endl;
        projectManager.saveProject();
        Logger::getInstance().info("Project created: " + projName);
    } else {
        std::cout << "[-] Failed to create project." << std::endl;
        Logger::getInstance().error("Failed to create project: " + projName);
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::handleOpenProject() {
    clearScreen();
    std::cout << "═ Open Project ═" << std::endl << std::endl;
    
    std::string prjPath;
    std::cout << "Project file path (.PRJ): ";
    std::getline(std::cin, prjPath);
    
    if (projectManager.loadProject(prjPath)) {
        std::cout << "[+] Project loaded: " << projectManager.getProjectName() << std::endl;
        Logger::getInstance().info("Project opened: " + prjPath);
    } else {
        std::cout << "[-] Failed to open project." << std::endl;
        Logger::getInstance().error("Failed to open project: " + prjPath);
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::handleAddFile() {
    clearScreen();
    std::cout << "═ Add File to Project ═" << std::endl << std::endl;
    
    std::string filePath;
    std::cout << "File path (.C or .ASM): ";
    std::getline(std::cin, filePath);
    
    if (projectManager.addSourceFile(filePath)) {
        std::cout << "[+] File added successfully!" << std::endl;
        Logger::getInstance().info("File added: " + filePath);
    } else {
        std::cout << "[-] Invalid file type. Use .C or .ASM" << std::endl;
        Logger::getInstance().warning("Invalid file type: " + filePath);
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::handleCompile() {
    clearScreen();
    std::cout << "═ Compile Project ═" << std::endl << std::endl;
    
    auto sourceFiles = projectManager.getSourceFiles();
    if (sourceFiles.empty()) {
        std::cout << "[-] No source files in project." << std::endl;
        Logger::getInstance().warning("Compile attempted with no source files");
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        return;
    }
    
    CompileOptions opts;
    opts.outputFile = projectManager.getOutputFile();
    opts.backend = CompilerBackend::AUTO_DETECT;
    opts.memoryModel = MemoryModel::PROTECTED_MODE;
    opts.optimizationLevel = projectManager.getSettings().optimizationLevel;
    opts.generateDebugInfo = projectManager.getSettings().debugInfo;
    
    std::cout << "Compiling " << sourceFiles.size() << " file(s)..." << std::endl << std::endl;
    Logger::getInstance().info("Starting compilation of " + std::to_string(sourceFiles.size()) + " files");
    
    auto result = compiler.compile(sourceFiles, opts);
    
    if (result.success) {
        std::cout << std::endl << "[+] Compilation successful!" << std::endl;
        std::cout << "Output: " << result.executablePath << std::endl;
        Logger::getInstance().info("Compilation successful: " + result.executablePath);
    } else {
        std::cout << std::endl << "[-] Compilation failed!" << std::endl;
        if (!result.errorLog.empty()) {
            std::cout << "Error: " << result.errorLog << std::endl;
            Logger::getInstance().error("Compilation failed: " + result.errorLog);
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::handleRun() {
    clearScreen();
    std::cout << "═ Run Program ═" << std::endl << std::endl;
    
    std::string exePath = projectManager.getOutputFile();
    std::cout << "Running: " << exePath << std::endl << std::endl;
    Logger::getInstance().info("Running program: " + exePath);
    
    int result = system(exePath.c_str());
    
    std::cout << std::endl << "Program exited with code: " << result << std::endl;
    Logger::getInstance().info("Program exited with code: " + std::to_string(result));
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::handleProjectSettings() {
    clearScreen();
    std::cout << "═ Project Settings ═" << std::endl << std::endl;
    
    auto& settings = projectManager.getSettings();
    
    std::cout << "Current Settings:" << std::endl;
    std::cout << "  Platform: " << settings.targetPlatform << std::endl;
    std::cout << "  Optimization: " << settings.optimizationLevel << std::endl;
    std::cout << "  Memory Model: " << settings.memoryModel << std::endl;
    std::cout << "  Debug Info: " << (settings.debugInfo ? "Yes" : "No") << std::endl;
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void IDEInterface::run() {
    running = true;
    
    while (running) {
        printMenu();
        
        std::cout << "Command: ";
        char cmd;
        std::cin >> cmd;
        std::cin.ignore();
        
        switch (std::tolower(cmd)) {
            case 'n':
                handleNewProject();
                break;
            case 'o':
                handleOpenProject();
                break;
            case 'c':
                handleCompile();
                break;
            case 'r':
                handleRun();
                break;
            case 's':
                handleProjectSettings();
                break;
            case 'f':
                handleAddFile();
                break;
            case 'h':
                clearScreen();
                std::cout << "Help - Wolf3DC IDE" << std::endl << std::endl;
                std::cout << "Commands:" << std::endl;
                std::cout << "  N - Create a new W3D project" << std::endl;
                std::cout << "  O - Open an existing .PRJ file" << std::endl;
                std::cout << "  C - Compile the project with DJGPP" << std::endl;
                std::cout << "  R - Run the compiled executable" << std::endl;
                std::cout << "  S - View/modify project settings" << std::endl;
                std::cout << "  F - Manage project files" << std::endl;
                std::cout << "  Q - Quit the IDE" << std::endl;
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                break;
            case 'q':
                running = false;
                break;
            default:
                std::cout << "Unknown command. Press Enter...";
                std::cin.ignore();
        }
    }
}

void IDEInterface::shutdown() {
    clearScreen();
    std::cout << "Exiting Wolf3DC IDE..." << std::endl;
}
