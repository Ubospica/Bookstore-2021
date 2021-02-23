//
// Created by Ubospica on 2021/2/8.
//

#include "MainClass.hpp"

#include <string>
#include <unordered_map>
#include <iostream>

#include "Exception.hpp"
#include "UserStatus.hpp"
#include "Bookstore.hpp"

namespace Bookstore {

    //awful; maybe I'll design a better & tricky implement later
    class Tool {
    public:
        using option_t_basic =
                std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>;
        using option_t = const option_t_basic&;
        static option_t getOptions();
        static std::vector<std::string> split(const std::string&);
//        static void destruct();
    };

    Tool::option_t Tool::getOptions() {
        static option_t options {
                {"su",UserStatus::su},
                {"logout",UserStatus::logout},
                {"useradd",UserStatus::useradd},
                {"register",UserStatus::register_},
                {"delete",UserStatus::delete_},
                {"passwd",UserStatus::passwd},
                {"select",Bookstore::select},
                {"modify",Bookstore::modify},
                {"import",Bookstore::import},
                {"show",Bookstore::show},
                {"buy",Bookstore::buy},
//                {"report",Parser::report},
//                {"log",Parser::log},
        };
        return options;
    }

    std::vector<std::string> Tool::split(const std::string &s) {
        std::stringstream ss(s);
        std::vector<std::string> res;
        while (ss && ss.peek() != EOF) {
            int ch0 = ss.peek();
            if (ch0 == '-') {
                std::string tmp;
                tmp += static_cast<char>(ss.get());
                int ch=ss.peek();
                while (ch != ' ' && ch != '=' && ch != EOF) {
                    tmp += static_cast<char>(ss.get());
                    ch=ss.peek();
                }
                if (tmp.length() == 1) {
                    throw SyntaxError();
                }
                res.push_back(tmp);
            }
            else if (ch0 == '=') {
                if (res.size() < 2 || res.back()[0] != '-') {
                    throw SyntaxError();
                }
                std::string tmp;
                tmp += static_cast<char>(ss.get());
                int ch=ss.peek();
                if (ch == '\"') {
                	ss.ignore();
                	ch = ss.peek();
	                while (ch != '\"' && ch != EOF) {
		                tmp += static_cast<char>(ss.get());
		                ch=ss.peek();
	                }
	                if (ch != '\"') {
		                throw SyntaxError();
	                }
	                ss.ignore();
                }
                else {
	                while (ch != ' ' && ch != EOF) {
		                tmp += static_cast<char>(ss.get());
		                ch = ss.peek();
	                }
                }
                res.push_back(tmp);
            }
            else if (ch0 == ' ') {
                do {
                    ss.ignore();
                } while (ss.peek() == ' ');// && ss.peek() != EOF
            }
            else {
                std::string tmp;
                getline(ss,tmp,' ');
                res.push_back(tmp);
            }
        }
        for (const auto &i : res) {
        	if (i[0] == '-' || i[0] == '=') {
        		if (i.size() == 1) {
        			throw SyntaxError();
        		}
        	}
        	else {
        		if (i.empty()) {
        			throw SyntaxError();
        		}
        	}
        }
//        for (const auto &i : res) {
//            std::cerr << i << '\n';
//        }
        return res;
    }
    
//    void Tool::destruct() {
//        UserStatus::getInstance().~UserStatus();
//    }

    void MainClass::init() {
//    	int cnt = 0;
        std::string inputLine;
        auto options = Tool::getOptions();
        while (getline(std::cin, inputLine)) {
//        	++cnt;
//        	if (cnt == 3658) {
//        		std::cerr << "aa";
//        	}
            try {
                const auto &option = Tool::split(inputLine);
	            if (option.empty()) {
	            	//throw SyntaxError("No option");
	            	continue;
	            }
	            if (option[0] == "quit" || option[0] == "exit") {
		            break;
	            }
	            else if (option[0] == "show") {
	            	if (option.size() > 1 && option[1] == "finance") {
	            		Bookstore::showFinance(option);
	            	}
	            	else {
			            Bookstore::show(option);
	            	}
	            }
	            else {
		            auto it = options.find(option.front());
		            if (it == options.end()) {
			            throw NoOptionError();
		            } else {
			            it->second(option);
		            }
	            }
//	            std::cerr << '\n' << "operation " <<inputLine << '\n';
//				std::cerr << "operator " << cnt << '\n';
//				if (cnt >= 3600 && cnt <= 3658)
//	            Bookstore::getInstance().kwdIndex.print();
            } catch (NoOptionError& e) {
            	std::cout << "Invalid\n";
                std::cerr << "Error: Option not found " << e.what() << '\n';
            } catch (SyntaxError& e) {
	            std::cout << "Invalid\n";
                std::cerr << "Error: Input Syntax error " << e.what() << '\n';
            } catch (PermissionError& e) {
	            std::cout << "Invalid\n";
                std::cerr << "Error: Insufficient privilege " << e.what() << '\n';
            } catch (RuntimeError& e) {
	            std::cerr << "Fatal: Runtime Error " << e.what() << '\n';
            } catch (RunningError& e) {
	            std::cout << "Invalid\n";
	            std::cerr << "Error: Problem occurs during executing " << e.what() << '\n';
            }
        }
//      Tool::destruct();
    }
}
