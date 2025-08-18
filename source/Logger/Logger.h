#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

// 4 logs_absolute_path Existance Check
#include <filesystem>

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

// for time
#include <iomanip>

enum class OS {
	Unknown = 0,

	// Unix, Linux and MacOS
	Posix = 1,
	Windows = 2
};

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	inline constexpr OS LOGGER_OS = OS::Posix;
#elif defined(_WIN32)
	inline constexpr OS LOGGER_OS = OS::Windows;
#else
	inline constexpr OS LOGGER_OS = OS::Unknown;
#endif


// Types
struct Types{
	static inline const std::string SUCCESS = "SUCCESS";
	static inline const std::string INFO = "INFO";
	static inline const std::string WARNING = "WARNING";
	static inline const std::string IMPORTANT = "IMPORTANT";
	static inline const std::string ERROR = "ERROR";
};

namespace woXrooX {
	class Logger final {
	public:
		template<typename T>
		static void success(T message) { Logger::log(Types::SUCCESS, message); }

		template<typename T>
		static void info(T message) { Logger::log(Types::INFO, message); }

		template<typename T>
		static void warning(T message) { Logger::log(Types::WARNING, message); }

		template<typename T>
		static void important(T message) { Logger::log(Types::IMPORTANT, message); }

		template<typename T>
		static void error(T message) { Logger::log(Types::ERROR, message); }

		template<typename T1, typename T2>
		static void custom(T1 type, T2 message) { Logger::log(type, message); }

		static void line() {
			// Comment 4 daemon. Daemon has no terminal to "stdout"
			// std::cout << Logger::color_line << "----------------------------------------------------------------" << Logger::color_end << '\n';

			// If Enabled Line For "log_to_file"
			if (Logger::log_to_file_enabled) Logger::log_to_file("\n----------------------------------------------------------------------------------------\n");
		}

		static void new_line() {
			// Comment 4 daemon. Daemon has no terminal to "stdout"
			// std::cout << '\n';

			// If enabled new line for "log_to_file"
			if (Logger::log_to_file_enabled) Logger::log_to_file("\n");
		}


		///////// Log_to_file: enable & disable

		static void enable_log_to_file() {
			// Check If "logs_absolute_path" Exists
			if (!std::filesystem::exists(Logger::logs_absolute_path)) {
				Logger::log(Types::WARNING, "Invalid logs_absolute_path: " + Logger::logs_absolute_path);
				return;
			}

			// Opening log file
			// Second argument for append mode
			Logger::file.open(Logger::logs_absolute_path + "all.log", std::ios_base::app);

			// Check if file opened successfully
			if (Logger::file.is_open()) {
				// After successful openening, enable log to file
				Logger::log_to_file_enabled = true;
				Logger::log(Types::INFO, "log_to_file_enabled: Enabled");
				Logger::log(Types::SUCCESS, "Log file opened successfully: all.log");
				Logger::log(Types::INFO, "Log file absolute path: " + Logger::logs_absolute_path);
			}

			else {
				Logger::log(Types::WARNING, "log_to_file_enabled: Not enabled");
				Logger::log(Types::WARNING, "Could not open log file");
				return;
			}
		}

		static void disable_log_to_file() {
			Logger::log_to_file_enabled = false;
			Logger::log(Types::INFO, "log_to_file_enabled: Disabled");
			Logger::close_log_file();
		}

		///////// enable & disable SquareBrackets

		static void enable_square_brackets() {
			Logger::square_brackets_open = "[";
			Logger::square_brackets_close = "] ";
			Logger::log(Types::INFO, "square_brackets: Enabled");
		}

		static void disable_square_brackets() {
			Logger::square_brackets_open = "";
			Logger::square_brackets_close = " ";
			Logger::log(Types::INFO, "square_brackets: Disabled");
		}

	private:
		template<typename T1, typename T2>
		static void log(T1 type, T2 message) {
			// setting color depending on the type
			Logger::set_color(type);

			// Getting timestamp and setting to a variable so it will be same timestamp on "log_to_file()"
			const std::string timestamp = Logger::get_timestamp();

			// Redirects all stdout to a file. #include <stdio.h>
			// freopen((Logger::logs_absolute_path+"all.log").c_str(), "a", stdout);

			// Out | Log
			// Comment 4 daemon. Daemon has no terminal to "stdout"
			// std::cout << Logger::square_brackets_open << timestamp << Logger::square_brackets_close << Logger::square_brackets_open << Logger::color_start << type << Logger::color_end << Logger::square_brackets_close << message << '\n';

			// Cosntructing "log_to_file" [timestamp] [TYPE] Message
			if (Logger::log_to_file_enabled) Logger::log_to_file(('[' + timestamp + "] [" + type + "] " + message + '\n'));
		}

		static void log_to_file(const std::string& log) {
			if (!Logger::log_to_file_enabled) return;

			// Flush is bad for performance!!!
			// https://softwareengineering.stackexchange.com/questions/386269/why-do-you-want-to-avoid-flushing-stdout
			// if (!(Logger::file << log << std::flush)) std::cout << "ERROR: Could not write the log" << '\n';
			// W/O flushing logs will be written when OS thinks buffer is big enough to flush instead flushing small data.
			if (!(Logger::file << log)) {
				// Comment 4 daemon. Daemon has no terminal to "stdout"
				// std::cout << "ERROR: Could not write the log: " << log << '\n';
			}
		}

		static void close_log_file() {
			Logger::file.close();

			// Check if file closed successfully
			if (!Logger::file.is_open()) Logger::log(Types::SUCCESS, "Log file closed successfully");
			else Logger::log(Types::ERROR, "Can not close the log file");
		}

		static const std::string get_timestamp() {
			// Be aware of ugly code
			std::stringstream ss;
			std::time_t const dt = time(NULL);
			ss << std::put_time(localtime(&dt), "%T %F");
			std::string datetime = ss.str();

			return datetime;
		}

		// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
		static void set_color(const std::string& type) {
			// Unix | Linux
			if (LOGGER_OS == OS::Posix) {
				Logger::color_start = "\033[1;90m";
				if (Types::SUCCESS.compare(type) == 0) Logger::color_start = "\033[1;32m";
				else if (Types::INFO.compare(type) == 0) Logger::color_start = "\033[1;34m";
				else if (Types::WARNING.compare(type) == 0) Logger::color_start = "\033[1;33m";
				else if (Types::IMPORTANT.compare(type) == 0) Logger::color_start = "\033[1;35m";
				else if (Types::ERROR.compare(type) == 0) Logger::color_start = "\033[1;31m";
			}
		}



		// File
		static std::ofstream file;
		static bool log_to_file_enabled;
		static bool log_only_to_file_enabled;
		static const std::string logs_absolute_path;

		// Square Brackets
		static std::string square_brackets_open;
		static std::string square_brackets_close;

		// Colors
		static std::string color_start;
		static std::string color_end;
		static std::string color_line;

	};
	std::ofstream Logger::file;
	bool Logger::log_to_file_enabled = false;
	bool Logger::log_only_to_file_enabled = false;
	const std::string Logger::logs_absolute_path = "/Users/woxroox/Desktop/CPP_tools/logs";

	std::string Logger::square_brackets_open = "";
	std::string Logger::square_brackets_close = " ";

	std::string Logger::color_start = "";
	std::string Logger::color_end = "\033[0m";
	std::string Logger::color_line = "\033[1;90m";

	// Alias Log To OG Logger
	typedef Logger Log;
}

#endif
