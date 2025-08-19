#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// memset
#include <string.h>
#include <sys/socket.h>

// for -> pton mton;arga
#include <arpa/inet.h>

// for -> close()
#include <unistd.h>

// for -> getaddrinfo
#include <netdb.h>

// Order Is Important!
#include "Configurations.h"
#include "File_System.h"
#include "HTTP.h"

//////////////// Mess
////// Server computer's infos
struct addrinfo hints, *server_computer_infos;

////// Client computer's Iinfos
struct sockaddr_storage client_address;
socklen_t client_address_size = sizeof client_address;

namespace woXrooX{
	class Server final {
	public:
		static void start() {
			File_System::collec();
			// File_System::demo();

			Log::line();
			Log::info("Web server is starting...");
			Log::new_line();
			Server::init_server_computer_infos();
			Server::create_socket_TCP();
			Server::make_address_reusalbe();
			Server::keep_alive();
			Server::create_bind();
			Server::create_listen();
			Server::on = true;
			Server::create_new_socket_TCP();
		}

		static void stop() {
			// Check If Server Is Running
			if(Server::on == false) return;

			Server::on = false;
			Server::close_new_socket_TCP();
			Server::shutdown_socket_TCP();
			Server::close_socket_TCP();
		}

	private:
		static bool on;

		static std::string out_data;

		// Not used for now
		// static std::string in_data;

		static int bytes_sent;
		static int bytes_received;

		// Socket SetUp
		static int get_addr_info;
		static int socket_TCP;
		static int binding;
		static int listening;
		static int new_socket_TCP;

		static void init_server_computer_infos() {
			// Zeroing hints before using
			memset(&hints, 0, sizeof hints);

			// Don't care IPv4 or IPv6
			hints.ai_family = AF_UNSPEC;

			// TCP stream sockets
			hints.ai_socktype = SOCK_STREAM;

			// Fill in my computer's IP for me
			hints.ai_flags = AI_PASSIVE;
			Server::get_addr_info = getaddrinfo(Configurations::Server::IP.c_str(), Configurations::Server::PORT.c_str(), &hints, &server_computer_infos);

			if(Server::get_addr_info != 0) Log::error("Error In Establishing Server Computer Informations");
			else Log::success("Server Computer Informations Established Successfully");
		}

		static void create_socket_TCP(){
			// Check if server computer informations established successfully
			if(Server::get_addr_info != 0) return;

			Server::socket_TCP = socket(server_computer_infos->ai_family, server_computer_infos->ai_socktype, server_computer_infos->ai_protocol);
			if(Server::socket_TCP == -1) Log::error("Failed to create socket descriptor.");
			else Log::success("Socket descriptor created successfully");
		}

		static void make_address_reusalbe(){
			// Check if socket TCP created successfully
			if (Server::socket_TCP == -1) return;

			// NOTE: For development!
			// Below code allows rebind to address while the address is in use.
			// W/O this function it fails to bind to the address after terminating the server.
			// OS keeps socket open for a while so binding is imposible!
			// Disable this function on production.
			// Enable/Disable at woXrooX::Server::start::make_address_reusalbe();
			const int REUSE_MODE = 1;
			if (setsockopt(Server::socket_TCP, SOL_SOCKET, SO_REUSEADDR, &REUSE_MODE, sizeof(REUSE_MODE)) == -1) Log::error("Failed to enable address reuse mode");
			else{
				Log::custom("DEV MODE", "Address reuseable mode is enabled successfully.");
				Log::warning("On Production Disable Address Reuseable Mode At 'woXrooX::Server::start()::make_address_reusalbe();'");
			}
		}

		static void keep_alive(){
			// Check if socket TCP created successfully
			if (Server::socket_TCP == -1) return;

			const int KEEP_ALIVE = 1;
			if (setsockopt(Server::socket_TCP, SOL_SOCKET, SO_KEEPALIVE, &KEEP_ALIVE, sizeof(KEEP_ALIVE)) == -1) Log::error("Failed to keep alive");
			else Log::success("Keep alive enabled successfully");
		}

