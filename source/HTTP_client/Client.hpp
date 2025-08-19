#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


namespace woXrooX{
	class Client final{
	public:
		static void request(
			const char* IP,
			const int PORT,
			const char* path = "/",
			const char* raw_request_data = ""
		) {
			Log::line();
			Log::info("HTTP client is starting...");
			Log::new_line();

			Client::IP = IP;
			Client::PORT = PORT;

			Client::init_server_socket_address();
			Client::create_socket_TCP();
			Client::create_connect();
			Client::out(raw_request_data);
			Client::in();
			Client::shutdown_socket_TCP();
			Client::close_socket_TCP();
		}

	private:
		inline static const char* IP = nullptr;
		inline static int PORT = 0;

		inline static struct sockaddr_in server_socket_address;

		inline static int server_socket_address_result = -1;
		inline static int socket_TCP = -1;
		inline static int connect_result = -1;

		static void init_server_socket_address() {
			// Zeroing addr before using
			memset(&Client::server_socket_address, 0, sizeof(Client::server_socket_address));

			Client::server_socket_address.sin_family = AF_INET;
			Client::server_socket_address.sin_port = htons(Client::PORT);

			Client::server_socket_address_result = inet_pton(AF_INET, Client::IP, &Client::server_socket_address.sin_addr);

			if (Client::server_socket_address_result == 0) Log::error("init_server_socket_address(): Invalid address string");
			if (Client::server_socket_address_result == -1) Log::error("init_server_socket_address(): Error (e.g., unsupported af); errno is set (commonly EAFNOSUPPORT)");
			else if (Client::server_socket_address_result == 1) Log::success("init_server_socket_address(): Success");
		}

		static void create_socket_TCP() {
			if (Client::server_socket_address_result != 1) return;

			Client::socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
			if (Client::socket_TCP == -1) Log::error("create_socket_TCP(): Failed to create socket descriptor");
			else Log::success("create_socket_TCP(): Socket descriptor created");
		}

		static void create_connect() {
			if (Client::socket_TCP == -1) return;

			Client::connect_result = connect(Client::socket_TCP, (struct sockaddr*)&server_socket_address, sizeof(server_socket_address));

			if (Client::connect_result != 0) Log::error("create_connect(): Error");
			else Log::success("create_connect(): Success");
		}

		static void out(const char* raw_request_data){
			if (Client::socket_TCP == -1) return;
			if (!raw_request_data) return Log::error("out(): null raw_request_data");

			// NOTE: The moment you need to send a body that may contain '\0' (compressed data, protobufs, etc.), this breaks.
			size_t length = strlen(raw_request_data);
			if (length == 0) return Log::error("out(): Empty request");

			if (!Client::send_all_bytes(raw_request_data, length)) return;

			Log::success("out(): request sent");
		}

		static bool send_all_bytes(const char* data, size_t length) {
			while (length) {
				ssize_t bytes_sent = ::send(Client::socket_TCP, data, length, 0);

				if (bytes_sent < 0) {
					Log::error("send_all(): send failed");
					return false;
				}

				data += (size_t)bytes_sent;
				length -= (size_t)bytes_sent;
			}
			return true;
		}

		static void in() {
			const int BUFFER_SIZE = 4096;
			char buffer[BUFFER_SIZE];
			ssize_t in_data;
			while ((in_data = recv(Client::socket_TCP, buffer, BUFFER_SIZE, 0)) > 0) fwrite(buffer, 1, in_data, stdout);

			if (in_data < 0) Log::error("in(): Error");
		}

		static void shutdown_socket_TCP(){
			if (Client::socket_TCP == -1) return;

			if (shutdown(Client::socket_TCP, SHUT_RDWR) != 0) Log::error("Failed to shutdown socket TCP");
			else Log::success("Socket TCP shutdown successfully");
		}

		static void close_socket_TCP(){
			if(Client::socket_TCP == -1) return;

			if (close(Client::socket_TCP) == -1) return Log::error("Failed to close socket TCP");

			Client::socket_TCP = -1;
			Log::success("Socket TCP closed successfully");
		}
	};
}

#endif
