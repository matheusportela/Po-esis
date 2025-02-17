// Definition of configuration parser, which can read values from a file. Each
// pair (key, value) is stored into Config in a map-like manner.
//
// The configuration file must use the following syntax:
// Key: String
// Value: String
// Comment: Line started by "#"
// Special keys: Some keys, starting by "$", have ability to improve an aspect
// of the configuration file, according to the table below:
//     $PATH: Sets a path that is prepended when getting values with CFG_GETP
//     $LOG_LEVEL: Sets the minimum log level to print messages.
// 
// Example:
// 
// example.cfg
// 
// # Directory containing all game resources
// $PATH = /home/portela/idj
// 
// # Background image for level 1
// BACKGROUND = /resources/img/ocean.jpg

#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <unordered_map>
#include <sstream>
#include <string>

#include "bandit/adapters/File.h"
#include "bandit/core/Log.h"
#include "bandit/core/parser/Parser.h"

#define DELIMITER "="

// Initializes configuration by reading from a file.
#define CFG_INIT(filename) \
    do \
    { \
        File file(filename); \
        ConfigParser::GetInstance().Parse(file); \
        file.Close(); \
    } while (false)

// Gets a configuration value as string.
#define CFG_GETS(key) \
    ConfigParser::GetInstance().Get(key)

// Gets a configuration value with prepended path.
#define CFG_GETP(key) \
    ConfigParser::GetInstance().GetWithPath(key)

// Gets a configuration value as integer.
#define CFG_GETI(key) \
    ConfigParser::GetInstance().GetAsInteger(key)

// Gets a configuration value as float.
#define CFG_GETF(key) \
    ConfigParser::GetInstance().GetAsFloat(key)

// Gets a configuration value as char.
#define CFG_GETC(key) \
    ConfigParser::GetInstance().GetAsChar(key)

// Gets a configuration value as boolean.
#define CFG_GETB(key) \
    ConfigParser::GetInstance().GetAsBool(key)

// Prints configuration keys and values.
#define CFG_PRINT() \
    ConfigParser::GetInstance().Print();

class ConfigParser : public Parser
{
  public:
    // Gets config instance as in the singleton pattern.
    static ConfigParser& GetInstance();

    // Reads configuration file.
    void Parse(File& file);

    // Gets single configuration value for a given key.
    std::string Get(std::string key);

    // Gets single configuration value for a given key, automatically prepending
    // the path.
    std::string GetWithPath(std::string key);

    // Gets single configuration value for a given key, automatically converting
    // to an integer value.
    int GetAsInteger(std::string key);

    // Gets single configuration value for a given key, automatically converting
    // to a float value.
    float GetAsFloat(std::string key);

    // Gets single configuration value for a given key, automatically converting
    // to a char value.
    char GetAsChar(std::string key);

    // Gets single configuration value for a given key, automatically converting
    // to a boolean value.
    bool GetAsBool(std::string key);

    // Prints configuration key, value pairs
    void Print();

  private:
    // Singleton pattern using the approach suggested at
    // http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    ConfigParser() {};
    ConfigParser(const ConfigParser&) = delete;
    void operator=(const ConfigParser&) = delete;

    // Sets path value.
    void SetPath(std::string path);

    // Checks whether the live is empty for human-readers, i.e., contains only
    // non-human readable characters.
    bool IsEmptyLine(std::string line);

    // Checks whether the given line is a comment line.
    bool IsCommentLine(std::string line);

    // Checks whether defined key modifies some configuration state.
    bool IsSpecialKey(std::string key);

    // Parses a single configuration line.
    void ParseLine(std::string line);

    // Sanitizes line by removing trailing white spaces.
    std::string SanitizeLine(std::string line);

    // Checks whether the configuration value exists for a given key.
    bool Contains(std::string key);

    // Gets single configuration value for a given key.
    std::string operator[](std::string key);

    // Map to hold configuration entries.
    std::unordered_map<std::string, std::string> configurationMap;

    // Path for file handling.
    std::string path;
};

#endif // CONFIG_PARSER_H_