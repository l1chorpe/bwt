#include <stdexcept>
#include <string>


/**
 * @brief Thrown whenever an error occured while handling a file.
 * 
 * This is a base class and shouldn't be instantiated.
 * 
 */
class FileException : std::runtime_error
{
    public:
        explicit FileException(const std::string& msg) : std::runtime_error(msg) { }
};

/**
 * @brief Thrown when a file is empty.
 * 
 */
class EmptyFileException final : FileException
{
    public:
        EmptyFileException() : FileException("The file provided is empty.") { }
};

/**
 * @brief Thrown when a file can't be read.
 * 
 */
class FileReadException final : FileException
{
    public:
        FileReadException() : FileException("The file couldn't be opened.") { }
};

class FileWriteException final : FileException
{
    public:
        FileWriteException() : FileException("The file couldn't be written to.") { }
};