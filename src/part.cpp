//
// part.cpp
// Nathanael Epps
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <Config.hpp>

//#define DEBUG_MODE

typedef std::vector<std::string> args_t;

static const char* cmd = nullptr;

inline bool IsNum(char ch) {
  return ch >= '0' && ch <= '9';
}

void help()
{
  std::cout << cmd << " v" << PROJECT_VERSION << "\n";
  std::cout << "Usage: " << cmd << " [options] <start> <end> <file>\n";
  std::cout << "Options:\n";
  std::cout << "   --version, -v, --info      Print some info\n";
  std::cout << "   --help, -h                 Print this help information\n";
}

struct input_t {
  int start;
  int end;
  std::string fname;
  bool valid;

  input_t() { valid = true; }
};

#ifdef DEBUG_MODE

std::ostream& operator<<(std::ostream& stream, const input_t& input)
{
  return stream << std::boolalpha << "{" << input.start << ", " << input.end << ", \"" << input.fname << "\", " << input.valid << "}";
}

#endif

input_t extract(args_t args)
{
  input_t data;

  if (args.size() != 4) {
    help();
    data.valid = false;
    return data;
  }

  // skip first arg (name of executable)
  for (int x = 1; x < args.size(); x++) {
    if (x <= 2) {
      std::string arg = args.at(x);
      for (int y = 0; y < arg.size(); y++) {
        if (!IsNum(arg[y])) {
          std::cerr << cmd << ": Error parsing number from string \"" << arg << "\"\n";
          data.valid = false;
          return data;
        }
      }
      std::stringstream ss (arg);
      if (x == 1)
        ss >> data.start;
      else if (x == 2)
        ss >> data.end;
    }
    else {
      data.fname = args.at(x);
    }
  }

  if (data.start > data.end) {
    std::cerr << cmd << ": Error: start > end\n";
    data.valid = false;
  }

#ifdef DEBUG_MODE
  std::cout << cmd << ": Extracted input: " << data << "\n";
#endif
  return data;
}

int run(args_t args)
{
  input_t input = extract(args);
  if (!input.valid)
    return EXIT_FAILURE;

  std::ifstream filestream(input.fname);
  if (!filestream) {
    std::cerr << cmd << ": Error: unable to open file \"" << input.fname << "\"\n";
    return EXIT_FAILURE;
  }

  int lineno = 1;
  std::string line;
  while (std::getline(filestream, line)) {
    if (lineno >= input.start && lineno <= input.end)
      std::cout << line << "\n";
    if (lineno == input.end)
      break;
    lineno++;
  }

  return EXIT_SUCCESS;
}

int info()
{
  std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << "\n";
  std::cout << "Written by Nathanael Epps, (c) 2021\n";
  return EXIT_SUCCESS;
}

int dispatch(args_t args)
{
  if (std::find(args.begin(), args.end(), "--version") != args.end() ||
      std::find(args.begin(), args.end(), "-v") != args.end() ||
      std::find(args.begin(), args.end(), "--info") != args.end()) {
    return info();
  }
  else if (std::find(args.begin(), args.end(), "--help") != args.end() ||
           std::find(args.begin(), args.end(), "-h") != args.end()) {
    help();
    return EXIT_SUCCESS;
  }
  else {
    return run(args);
  }
}

int main(int argc, const char** argv)
{
  cmd = argv[0];
  try {
    return dispatch(args_t(argv, argv + argc));
  }
  catch (std::exception& ex) {
    std::cerr << cmd << ": " << ex.what() << "\n";
  }
  catch (...) {
    std::cerr << cmd << ": *** unknown error ***\n";
  }

  return EXIT_FAILURE;
}
