#include "logger.h"

Logger::Logger() {

}


std::shared_ptr<Logger> Logger::get_instance() {
	//std::make_shared is more efficient then using straight up the 'new' keyword
	//when used seperetaly, so something like that: static std::shared_ptr<Logger> logger{ new Logger };
	//the allocation of the control block (which contains reference count and other metadata) and the object itself
	//are allocated seperately std::make_shared combines those allocations, it boosts perfomance and can increase cache locality
	static auto logger = std::make_shared<Logger>();
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
	case LOG_LEVEL::ERROR:   return "ERROR";
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
	this->log(LOG_LEVEL::ERROR, message);
}


void Logger::set_output_file(const std::string& filename) {
	//acquaire lock to avoid race conditions
	std::lock_guard<std::mutex> guard(this->log_mutex);
	//if file is open close it safely
	if (this->out_file.is_open()) {
		out_file.close();
	}
	//open file with provided filename
	this->out_file.open(filename, std::ios::out);
	if (!this->out_file) {
		throw std::runtime_error("Logger: Unable to open file " + filename);
	}
}