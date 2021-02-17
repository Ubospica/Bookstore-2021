//
// Created by Ubospica on 2021/2/9.
//

#ifndef BOOKSTORE_2021_EXCEPTION_HPP
#define BOOKSTORE_2021_EXCEPTION_HPP

#include <exception>
#include <utility>
#include <string>

namespace Bookstore {
    class ExceptionBase : std::exception {
    private:
        std::string whatStr;
    public:
        explicit ExceptionBase(const std::string &str = std::string()) {
            if (!str.empty()) {
                whatStr = "Info: " + str;
            }
        }
        const char *what() const noexcept override {
            return whatStr.c_str();
        }
    };

    class NoOptionError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class SyntaxError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class RunningError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
    class PermissionError : public ExceptionBase {
        using ExceptionBase::ExceptionBase;
    };
	class RuntimeError : public ExceptionBase {
		using ExceptionBase::ExceptionBase;
	};
}

#endif //BOOKSTORE_2021_EXCEPTION_HPP
