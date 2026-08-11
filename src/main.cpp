#include <iostream>
#include <filesystem>
#include <Magick++.h>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;
using namespace Magick;

struct ConversionOptions {
    string inputFile;
    string outputFile;
    int quality = 85;
    bool recursive = false;
};

void printUsage(const char* programName) {
    cout << "Usage: " << programName << " [options]\n"
         << "Options:\n"
         << "  -i, --input <file>     Input HEIC file\n"
         << "  -o, --output <file>    Output file (default: same name with .jpg)\n"
         << "  -q, --quality <1-100>  JPEG quality (default: 85)\n"
         << "  -r, --recursive        Convert all HEIC files in directory\n"
         << "  -h, --help             Show this help message\n";
}

bool convertHeicToJpg(const string& inputPath, const string& outputPath, int quality) {
    try {
        Image image;
        image.read(inputPath);
        image.quality(quality);
        image.write(outputPath);
        return true;
    } catch (const Exception& e) {
        cerr << "Error converting " << inputPath << ": " << e.what() << endl;
        return false;
    }
}

vector<string> findHeicFiles(const string& directory) {
    vector<string> heicFiles;
    
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            string extension = entry.path().extension().string();
            // Convert to lowercase for comparison
            transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (extension == ".heic" || extension == ".heif") {
                heicFiles.push_back(entry.path().string());
            }
        }
    }
    
    return heicFiles;
}

int main(int argc, char* argv[]) {
    try {
        InitializeMagick(*argv);
        
        ConversionOptions options;
        bool showHelp = false;
        
        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];
            
            if (arg == "-h" || arg == "--help") {
                showHelp = true;
            } else if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
                options.inputFile = argv[++i];
            } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
                options.outputFile = argv[++i];
            } else if ((arg == "-q" || arg == "--quality") && i + 1 < argc) {
                options.quality = stoi(argv[++i]);
                if (options.quality < 1 || options.quality > 100) {
                    cerr << "Quality must be between 1 and 100" << endl;
                    return 1;
                }
            } else if (arg == "-r" || arg == "--recursive") {
                options.recursive = true;
            }
        }
        
        if (showHelp || options.inputFile.empty()) {
            printUsage(argv[0]);
            return 0;
        }
        
        // Single file conversion
        if (!options.recursive) {
            if (!fs::exists(options.inputFile)) {
                cerr << "Input file not found: " << options.inputFile << endl;
                return 1;
            }
            
            // Generate output filename if not provided
            if (options.outputFile.empty()) {
                options.outputFile = options.inputFile;
                size_t dotPos = options.outputFile.rfind('.');
                if (dotPos != string::npos) {
                    options.outputFile = options.outputFile.substr(0, dotPos) + ".jpg";
                } else {
                    options.outputFile += ".jpg";
                }
            }
            
            cout << "Converting: " << options.inputFile << " -> " << options.outputFile << endl;
            if (convertHeicToJpg(options.inputFile, options.outputFile, options.quality)) {
                cout << "Conversion successful!" << endl;
                return 0;
            } else {
                return 1;
            }
        } else {
            // Batch conversion
            if (!fs::is_directory(options.inputFile)) {
                cerr << "Input path is not a directory: " << options.inputFile << endl;
                return 1;
            }
            
            vector<string> heicFiles = findHeicFiles(options.inputFile);
            if (heicFiles.empty()) {
                cout << "No HEIC files found in directory" << endl;
                return 0;
            }
            
            int successCount = 0;
            int failCount = 0;
            
            cout << "Found " << heicFiles.size() << " HEIC file(s)" << endl;
            
            for (const auto& inputFile : heicFiles) {
                string outputFile = inputFile;
                size_t dotPos = outputFile.rfind('.');
                if (dotPos != string::npos) {
                    outputFile = outputFile.substr(0, dotPos) + ".jpg";
                }
                
                cout << "Converting: " << inputFile << " -> " << outputFile << endl;
                if (convertHeicToJpg(inputFile, outputFile, options.quality)) {
                    successCount++;
                } else {
                    failCount++;
                }
            }
            
            cout << "\nConversion complete: " << successCount << " successful, "
                 << failCount << " failed" << endl;
            
            return failCount > 0 ? 1 : 0;
        }
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
}