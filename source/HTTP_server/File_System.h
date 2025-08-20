// std::cout << std::filesystem::exists(entry) << '\n';
// std::cout << std::filesystem::is_directory(entry) << '\n';

#ifndef HTTP_SERVER_FILES_H
#define HTTP_SERVER_FILES_H

#include <filesystem>
#include <fstream>
#include <string>
#include <map>

// 4 Debugging
#include <iostream>

namespace woXrooX {
	class File_System final {
	public:
		static void demo(){
			for (auto const& [key, val] : File_System::all) std::cout << key << "\n------------\n" << val << '\n';
		}

		static const std::map<std::string, std::string>& get_all(){ return File_System::all; }

		static void collec(){
			Log::line();
			Log::info("Collecting files...");
			Log::new_line();

			// Check if directory exists
			if (!std::filesystem::exists(Configurations::absolute_path_front)) {
				Log::warning("Directory does not exists: "+Configurations::absolute_path_front);
				File_System::has_error = true;
				return;
			}

			// Loop through all directories & files in a given path
			for (auto const& entry : std::filesystem::recursive_directory_iterator(Configurations::absolute_path_front)) {
				if(!std::filesystem::is_regular_file(entry)) continue;
				File_System::open((const std::string&)entry);
			}
		}

	private:
		inline static bool has_error = false;
		inline static std::map<std::string, std::string> all;

		static void open(const std::string& path) {
			std::ifstream file;
			std::string one_line, all_lines, relevant_path;

			file.open(path, std::ios::in);

			// Could Not Open
			if (!file.is_open()) {
				Log::warning("Could not open: "+path);
				File_System::has_error = true;
				return;
			}

			// Reading all lines
			while (getline(file, one_line)) all_lines += one_line+'\n';

			// Removing Full Path And Leaving Just Path For "Front" Folder Ex. "/js/main.js"
			relevant_path = path.substr(path.rfind("/front")+6, path.length()-path.rfind("/front"));
			File_System::all.insert({relevant_path, all_lines});

			// File Opened
			Log::success("Opened: "+path);

			file.close();
		}
	};
}

#endif
