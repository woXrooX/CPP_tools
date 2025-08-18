// HTTPs://code.tutsplus.com/tutorials/HTTP-headers-for-dummies--net-8039

#ifndef WEB_SERVER_HTTP_H
#define WEB_SERVER_HTTP_H

// 4 debugging
#include <iostream>

namespace woXrooX{
	class HTTP final{
	public:
		static void handle(const std::string &in_data) {
			// Parse HTTP Request
			HTTP::parse(in_data);

			// Handle GET
			if(HTTP::request_method == "GET") HTTP::handle_GET();

			// Update response
			HTTP::update_response();
		}

		//////// Getters
		static const std::string& get_request_firs_line() { return HTTP::request_first_line; }
		static const std::string& get_response() { return HTTP::response; }
		static const std::string& get_response_first_line() { return HTTP::response_first_line; }

	private:
		static inline void parse(const std::string &in_data){
			//// First line
			HTTP::request_first_line = in_data.substr(0, in_data.find("\r\n"));

			// Validate first line. If first line is invalid rest doesn't matter
			int request_first_line_space_count = 0;

			for (auto &c : HTTP::request_first_line) if(c == ' ') request_first_line_space_count++;

			if (request_first_line_space_count != 2) {
				Log::error("Invalid HTTP request: Invalid first line");
				HTTP::has_error = true;
				return;
			}

			HTTP::request_header = in_data.substr(0, in_data.find("\r\n\r\n"));

			HTTP::request_method = HTTP::request_first_line.substr(0, HTTP::request_first_line.find(' '));

			HTTP::request_protocol = HTTP::request_first_line.substr(
				HTTP::request_first_line.rfind(' ') + 1, HTTP::request_first_line.length() - HTTP::request_first_line.rfind(' ')
			);

			HTTP::request_file = HTTP::request_first_line.substr(
				HTTP::request_first_line.find(' ')+1, HTTP::request_first_line.length() - HTTP::request_method.length() - HTTP::request_protocol.length() - 2
			);
		}

		static inline void handle_GET(){
			//// Check if request is file or just path
			if (
				HTTP::request_file.rfind('.') != std::string::npos &&
				HTTP::request_file.rfind('/') < HTTP::request_file.rfind('.')
			) {
				//// Get the file
				// 404
				if (File_System::get_all().find(HTTP::request_file) == File_System::get_all().end()) {
					HTTP::response_status = Configurations::HTTP::Response_Status::not_found;
					HTTP::response_content_type.clear();
					HTTP::response_body.clear();
				}

				// 200
				else {
					//// If file exists then define response Content-Type

					// File extension
					std::string file_extension = HTTP::request_file.substr(HTTP::request_file.rfind('.') + 1, HTTP::request_file.length() - HTTP::request_file.rfind('.'));

					// Text
					if (file_extension == "html") HTTP::response_content_type = Configurations::HTTP::Content_Type::text_HTML;
					else if (file_extension == "css") HTTP::response_content_type = Configurations::HTTP::Content_Type::text_CSS;
					else if (file_extension == "js") HTTP::response_content_type = Configurations::HTTP::Content_Type::text_JavaScript;
					else if (file_extension == "txt") HTTP::response_content_type = Configurations::HTTP::Content_Type::text_plain;

					// Image
					else if (file_extension == "svg") HTTP::response_content_type = Configurations::HTTP::Content_Type::image_SVG;
					else if (file_extension == "ico") HTTP::response_content_type = Configurations::HTTP::Content_Type::image_x_icon;
					else if (file_extension == "png") HTTP::response_content_type = Configurations::HTTP::Content_Type::image_PNG;
					else if (file_extension == "jpeg" || file_extension == "jpg") HTTP::response_content_type = Configurations::HTTP::Content_Type::image_JPEG;

					// Font
					else if (file_extension == "woff") HTTP::response_content_type = Configurations::HTTP::Content_Type::font_WOFF;
					else if (file_extension == "woff2") HTTP::response_content_type = Configurations::HTTP::Content_Type::font_WOFF2;
					else if (file_extension == "otf") HTTP::response_content_type = Configurations::HTTP::Content_Type::font_OTF;
					else if (file_extension == "ttf") HTTP::response_content_type = Configurations::HTTP::Content_Type::font_TTF;

					// Unknown
					else HTTP::response_content_type = "unknown/unknown";

					HTTP::response_status = Configurations::HTTP::Response_status::OK;
					HTTP::response_body = File_System::get_all().find(HTTP::request_file)->second;
				}
			}

			// Serve for just path request
			else {
				HTTP::response_status = Configurations::HTTP::Response_status::OK;
				HTTP::response_content_type = Configurations::HTTP::Content_Type::text_HTML;
				HTTP::response_body = File_System::get_all().find(Configurations::path_index_HTML)->second;
			}
		}

		static inline void update_response(){
			HTTP::response_first_line = HTTP::response_protocol + " " + HTTP::response_status;
			HTTP::response_header =
				HTTP::response_first_line + "\r\n" +
				// HTTP::response_cache_control + "\r\n" +
				"Content-Type: " + HTTP::response_content_type + "\r\n" +
				// "Content-Length: " + HTTP::response_body::size() +
				// HTTP::response_connection +
				HTTP::response_server +
				HTTP::response_header_end;

			HTTP::response = HTTP::response_header + HTTP::response_body;
		}

		static bool has_error;

		// Request
		static std::string request_header;
		static std::string request_first_line;
		static std::string request_method;
		static std::string request_file;
		static std::string request_protocol;

		// Response
		static std::string response;
		static std::string response_header;
		static std::string response_body;
		static std::string response_first_line;
		static std::string response_status;
		static std::string response_protocol;
		static std::string response_cache_control;
		static std::string response_content_type;
		static std::string response_connection;
		static const std::string response_server;
		static const std::string response_header_end;

	};

	bool HTTP::has_error = false;

	////// In data
	std::string HTTP::request_header;
	std::string HTTP::request_first_line;
	std::string HTTP::request_method;
	std::string HTTP::request_file;
	std::string HTTP::request_protocol;


	////// Out data
	std::string HTTP::response;
	std::string HTTP::response_header;
	std::string HTTP::response_body;
	std::string HTTP::response_first_line;
	std::string HTTP::response_protocol = "HTTP/2";
	std::string HTTP::response_status;
	std::string HTTP::response_cache_control = "max-age=600";
	std::string HTTP::response_content_type;
	std::string HTTP::response_connection = "Connection: Close\r\n";
	const std::string HTTP::response_server = "Server: woXrooX Web Server (WWS)\r\n";
	const std::string HTTP::response_header_end = "\r\n";
}

#endif
