//
// Created by Ubospica on 2021/2/17.
//

#ifndef BOOKSTORE_2021_BOOK_HPP
#define BOOKSTORE_2021_BOOK_HPP

#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>

#include "Exception.hpp"

namespace Bookstore {
	class Book {
	public:
		char ISBN[25] {0}, name[65] {0}, author[65] {0}, keyword[60][65] {{0}};
		int quantity = 0, kwdCnt = 0;
		double price = 0;
		
		void setISBN(const std::string& s) {
			std::strcpy(ISBN, s.c_str());
		}
		void setName(const std::string& s) {
			std::strcpy(name, s.c_str());
		}
		void setAuthor(const std::string& s) {
			std::strcpy(author, s.c_str());
		}
		void setKwd(const std::string &s) {
			static char newKwd[65][65];
			int newKwdCnt = 0;
			std::stringstream ss(s);
			while (ss.getline(newKwd[newKwdCnt], 65, '|')) {
				if (newKwd[newKwdCnt][0] == 0) {
					throw SyntaxError("modify");
				}
				++newKwdCnt;
			}
			for (int i = 0; i < newKwdCnt; ++i) {
				for (int j = i + 1; j < newKwdCnt; ++j) {
					if (strcmp(newKwd[i], newKwd[j]) == 0) {
						throw RunningError("modify: keyword is repeated");
					}
				}
			}
			kwdCnt = newKwdCnt;
			for (int i = 0; i < newKwdCnt; ++i) {
				strcpy(keyword[i], newKwd[i]);
			}
		}
		void setPrice(double newPrice) {
			price = newPrice;
		}
		
		friend std::ostream &operator<<(std::ostream &os, const Book &one) {
			auto tmp = os.flags();
			os << one.ISBN << '\t' << one.name << '\t' << one.author << '\t';
			for (int i = 0; i < one.kwdCnt; ++i) {
				os << one.keyword[i] << (i == one.kwdCnt - 1 ? "" : "|");
			}
			os << '\t';
			os << std::fixed << std::setprecision(2) << one.price << '\t' << one.quantity;
			os.flags(tmp);
			return os;
		}
	};
}


#endif //BOOKSTORE_2021_BOOK_HPP
