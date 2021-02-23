//
// Created by Ubospica on 2021/2/8.
//

#ifndef BOOKSTORE_2021_USERSTATUS_HPP
#define BOOKSTORE_2021_USERSTATUS_HPP

#include <unordered_map>
#include <vector>
#include <stack>
#include "Book.hpp"
#include "BPlusTree.hpp"
#include "User.hpp"

namespace Bookstore {
    class UserStatus {
        //singleton class
    public:
        BPlusTree<Data, User, 100> users;
        int visiterPtr;//, rootPtr;
        std::vector<std::tuple<int, int>> curUser;

        static UserStatus& getInstance(){
            static UserStatus instance;
            return instance;
        }
        User getCurUser() {
        	return users.getVal(std::get<0>(curUser.back()));
        }

        static void su(const std::vector<std::string>& s);
        static void logout(const std::vector<std::string>& s);
        static void useradd(const std::vector<std::string>& s);
        static void register_(const std::vector<std::string>& s);
        static void delete_(const std::vector<std::string>& s);
        static void passwd(const std::vector<std::string>& s);
    private:
        UserStatus() : users("Users") {
	        visiterPtr = users.find(Data("__visitor"));
	        if (visiterPtr == -1) {
	        	visiterPtr = users.insert(Data("__visitor"), User::getVisitor());
	            users.insert(Data("root"), User::getRoot());
	        }
	        curUser.push_back(std::make_tuple(visiterPtr, -1));
        }
    };
}

#endif //BOOKSTORE_2021_USERSTATUS_HPP
