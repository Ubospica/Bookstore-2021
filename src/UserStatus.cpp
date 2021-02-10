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
        if (ins.curUserPtr -> perm < User::VISITOR) {
            throw PermissionError();
        }
        auto it = ins.users.find(s[1]);
        if (it == ins.users.end()) {
            throw RunningError("User not found");
        }
        if (ins.curUserPtr -> perm > (it -> second).perm) {
            if (s.size() > 3) {
                throw SyntaxError("su: The number of argument is incorrect.");
            }
            ins.curUserPtr = &(it -> second);
        }
        else {
            if (s.size() != 3) {
                throw SyntaxError("su: The number of argument is incorrect.");
            }
            if (!(it -> second).checkPasswd(s[2])) {
                throw RunningError("Passwd incorrect");
            }
            else {
                ins.curUserPtr = &(it -> second);
            }
        }
    }
    void UserStatus::logout(const std::vector<std::string>& s) {
        if (s.size() != 1) {
            throw SyntaxError("logout: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        if (ins.curUserPtr -> perm < User::USER) {
            throw PermissionError();
        }
        ins.curUserPtr = &User::getVisitor();
    }
    void UserStatus::useradd(const std::vector<std::string>& s) {
        if (s.size() != 5) {
            throw SyntaxError("useradd: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        if (ins.curUserPtr -> perm < User::ADMIN) {
            throw PermissionError();
        }
        if (ins.users.find(s[1]) != ins.users.end()) {
            throw RunningError("User already exists.");
        }
        int perm = s[3][0] - '0';
        if (s[3].length() > 1 || (perm != 7 && perm != 3 && perm != 1)) {
            throw SyntaxError();
        }
        if (ins.curUserPtr -> perm <= perm) {
            throw PermissionError();
        }
        ins.users.insert(std::make_pair(s[1], User(s[1], s[4], s[2], static_cast<User::Permission>(perm))));
    }
//Object of type 'std::unordered_map<std::__cxx11::basic_string<char>, Bookstore::User,
// std::hash<std::string>, std::equal_to<std::__cxx11::basic_string<char>>,
// std::allocator<std::pair<const std::__cxx11::basic_string<char>, Bookstore::User>>>::mapped_type'
// (aka 'Bookstore::User') cannot be assigned because its copy assignment operator is implicitly deleted
//su [user-id] [passwd] #0:登录到某用户，从高权限用户登录到低权限不需填写密码
//logout #1:返回到未登录状态
//useradd [user-id] [passwd] [7/3/1] [name] #3:增加一个指定权限的用户，只能创建小于自己权限的账户
//register [user-id] [passwd] [name] #0:注册一个带有这些信息的权限1用户
//delete [user-id] #7:删除某用户
//[user-id]、[passwd]包含连续无空格的数字字母下划线，不超过30个字符。
//
//[name] 包含连续无空格的字符串，不超过30个字符。

    void UserStatus::register_(const std::vector<std::string>& s) {
        if (s.size() != 5) {
            throw SyntaxError("register: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        if (ins.curUserPtr -> perm < User::ADMIN) {
            throw PermissionError();
        }
        if (ins.users.find(s[1]) != ins.users.end()) {
            throw RunningError("User already exists.");
        }
        ins.users.insert(std::make_pair(s[1], User(s[1], s[3], s[2], User::USER)));
    }
    void UserStatus::delete_(const std::vector<std::string>& s) {
        if (s.size() != 2) {
            throw SyntaxError("delete: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        if (ins.curUserPtr -> perm < User::SUPERUSER) {
            throw PermissionError();
        }
        auto it = ins.users.find(s[1]);
        if (it != ins.users.end()) {
            throw RunningError("User not exist.");
        }
        ins.users.erase(it);
    }

//passwd [user-id] [old-passwd(if not root)] [new-passwd] #1:root不需要填写旧密码，其余账户需要
    void UserStatus::passwd(const std::vector<std::string>& s) {
        if (s.size() < 3) {
            throw SyntaxError("delete: The number of argument is incorrect.");
        }
        auto &ins = getInstance();
        if (ins.curUserPtr -> perm < User::SUPERUSER) {
            throw PermissionError();
        }
        auto it = ins.users.find(s[1]);
        if (it != ins.users.end()) {
            throw RunningError("User not exist.");
        }
        if (ins.curUserPtr -> perm == User::SUPERUSER) {
            if (s.size() > 4) {
                throw SyntaxError("delete: The number of argument is incorrect.");
            }
            if (s.size() == 3) {
                (it -> second).modifyPasswd(s[2]);
            }
            else {
                (it -> second).modifyPasswd(s[3]);
            }
        }
        else {
            if (s.size() != 4) {
                throw SyntaxError("delete: The number of argument is incorrect.");
            }
            if (!(it -> second).checkPasswd(s[2])) {
                throw RunningError("Passwd incorrect");
            }
            else {
                (it -> second).modifyPasswd(s[3]);
            }
        }
    }
}