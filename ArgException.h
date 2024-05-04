#include <stdexcept>
#include <string>

/**
 * @brief This is a base class and shouldn't be instantiated.
 * 
 */
class ArgException : std::runtime_error
{
    public:
        explicit ArgException(const std::string& msg) : std::runtime_error(msg) { }
};

/**
 * @brief Throw when the program is called without arguments.
 * 
 */
class MissingArgsException final : ArgException
{
    public:
        MissingArgsException() : ArgException("No arguments were provided.") { }
};

/**
 * @brief Thrown when the program is called with more than 4 arguments.
 * 
 */
class InvalidArgCountException final : ArgException
{
    public:
        InvalidArgCountException() : ArgException("Too many arguments were given.") { }
};

/**
 * @brief Thrown when the program is called with invalid flags. 
 * 
 */
class InvalidArgsException final : ArgException
{
    public:
        InvalidArgsException() : ArgException("The arguments provided are invalid.") { }
};