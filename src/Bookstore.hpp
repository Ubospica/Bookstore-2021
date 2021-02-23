//
// Created by Ubospica on 2021/2/17.
//

#ifndef BOOKSTORE_2021_BOOKSTORE_HPP
#define BOOKSTORE_2021_BOOKSTORE_HPP

#include <string>

#include "Book.hpp"
#include "BPlusTree.hpp"
//select [ISBN] #3:选定ISBN为指定值的图书，若不存在则创建该ISBN的书并将其余信息留空等待modify进行填写
//	modify -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] -price=[price] #3:参数可省略，也不要求输入顺序，会更新(替换而非添加)上次选中的书至新的信息
//	import [quantity] [cost_price(in total)] #3:将上次选中的书以总共[cost_price]的价格进[quantity]本
//	show -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] #1:参数可省略，也不要求输入顺序，将匹配的图书以ISBN号排序输出，需要注意该命令参数只支持单参数，如果参数是keyword，只支持单关键词查询
//	show finance [time] #7:time项省略时，输出总的收入与支出;否则输出近[time]次进货、卖出操作(分别算一次)的合计收入支出。
//buy [ISBN] [quantity] #1:购买该ISBN号的图书[quantity]本
namespace Bookstore {
	class Bookstore {
//	private:
	public:
		BPlusTree<Data, Book> isbnIndex;
		BPlusTree<Data, int> nameIndex;
		BPlusTree<Data, int> authorIndex;
		BPlusTree<Data, int> kwdIndex;
		std::fstream accountLog;
		// accountLog structure:
		// count in out [<in,out>]...
		
		struct Account {
			double val[2];
			Account operator + (const Account &b) const {
				return (Account){val[0] + b.val[0], val[1] + b.val[1]};
			}
			Account& operator += (const Account &b) {
				val[0] += b.val[0], val[1] += b.val[1];
				return *this;
			}
			friend std::ostream& operator << (std::ostream &os, const Account &one) {
				auto fl = std::cout.flags();
				std::cout << std::fixed << std::setprecision(2);
				std::cout << "+ " << one.val[0] << " - " << one.val[1];
				std::cout.flags(fl);
				return os;
			}
		};
		
		Bookstore() : isbnIndex("IsbnIndex"), nameIndex("NameIndex"),
				authorIndex("AuthorIndex"), kwdIndex("KwdIndex"),
				accountLog("AccountLog.dat", std::ios::in | std::ios::out | std::ios::binary) {
			if (accountLog.fail()) {
				std::ofstream("AccountLog.dat");
				accountLog.open("AccountLog.dat", std::ios::in | std::ios::out | std::ios::binary);
				if (accountLog.fail()) {
					throw RuntimeError("File system error");
				}
				int tmp1 = 0;
				Account total {0, 0};
				accountLog.write(reinterpret_cast<const char*>(&tmp1), sizeof(tmp1));
				accountLog.write(reinterpret_cast<const char*>(&total), sizeof(total));
			}
		}
		
		int insertBook(const Book &one, int pos);
		void deleteBook(const Book &one);
		void addDeal(const Account &deal);
		
	
	public:
		static void select(const std::vector<std::string> &option);
		static void modify(const std::vector<std::string> &option);
		static void import(const std::vector<std::string>& option);
		static void show(const std::vector<std::string>& option);
		static void showFinance(const std::vector<std::string>& option);
		static void buy(const std::vector<std::string>& option);
		
		static Bookstore& getInstance() {
			static Bookstore instance;
			return instance;
		}
	};
}


#endif //BOOKSTORE_2021_BOOKSTORE_HPP
