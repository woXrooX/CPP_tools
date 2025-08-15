#ifndef WEB_SERVER_CONFIGURATIONS_H
#define WEB_SERVER_CONFIGURATIONS_H

#include <string>

namespace woXrooX{
	struct Configurations final{
		struct Server final{
			// "127.0.0.1" -> localhost
			static inline const std::string IP = "127.0.0.1";
			static inline const std::string PORT = "8080";

			static inline const int BUFFER = 131072;
			static inline const int BACKLOG = 1024;
		};

		struct HTTP final{
			struct Content_Type final{
				static inline const std::string text_plain = "text/plain; charset=utf-8";
				static inline const std::string text_HTML = "text/html; charset=utf-8";
				static inline const std::string text_JavaScript = "text/javascript; charset=utf-8";
				static inline const std::string text_CSS = "text/css; charset=utf-8";

				static inline const std::string image_SVG = "image/svg+xml";
				static inline const std::string image_x_icon = "image/x-icon";
				static inline const std::string image_PNG = "image/png";
				static inline const std::string image_JPEG = "image/jpeg";
				static inline const std::string image_GIF = "image/gif";

				static inline const std::string font_WOFF = "ont/woff";
				static inline const std::string font_WOFF2 = "font/woff2";
				static inline const std::string font_OTF = "font/otf";
				static inline const std::string font_TTF = "font/ttf";
			};

			struct Response_Status final{
				static inline const std::string ok = "200 OK";
				static inline const std::string not_found = "404 Not Found";
			};
		};

		// If two or more "front" named folders exist in Configurations::absolute_path_front then it will leack system files!!!
		static inline const std::string absolute_path_front = "/home/woxroox/Desktop/woXTools/source/tools/server/front/";
		static inline const std::string absolute_path_back = "";
		static inline const std::string path_index_HTML = "/html/index.html";
	};
}

#endif
