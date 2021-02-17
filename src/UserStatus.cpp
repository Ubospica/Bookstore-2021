//
// Created by Ubospica on 2021/2/8.
//

#include "UserStatus.hpp"

#include <vector>
#include <string>

#include "Exception.hpp"

namespace Bookstore {

    void UserStatus::su(const std::vector<std::string>& s) {
        if (s.size() < 2) {
            throw SyntaxError("su: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        auto curUser = ins.getCurUser();
        if (curUser.perm < User::VISITOR) {
            throw PermissionError();
        }
        auto it = ins.users.find(s[1]);
        if (it == -1) {
            throw RunningError("su: User not found");
        }
        auto newUser = ins.users.getVal(it);
        if (curUser.perm > newUser.perm) {
            if (s.size() > 3) {
                throw SyntaxError("su: The number of argument is incorrect.");
            }
            ins.curUserPtr = it;
        }
        else {
            if (s.size() != 3) {
                throw SyntaxError("su: The number of argument is incorrect.");
            }
            if (!newUser.checkPasswd(s[2])) {
                throw RunningError("su: Passwd incorrect");
            }
            else {
                ins.curUserPtr = it;
            }
        }
//	    if (it == 496)
//	    	auto cc = ins.users.getVal(496);
    }
    void UserStatus::logout(const std::vector<std::string>& s) {
        if (s.size() != 1) {
            throw SyntaxError("logout: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
//        int tmp = ins.curUserPtr;
//        auto cc = ins.users.getVal(tmp);
	    auto curUser = ins.getCurUser();
        if (curUser.perm < User::USER) {
            throw PermissionError("logout");
        }
        ins.curUserPtr = ins.visiterPtr;
    }
    void UserStatus::useradd(const std::vector<std::string>& s) {
        if (s.size() != 5) {
            throw SyntaxError("useradd: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
	    auto curUser = ins.getCurUser();
        if (curUser.perm < User::ADMIN) {
            throw PermissionError("useradd");
        }
//        if (ins.users.find(s[1]) == -1) {
//            throw RunningError("User already exists.");
//        }
        int perm = s[3][0] - '0';
        if (s[3].length() > 1 || (perm != 7 && perm != 3 && perm != 1)) {
            throw SyntaxError("useradd");
        }
        if (curUser.perm <= perm) {
            throw PermissionError("useradd");
        }
        if (ins.users.insert(s[1], User(s[1], s[4], s[2], static_cast<User::Permission>(perm)))
        	    == -1) {
            throw RunningError("useradd: User already exists.");
        }
    }

    void UserStatus::register_(const std::vector<std::string>& s) {
        if (s.size() != 4) {
            throw SyntaxError("register: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
	    auto curUser = ins.getCurUser();
        if (curUser.perm < User::VISITOR) {
            throw PermissionError("register");
        }
//        if (ins.users.find(s[1]) != ins.users.end()) {
//            throw RunningError("User already exists.");
//        }
        if (ins.users.insert(s[1], User(s[1], s[3], s[2], User::USER))
                == -1) {
	        throw RunningError("register: User already exists.");
        }
    }
    
    void UserStatus::delete_(const std::vector<std::string>& s) {
        if (s.size() != 2) {
            throw SyntaxError("delete: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
	    auto curUser = ins.getCurUser();
        if (curUser.perm < User::SUPERUSER) {
            throw PermissionError("delete");
        }
        if (ins.users.erase(s[1]) == false){
	        throw RunningError("delete: User not exist.");
        }
    }

//passwd [user-id] [old-passwd(if not root)] [new-passwd] #1:root不需要填写旧密码，其余账户需要
    void UserStatus::passwd(const std::vector<std::string>& s) {
        if (s.size() < 3) {
            throw SyntaxError("passwd: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
		auto curUser = ins.getCurUser();
        if (curUser.perm < User::USER) {
            throw PermissionError("passwd");
        }
        auto it = ins.users.find(s[1]);
        if (it == -1) {
            throw RunningError("passwd: User not exist.");
        }
		auto newUser = ins.users.getVal(it);
        if (curUser.perm == User::SUPERUSER) {
            if (s.size() > 4) {
                throw SyntaxError("passwd: The number of argument is incorrect.");
            }
            if (s.size() == 3) {
                newUser.modifyPasswd(s[2]);
            }
            else {
	            newUser.modifyPasswd(s[3]);
            }
            ins.users.modifyVal(it, newUser);
        }
        else {
            if (s.size() != 4) {
                throw SyntaxError("passwd: The number of argument is incorrect.");
            }
            if (!newUser.checkPasswd(s[2])) {
                throw RunningError("passwd: Passwd incorrect");
            }
            else {
	            newUser.modifyPasswd(s[3]);
	            ins.users.modifyVal(it, newUser);
            }
        }
    }
}