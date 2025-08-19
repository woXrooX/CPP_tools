#ifndef HTTP_SERVER_PAGE_H
#define HTTP_SERVER_PAGE_H

#include <map>

namespace woXrooX {
	class Page final : public HTML{
	public:
		//// Get all pages
		static const std::map<std::string, Page*>& get_all(){return Page::all;}

		//// Development demo all pages infos
		static void demo(){
			for (auto page : woXrooX::Page::get_all()) {
				std::cout
				<< "\nPage: " << page.first << "\n\n"
				<< page.second->get_source() << "\n<---------------------------------------------->\n";
			}
		}

		///////// Construtors
		// Default & Expected Initiator
		Page(const std::string &id, const std::string &title = "Undefined", bool get = true, bool post = false) : id{id}, methods{get, post}{
			HTML::set_title(title);
			HTML::construct();

			Page::all[this->id] = this;
		}

		// Remove page from Page::all list
		~Page() { Page::all.erase(this->id); }

		// Coppy construtor. This construtor will be called when object is coppied. Example: Page home; Page me = home;
		Page(const Page &page) { std::cout << "Coppied" << '\n'; }


		// void main() override;
		const std::string main() override {return "Page->Main";}

		// Get page source code
		const std::string& get_source() const {return HTML::HTML;}

	private:
		//// All pages
		static inline std::map<std::string, Page*> all;

		// Identificator. Example: home | me..
		std::string id;

		// Methods
		struct Methods { bool GET = true, POST = false; };
		Methods methods;
	};
}

#endif
