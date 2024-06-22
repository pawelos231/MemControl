#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <sys/stat.h>
#include <direct.h>


//A singleton class to log events to the file and console
class Logger {
public:
	enum class LOG_LEVEL {
		INFO,
		WARNING,
		ERR
	};

	// Get the singleton instance of Logger
	static std::shared_ptr<Logger> get_instance();

	// Delete copy constructor and assignment operator to prevent copies
	Logger(Logger const&) = delete;
	Logger& operator=(Logger const&) = delete;

	// Log a message with a specific logging level
	void log(LOG_LEVEL level, const std::string& message);

	// Log an info message
	void info(const std::string& message);

	// Log a warning message
	void warning(const std::string& message);

	// Log an error message
	void error(const std::string& message);

	// Set the output file for the logger
	void set_output_file(const std::string& filename);

	//sets the current logger prefix
	void set_current_logger_prefix(const char* prefix);

	//gets the current logger prefix, for example ./logs, which setups folder to store it in to be called logs
	const char* get_current_logger_prefix();


private:
	//private Logger constructor to not be able to create it later in the app
	Logger();
	~Logger();

	std::ofstream out_file; //output file where logs will be stored
	LOG_LEVEL current_log_level; //current log level, info, warning or error
	std::mutex log_mutex; // mutex to lock ability to read and write to the file
	const char* current_logger_prefix;

	// Helper method to get string representation of log level
	std::string level_to_string(LOG_LEVEL level) const;

	//helper method to log to the standard output and the file
	void log_message(const std::string& message);

};