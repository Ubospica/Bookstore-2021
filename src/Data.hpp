//
// Created by Ubospica on 2021/2/17.
//

#ifndef BOOKSTORE_2021_DATA_HPP
#define BOOKSTORE_2021_DATA_HPP


#include <cstring>
#include <string>
#include <iostream>

struct Data {
	char val[65] {0};
	char isbn[25] {0};
	enum class CmpType{
		VAL,
		VAL_ISBN,
		TRUE,
	};
	static CmpType cmpType;
	Data() = default;
	Data (const std::string &s1, const std::string &s2) {
		strcpy(val, s1.c_str());
		strcpy(isbn, s2.c_str());
	}
	Data (const std::string &s1) : Data(s1, std::string()) { }
	Data (const Data &ano) {
		strcpy(val, ano.val);
		strcpy(isbn, ano.isbn);
	}
	Data& operator = (const Data &ano) {
		if (this == &ano) {
			return *this;
		}
		strcpy(val, ano.val);
		strcpy(isbn, ano.isbn);
		return *this;
	}
	
	bool operator < (const Data &ano) const {
		if (cmpType == CmpType::TRUE) {
			return false;
		}
		else if (cmpType == CmpType::VAL) {
			return std::strcmp(val, ano.val) < 0;
		}
		else {
			int tmp = std::strcmp(val, ano.val);
			return tmp != 0 ? tmp < 0 : std::strcmp(isbn, ano.isbn) < 0;
		}
	}
	
	bool operator == (const Data &ano) const {
		if (cmpType == CmpType::TRUE) {
			return true;
		}
		else if (cmpType == CmpType::VAL) {
			return std::strcmp(val, ano.val) == 0;
		}
		else {
			return std::strcmp(val, ano.val) != 0 ? false : std::strcmp(isbn, ano.isbn) == 0;
		}
	}
	
	bool operator != (const Data &ano) const {
		return !(*this == ano);
	}
	
	friend std::ostream& operator << (std::ostream &os, const Data &s) {
		os << s.val;// << ' ' << s.isbn;
		return os;
	}
};


#endif //BOOKSTORE_2021_DATA_HPP
