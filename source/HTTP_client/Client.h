#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

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
			const char* path,
			const char* request_head,
			const char* request_body
		) {
			Log::line();
			Log::info("HTTP client is starting...");
			Log::new_line();

			Client::IP = IP;
			Client::PORT = PORT;

			Client::init_server_socket_address();
			Client::create_socket_TCP();
			Client::create_connect();
			Client::out();
			Client::in();
			Client::shutdown_socket_TCP();
			Client::close_socket_TCP();
		}

	private:
		inline static const char* IP = nullptr;
		inline static int PORT = 0;

		inline static int server_socket_address_result = -1;
		inline static int socket_TCP = -1;
		inline static int connect_result = -1;

		inline static struct sockaddr_in server_socket_address;

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

		static void out() {
			const int BUFFER_SIZE = 256;
			char buffer[BUFFER_SIZE];
			int bytes_to_send = snprintf(
				buffer,
				BUFFER_SIZE,
				"GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
				Client::IP
			);

			if (send(Client::socket_TCP, buffer, bytes_to_send, 0) < 0) Log::error("out(): Error");
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
