//
// Created by Ubospica on 2021/2/8.
//

#ifndef BOOKSTORE_2021_USER_HPP
#define BOOKSTORE_2021_USER_HPP

#include <string>
#include <utility>

namespace Bookstore{

    class User {
    public:
        enum Permission {
            SUPERUSER = 7,
            ADMIN = 3,
            USER = 1,
            VISITOR = 0
        };
        const Permission perm;
        std::string userName;
        std::string userID;

        User (std::string userID, std::string userName, const std::string& passwd, Permission perm) noexcept
            : userID(std::move(userID)), userName(std::move(userName)), perm(perm) {
            ;`
        }

        User (const User &ano) = default;


        bool checkPasswd(const std::string &s);
        void modifyPasswd(const std::string &newPwd);

        static User& getRoot();

        static User& getVisitor();

    private:
        std::string passwd;
    };

}


#endif //BOOKSTORE_2021_USER_HPP
