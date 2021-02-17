//
// Created by Ubospica on 2021/2/8.
//

#include "MainClass.hpp"

#include <string>
#include <unordered_map>
#include <iostream>

#include "Exception.hpp"
#include "UserStatus.hpp"

namespace Bookstore {

    //awful; maybe I'll design a better & tricky implement later
    class Tool {
    public:
        using option_t_basic =
                std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>;
        using option_t = const option_t_basic&;
        static option_t getOptions();
        static std::vector<std::string> split(const std::string&);
        static void destruct();
    };

    Tool::option_t Tool::getOptions() {
        static option_t options {
                {"su",UserStatus::su},
                {"logout",UserStatus::logout},
                {"useradd",UserStatus::useradd},
                {"register",UserStatus::register_},
                {"delete",UserStatus::delete_},
                {"passwd",UserStatus::passwd},
//                {"select",Parser::select},
//                {"modify",Parser::modify},
//                {"import",Parser::import},
//                {"show",Parser::show},
//                {"buy",Parser::buy},
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
                while (ch != ' ' && ch != EOF) {
                    tmp += static_cast<char>(ss.get());
                    ch=ss.peek();
                }
                if (tmp.length() == 1) {
                    throw SyntaxError();
                }
                res.push_back(tmp);
            }
            else if (ch0 == ' ') {
                do {
                    ss.ignore();
                } while (ss.peek() != EOF && ss.peek() == ' ');
            }
            else {
                std::string tmp;
                getline(ss,tmp,' ');
                res.push_back(tmp);
            }
        }
//        for (const auto &i : res) {
//            std::cerr << i << '\n';
//        }
        return res;
    }
    
    void Tool::destruct() {
        UserStatus::getInstance().~UserStatus();
    }

    void MainClass::init() {
        std::string inputLine;
        auto options = Tool::getOptions();
        while (getline(std::cin, inputLine)) {
        	if (inputLine == "passwd user1 user1_password user1_passworddd") {
        		std::cerr << "tmp";
        	}
	        if (inputLine == "su user0 user0_password") {
		        std::cerr << "tmp";
	        }
            try {
                const auto &option = Tool::split(inputLine);
	            if (option.empty()) {
	            	throw SyntaxError("No option");
	            }
	            if (option[0] == "quit" || option[0] == "exit") {
		            break;
	            }
                auto it = options.find(option.front());
                if (it == options.end()) {
                    throw NoOptionError();
                }
                else {
                    it->second(option);
                }
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
            std::cerr << '\n' << "operation " <<inputLine << '\n';
            UserStatus::getInstance().users.print();
        }
        Tool::destruct();
    }


}
