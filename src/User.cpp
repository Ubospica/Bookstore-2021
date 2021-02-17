//
// Created by Ubospica on 2021/2/8.
//

#include "User.hpp"


namespace Bookstore {

    bool User::checkPasswd(const std::string &s) {
        return passwd == s;
    }

    void User::modifyPasswd(const std::string &newPwd) {
        strcpy(passwd, newPwd.c_str());
    }

    User& User::getRoot() {
        static User root("root","root","sjtu",SUPERUSER);
        return root;
    }

    User& User::getVisitor() {
        static User visitor("visitor","visitor","",VISITOR);
        return visitor;
    }

}