#include <algorithm> // For std::reverse
#include <iostream>
#include <sstream> // For std::stringstream
#include <string>
#include <vector>

std::string
reverseBlocksOrderInToml(const std::string& toml)
{
    if (toml.empty()) {
        return toml;
    }

    std::vector<std::string> sections;
    std::string topLevelVars;
    std::stringstream ss(toml);
    std::string line;
    std::string currentBlock;
    bool inSection = false;

    // Parse the TOML content into blocks
    while (std::getline(ss, line)) {
        // Check if this line starts a new section
        if (!line.empty() && line[0] == '[') {
            // If we were building a previous block, save it
            if (!currentBlock.empty()) {
                // Remove trailing newlines from the current block
                while (!currentBlock.empty() && currentBlock.back() == '\n') {
                    currentBlock.pop_back();
                }
                if (inSection) {
                    sections.push_back(currentBlock);
                } else {
                    topLevelVars = currentBlock;
                }
                currentBlock.clear();
            }
            inSection = true;
        }
        
        // Add the line to the current block
        if (!currentBlock.empty()) {
            currentBlock += "\n";
        }
        currentBlock += line;
    }

    // Don't forget the last block
    if (!currentBlock.empty()) {
        // Remove trailing newlines from the last block
        while (!currentBlock.empty() && currentBlock.back() == '\n') {
            currentBlock.pop_back();
        }
        if (inSection) {
            sections.push_back(currentBlock);
        } else {
            topLevelVars = currentBlock;
        }
    }

    // Reverse the order of sections only
    std::reverse(sections.begin(), sections.end());

    // Also reverse the lines within each section (except the section header)
    for (auto& section : sections) {
        std::stringstream ss(section);
        std::string line;
        std::vector<std::string> lines;
        
        // Read all lines from the section
        while (std::getline(ss, line)) {
            lines.push_back(line);
        }
        
        if (lines.size() > 1) {
            // Keep the first line (section header) in place, reverse the rest
            std::string sectionHeader = lines[0];
            std::vector<std::string> contentLines(lines.begin() + 1, lines.end());
            std::reverse(contentLines.begin(), contentLines.end());
            
            // Reconstruct the section
            section = sectionHeader;
            for (const auto& contentLine : contentLines) {
                section += "\n" + contentLine;
            }
        }
    }

    // Reconstruct the TOML string: top-level vars first, then reversed sections
    std::string result;
    
    // Add top-level variables first
    if (!topLevelVars.empty()) {
        result += topLevelVars;
        if (!sections.empty()) {
            result += "\n\n";
        }
    }
    
    // Add sections in reversed order
    for (size_t i = 0; i < sections.size(); ++i) {
        result += sections[i];
        if (i < sections.size() - 1) {
            result += "\n\n";  // Add double newline between sections
        }
    }

    return result;
}