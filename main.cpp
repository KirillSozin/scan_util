#include "AhoCorasic.h"
#include <filesystem>
#include <fstream>
#include <list>
#include <ctime>

namespace fs = std::filesystem;

uint32_t COUNT_FILES = 0;
uint32_t COUNT_ERROR = 0;
uint32_t COUNT_JS = 0;
uint32_t COUNT_UNIX = 0;
uint32_t COUNT_macOS = 0;

std::list<std::string> dict_JS = { "<script>evil_script()</script>" };
std::list<std::string> dict_UNIX = { "rm -rf ~/Documents" };
std::list<std::string> dict_macOS = { "system(\"launchctl load /Library/LaunchAgents/com.malware.agent\")"};

void GetRequest(const int argc,
                const char **argv,
                std::string &path_string) { // read compilation parameters
  if (argc == 2) {
    path_string = argv[1];
  } else
    std::cerr << "ff: bad request\n" << "Count of arguments: " << argc << '\n';
}

void ScanDirectory(std::string &path_string,
                   AhoCorasic &checker_JS,
                   AhoCorasic &checker_UNIX,
                   AhoCorasic &checker_macOS) {
  fs::path dir_path(path_string);
  if (!fs::is_directory(dir_path)) {
    std::cerr << "error: " << path_string << " is not directory\n";
    return;
  }

  for (const auto &entry : fs::directory_iterator(dir_path)) {
    std::string current_file = static_cast<std::string>(entry.path());
    std::ifstream in(current_file);
    if (in.fail())
      ++COUNT_ERROR;
    else {
      std::string text_file;
      while (std::getline(in, text_file)) {
        if (current_file.substr(current_file.length() - 3, 3) == ".js") {
          COUNT_JS += checker_JS.run(text_file);
        }
        COUNT_UNIX += checker_UNIX.run(text_file);
        COUNT_macOS += checker_macOS.run(text_file);
      }
    }
    ++COUNT_FILES;
  }
}

template<class T>
void CheckerPreparation(AhoCorasic &checker, const T &dict) {
  for (const auto &word : dict)
    checker.add(word);
}

void PrintResScan(uint32_t time_scan) {
  std::cout << "====== Scan result ======\n";
  std::cout << "Processed files: " << COUNT_FILES << '\n';
  std::cout << "JS detects: " << COUNT_JS << '\n';
  std::cout << "UNIX detects: " << COUNT_UNIX << '\n';
  std::cout << "macOS detects: " << COUNT_macOS << '\n';
  std::cout << "Errors: " << COUNT_ERROR << '\n';
  std::cout << "Exection time: ";

  uint32_t h = time_scan / 3600, m = time_scan / 60 % 60, s = time_scan % 60;
  std::cout << std::setfill('0') << std::setw(2) << h << ':';
  std::cout << std::setfill('0') << std::setw(2) << m << ':';
  std::cout << std::setfill('0') << std::setw(2) << s << '\n';
}

void ScanUtil(const int argc, const char **argv) {
  uint32_t time_start = clock();

  std::string path_string;
  GetRequest(argc, argv, path_string);

  AhoCorasic checker_JS;
  AhoCorasic checker_UNIX;
  AhoCorasic checker_macOS;
  CheckerPreparation(checker_JS, dict_JS);
  CheckerPreparation(checker_UNIX, dict_UNIX);
  CheckerPreparation(checker_macOS, dict_macOS);

  ScanDirectory(path_string, checker_JS, checker_UNIX, checker_macOS);

  uint32_t time_end = clock();
  PrintResScan((time_end - time_start) / 1'000'000);
}

int main(const int argc, const char **argv) {
  ScanUtil(argc, argv);
  return 0;
}
