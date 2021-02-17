//
// Created by Ubospica on 2021/2/8.
//

#ifndef BOOKSTORE_2021_USERSTATUS_HPP
#define BOOKSTORE_2021_USERSTATUS_HPP

#include <unordered_map>
#include <vector>
#include "BPlusTree.hpp"
#include "User.hpp"

namespace Bookstore {
    class UserStatus {
        //singleton class
    public:
        BPlusTree<std::string, User, 100> users;
        int curUserPtr, visiterPtr, rootPtr;

        static UserStatus& getInstance(){
            static UserStatus instance;
            return instance;
        }
        User getCurUser() {
        	int tmp = curUserPtr;
        	auto tmp1 = users.getVal(curUserPtr);
        	return users.getVal(curUserPtr);
        }

        static void su(const std::vector<std::string>& s);
        static void logout(const std::vector<std::string>& s);
        static void useradd(const std::vector<std::string>& s);
        static void register_(const std::vector<std::string>& s);
        static void delete_(const std::vector<std::string>& s);
        static void passwd(const std::vector<std::string>& s);
    private:
        UserStatus() : users("Users") {
	        visiterPtr = users.insert("__visitor", User::getVisitor());
	        rootPtr = users.insert("root", User::getRoot());
	        curUserPtr = visiterPtr;
        }
    };
}

#endif //BOOKSTORE_2021_USERSTATUS_HPP
