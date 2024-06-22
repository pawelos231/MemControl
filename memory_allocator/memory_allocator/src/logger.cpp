#include "logger.h"

Logger::Logger() : current_logger_prefix("./logs") {
	struct stat info;

	// Check if the directory exists
	if (stat(this->get_current_logger_prefix(), &info) != 0) {
		// Directory does not exist, create it
		if (_mkdir(this->get_current_logger_prefix()) != 0) {
			throw std::runtime_error("Failed to create logs folder.");
		}
		else {
			std::cout << "Logs folder created successfully.\n";
		}
	}
	else if (info.st_mode & S_IFDIR) {
		// Directory exists, do nothing
	}
	else {
		throw std::runtime_error("Path exists but is not a directory.");
	}
}

Logger::~Logger() {
	if (out_file.is_open()) {
		out_file.close();
	}
}

std::shared_ptr<Logger> Logger::get_instance() {
	//std::make_shared, wont work here beacuse logger constructor is private
	//normally it is best to use make_shared beacuse it reduces the allocations of control block and object itself are combined
	static std::shared_ptr<Logger> logger(new Logger(), [](Logger* p) { delete p; });
	return logger;
}

void Logger::log(LOG_LEVEL level, const std::string& message) {
	if (level >= this->current_log_level) {
		//we create a guard to prevent race conditions and make it thread safe
		//guard unlocks automatically once it goes out of scope
		std::lock_guard<std::mutex> guard(this->log_mutex);
		std::string logMessage = this->level_to_string(level) + ": " + message;
		this->log_message(logMessage);
	}
}

void Logger::log_message(const std::string& message) {
	std::cout << message << std::endl;
	if (this->out_file.is_open()) {
		//if out_file is open write a message to it
		this->out_file << message << std::endl;
	}
}

std::string Logger::level_to_string(LOG_LEVEL level) const {
	switch (level) {
	case LOG_LEVEL::INFO:    return "INFO";
	case LOG_LEVEL::WARNING: return "WARNING";
	case LOG_LEVEL::ERR:     return "ERROR";
	default:				 return "UNKNOWN";
	}
}

void Logger::info(const std::string& message) {
	this->log(LOG_LEVEL::INFO, message);
}

void Logger::warning(const std::string& message) {
	this->log(LOG_LEVEL::WARNING, message);
}

void Logger::error(const std::string& message) {
	this->log(LOG_LEVEL::ERR, message);
}


void Logger::set_output_file(const std::string& filename) {
	//acquaire lock to avoid race conditions
	std::lock_guard<std::mutex> guard(this->log_mutex);
	//if file is open close it safely
	if (this->out_file.is_open()) {
		out_file.close();
	}
	//open file with provided filename
	std::string file_path = std::string(this->get_current_logger_prefix()) + '/' + filename;

	this->out_file.open(file_path, std::ios::out);
	if (!this->out_file) {
		throw std::runtime_error("Logger: Unable to open file " + filename);
	}
}

const char* Logger::get_current_logger_prefix() {
	return this->current_logger_prefix;
}

void Logger::set_current_logger_prefix(const char* prefix) {
	this->current_logger_prefix = prefix;
}