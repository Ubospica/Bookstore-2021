//
// Created by Ubospica on 2021/2/8.
//

#ifndef BOOKSTORE_2021_USERSTATUS_HPP
#define BOOKSTORE_2021_USERSTATUS_HPP

#include <unordered_map>
#include <vector>
#include "User.hpp"

namespace Bookstore {
    class UserStatus {
        //singleton class
    public:
        std::unordered_map<std::string,User> users {std::make_pair("root", User::getRoot())};
        User *curUserPtr = &User::getVisitor();

        static UserStatus& getInstance(){
            static UserStatus instance;
            return instance;
        }

        static void su(const std::vector<std::string>& s);
        static void logout(const std::vector<std::string>& s);
        static void useradd(const std::vector<std::string>& s);
        static void register_(const std::vector<std::string>& s);
        static void delete_(const std::vector<std::string>& s);
        static void passwd(const std::vector<std::string>& s);
    private:
        UserStatus() = default;
    };
}

#endif //BOOKSTORE_2021_USERSTATUS_HPP
