#include "HTTP_client/Client.h"

int main() {
	const char* raw_request_data = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n {\"for\":\"woXrooX.com\"}";

	woXrooX::Client::request(
		"127.0.0.1",
		8000,
		"/",
		raw_request_data
	);

	return 0;
}
