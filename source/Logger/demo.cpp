#include "Logger/Logger.h"

int main() {
	// woXrooX::Logger::enable_log_to_file();
	// woXrooX::Logger::disable_log_to_file();

	// woXrooX::Logger::enable_square_brackets();
	// woXrooX::Logger::disable_square_brackets();

	woXrooX::Log::custom("CUSTOM", "Custom Message");
	woXrooX::Log::success("SUCCESS");
	woXrooX::Log::info("INFO");
	woXrooX::Log::warning("WARNING");
	woXrooX::Log::important("IMPORTANT");
	woXrooX::Log::error("ERROR");

	woXrooX::Log::line();
	woXrooX::Log::new_line();

	return 0;
}
