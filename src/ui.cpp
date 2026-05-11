#include "ui.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void UI::printHeader() {
    clearScreen();
    printSeparator('=', 70);
    printColored(centerText("Wolf3DC IDE", 70), Color::CYAN);
    printColored(centerText("Version 1.0", 70), Color::CYAN);
    printSeparator('=', 70);
    std::cout << std::endl;
}

void UI::printFooter() {
    std::cout << std::endl;
    printSeparator('=', 70);
    std::cout << std::endl;
}

void UI::printSeparator(char c, int width) {
    for (int i = 0; i < width; i++) {
        std::cout << c;
    }
    std::cout << std::endl;
}

void UI::printDottedSeparator(int width) {
    for (int i = 0; i < width; i++) {
        if (i % 2 == 0) std::cout << ".";
        else std::cout << " ";
    }
    std::cout << std::endl;
}

void UI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool UI::supportsColor() {
#ifdef _WIN32
    return true;  // Windows 10+ supports colors
#else
    return isatty(STDOUT_FILENO);
#endif
}

std::string UI::getColorCode(Color color, bool background) {
    if (!supportsColor()) return "";
    
#ifdef _WIN32
    // Windows color codes are handled differently
    return "";
#else
    int code = 30 + static_cast<int>(color);
    if (background) code += 10;
    
    std::string ansiCode = "\033[";
    ansiCode += std::to_string(code);
    ansiCode += "m";
    return ansiCode;
#endif
}

std::string UI::resetColor() {
    if (!supportsColor()) return "";
#ifdef _WIN32
    return "";
#else
    return "\033[0m";
#endif
}

void UI::printColored(const std::string& text, Color color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));
    std::cout << text << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    if (supportsColor()) {
        std::cout << getColorCode(color) << text << resetColor() << std::endl;
    } else {
        std::cout << text << std::endl;
    }
#endif
}

void UI::printSuccess(const std::string& text) {
    std::cout << "[";
    printColored(" ✓ ", Color::GREEN);
    std::cout << "] " << text << std::endl;
}

void UI::printError(const std::string& text) {
    std::cout << "[";
    printColored(" ✗ ", Color::RED);
    std::cout << "] " << text << std::endl;
}

void UI::printWarning(const std::string& text) {
    std::cout << "[";
    printColored(" ⚠ ", Color::YELLOW);
    std::cout << "] " << text << std::endl;
}

void UI::printInfo(const std::string& text) {
    std::cout << "[";
    printColored(" ℹ ", Color::BLUE);
    std::cout << "] " << text << std::endl;
}

void UI::printDebug(const std::string& text) {
    std::cout << "[";
    printColored(" ◆ ", Color::MAGENTA);
    std::cout << "] " << text << std::endl;
}

void UI::printStep(int step, const std::string& description) {
    std::cout << "\n";
    printColored("=" + std::string(68, '='), Color::CYAN);
    std::ostringstream oss;
    oss << "Step " << step << ": " << description;
    std::cout << oss.str() << std::endl;
    printColored("=" + std::string(68, '='), Color::CYAN);
}

void UI::printSubStep(const std::string& description) {
    std::cout << "  ├─ " << description << std::endl;
}

void UI::printStats(const std::string& label, const std::string& value) {
    std::cout << std::left << std::setw(35) << label 
              << " : " << std::right << std::setw(20) << value << std::endl;
}

void UI::printStatsHeader() {
    printSeparator('-', 60);
    std::cout << std::left << std::setw(35) << "Parameter" 
              << " : " << std::right << std::setw(20) << "Value" << std::endl;
    printSeparator('-', 60);
}

void UI::printStatsFooter() {
    printSeparator('-', 60);
}

void UI::printProgress(int current, int total, const std::string& task) {
    int percent = (current * 100) / total;
    int bars = percent / 5;
    
    std::cout << "\r" << task << " [";
    for (int i = 0; i < 20; i++) {
        if (i < bars) std::cout << "█";
        else std::cout << "░";
    }
    std::cout << "] " << std::setw(3) << percent << "%" << std::flush;
}

void UI::printTableHeader(const std::vector<std::string>& columns) {
    printSeparator('-', 70);
    for (size_t i = 0; i < columns.size(); i++) {
        std::cout << std::left << std::setw(20) << columns[i];
        if (i < columns.size() - 1) std::cout << " | ";
    }
    std::cout << std::endl;
    printSeparator('-', 70);
}

void UI::printTableRow(const std::vector<std::string>& values) {
    for (size_t i = 0; i < values.size(); i++) {
        std::cout << std::left << std::setw(20) << values[i];
        if (i < values.size() - 1) std::cout << " | ";
    }
    std::cout << std::endl;
}

void UI::printTableSeparator(const std::vector<int>& widths) {
    for (size_t i = 0; i < widths.size(); i++) {
        for (int j = 0; j < widths[i] + 2; j++) std::cout << "-";
        if (i < widths.size() - 1) std::cout << "+";
    }
    std::cout << std::endl;
}

std::string UI::centerText(const std::string& text, int width) {
    int padding = (width - text.length()) / 2;
    if (padding < 0) padding = 0;
    return std::string(padding, ' ') + text;
}