		static void create_bind(){
			// Check if socket TCP created successfully
			if (Server::socket_TCP == -1) return;

			Server::binding = bind(Server::socket_TCP, server_computer_infos->ai_addr, server_computer_infos->ai_addrlen);
			if (Server::binding == -1) Log::error("Failed to bind.");
			else Log::success("Bound successfully");
		}

		static void create_listen(){
			// Check if bound successfully
			if (Server::binding == -1) return;

			Server::listening = listen(Server::socket_TCP, Configurations::Server::BACKLOG);
			if (Server::listening == -1) Log::error("Failed to listen");
			else {
				Log::success("Listening successfully on: http://"+Configurations::Server::IP+":"+Configurations::Server::PORT);
				Log::line();
			}
		}

		static void create_new_socket_TCP(){
			// Check if listening successfully
			if (Server::listening == -1) return;

			while ((Server::new_socket_TCP = accept(Server::socket_TCP, (struct sockaddr *)&client_address, &client_address_size)) >= 0) {
				Log::line();
				Log::success("New connection accepted successfully");

				// Log client IP address

				// 46 is max IPv6 size
				char client_address_printable[46];
				inet_ntop(AF_INET, &client_address, client_address_printable, sizeof client_address_printable);
				Log::custom("Client IP address", client_address_printable);

				// Receive data
				Server::in();

				// Send data
				Server::out();

				Server::close_new_socket_TCP();
			}

			if (Server::new_socket_TCP == -1) Log::error("Failed to accept");
		}

		static void in(){
			// bytes_received = -1 on error
			// bytes_received = 0 on end of the line
			// bytes_received > 0 on data

			// Check if new_socket_TCP established successfully
			if (Server::new_socket_TCP == -1) return;

			char in_data[Configurations::Server::BUFFER];
			int received_data_size = 0;
			do {
				Server::bytes_received = recv(Server::new_socket_TCP, in_data, Configurations::Server::BUFFER, 0);
				received_data_size += Server::bytes_received;

				if (received_data_size > Configurations::Server::BUFFER-1 || in_data[Configurations::Server::BUFFER-1] == '\n') break;

				if (Server::bytes_received == -1) {
					Log::error("Failed to receive data");
					return;
				}

				if (Server::bytes_received == 0) {
					Log::info("End of the line");
					break;
				}

				if (Server::bytes_received > 0) {
					Log::success("Data received successfully");
					break;
				}

			} while(true);

			HTTP::handle(in_data);
			Log::custom("<", HTTP::get_request_firs_line());
		}

		static void out(){
			// Check if new_socket_TCP established successfully
			if (Server::new_socket_TCP == -1) return;

			Server::out_data = HTTP::get_response();
			Server::bytes_sent = send(Server::new_socket_TCP, Server::out_data.c_str(), Server::out_data.size(), 0);
			if (Server::bytes_sent == -1) {
				Log::error("Failed to send data");
				return;
			}
			// else Log::success("Data sent successfully");

			Log::custom(">", HTTP::get_response_first_line());
		}

		static void close_new_socket_TCP(){
			// Check if new_socket_new_socket_TCPtcp established successfully
			if (Server::new_socket_TCP == -1) return;

			if (close(Server::new_socket_TCP) == -1) Log::error("Failed to close new socket TCP");
			else Log::success("New socket TCP closed successfully");
		}

		static void shutdown_socket_TCP(){
			// Check if socket_TCP established successfully
			if (Server::socket_TCP == -1) return;

			if (shutdown(Server::socket_TCP, SHUT_RDWR) != 0) Log::error("Failed to shutdown socket TCP");
			else Log::success("Socket TCP shutdown successfully");
		}

		static void close_socket_TCP(){
			// Check if socket_TCP established successfully
			if(Server::socket_TCP == -1) return;

			if(close(Server::socket_TCP) == -1) Log::error("Failed to close socket TCP");
			else Log::success("Socket TCP closed successfully");
		}
	};

	bool Server::on = false;

	std::string Server::out_data;

	int Server::bytes_sent = 0;
	int Server::bytes_received = 0;

	int Server::get_addr_info = 0;
	int Server::socket_TCP = -1;
	int Server::binding = -1;
	int Server::listening = -1;
	int Server::new_socket_TCP = -1;
}

#endif
