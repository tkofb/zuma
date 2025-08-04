#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

string dotenv(string &key) {
  fs::path dotenvPath = fs::current_path() / ".env";

  if (!fs::exists(dotenvPath)) {
    // Fallback: look in parent directory (useful if running from /build)
    dotenvPath = fs::current_path().parent_path() / ".env";
  }

  std::ifstream dataFile;

  dataFile.open(dotenvPath, std::ios::in);
  if (dataFile.is_open()) {
    std::string line;

    while (std::getline(dataFile, line)) {
      std::string delimiter = "=";
      std::string keyStr = line.substr(0, line.find(delimiter));
      std::string valStr = line.substr(line.find(delimiter) + 1, line.size());

      if (keyStr == key) {
        return valStr;
      }
    }

    dataFile.close();
    throw std::runtime_error("Key '" + key + "' not found in .env file.");
  } else {
    throw std::runtime_error("Error opening file.");
  }
}