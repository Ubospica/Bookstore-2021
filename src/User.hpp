//
// Created by Ubospica on 2021/2/8.
//

#ifndef BOOKSTORE_2021_USER_HPP
#define BOOKSTORE_2021_USER_HPP

#include <cstring>
#include <ostream>
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
        const Permission perm = VISITOR;
        char userName[40];
        char userID[40];

        User() = default;
        User (std::string userID, std::string userName, const std::string& passwd, Permission perm) noexcept
            : perm(perm) {
            strcpy(this->userID, userID.c_str());
            strcpy(this->userName, userName.c_str());
	        strcpy(this->passwd, passwd.c_str());
        }

        User (const User &ano) = default;


        bool checkPasswd(const std::string &s);
        void modifyPasswd(const std::string &newPwd);

        static User& getRoot();
        static User& getVisitor();
        
		friend std::ostream& operator<<(std::ostream &os, User u) {
			os << "ID = " << u.userID << " Name = " << u.userName << " Pwd = " << u.passwd
				<< " Perm = " << u.perm << '\n';
			return os;
		}
    private:
        char passwd[40];
    };
	
}


#endif //BOOKSTORE_2021_USER_HPP
