//
// Created by Ubospica on 2021/2/17.
//

#include "Bookstore.hpp"
#include "UserStatus.hpp"
#include "Exception.hpp"

//select [ISBN] #3:选定ISBN为指定值的图书，若不存在则创建该ISBN的书并将其余信息留空等待modify进行填写
//modify -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] -price=[price] #3:参数可省略，也不要求输入顺序，会更新(替换而非添加)上次选中的书至新的信息

//
//[ISBN] 为不超过20位的连续字符串
//[name] 和 [author] 被双引号包起来(不含双引号)，保证
//本身内容没有双引号
//内容不超过60个英文字符
//[keyword] 整体被双引号包起来(不含双引号)，关键字之间| (竖线)隔开，保证
//每个关键字本身内容无引号以及空格
//内容总共不超过60个英文字符
//[quantity] 为整数(<1000000)
//

namespace Bookstore {
	
	class IOTool {
	public:
		static bool chkInt(const std::string &s, int &val) { //non-negative
			std::stringstream ss(s);
			ss >> val;
			if (ss.fail() || ss.peek() != EOF || val < 0) {
				return false;
			}
			else {
				return true;
			}
		}
		static bool chkFloat(const std::string &s, double &val) { //non-negative
			std::stringstream ss(s);
			ss >> val;
			if (ss.fail() || ss.peek() != EOF || val < 0) {
				return false;
			}
			else {
				return true;
			}
		}
	};
	
	void Bookstore::deleteBook(const Book &one) {
		isbnIndex.erase(Data(one.ISBN));
		if (one.name[0]) nameIndex.erase(Data(one.name, one.ISBN));
		if (one.author[0]) authorIndex.erase(Data(one.author, one.ISBN));
		for (int i = 0; i < one.kwdCnt; ++i) {
			kwdIndex.erase(Data(one.keyword[i], one.ISBN));
		}
	}
	
	int Bookstore::insertBook(const Book &one, int pos) {
		if (isbnIndex.insertIndex(Data(one.ISBN), pos) == -1) {
			throw RunningError("modify: book isbn exists");
		}
		if (one.name[0]) nameIndex.insertIndex(Data(one.name, one.ISBN), pos);
		if (one.author[0]) authorIndex.insertIndex(Data(one.author, one.ISBN), pos);
		for (int i = 0; i < one.kwdCnt; ++i) {
			kwdIndex.insertIndex(Data(one.keyword[i], one.ISBN), pos);
		}
//		kwdIndex.print();
		return pos;
	}
	
	void Bookstore::select(const std::vector<std::string> &option) {
		if (option.size() != 2 || option[1].size() > 20) {
			throw SyntaxError("select");
		}
		auto &usIns = UserStatus::getInstance();
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::ADMIN) {
			throw PermissionError("select");
		}
		auto &ins = getInstance();
		int tmp = ins.isbnIndex.find(option[1]);
		if (tmp == -1) {
			Book newBook;
			newBook.setISBN(option[1]);
			tmp = ins.isbnIndex.insert(option[1], newBook);
//			tmp = ins.insertBook(newBook);
		}
		std::get<1>(usIns.curUser.back()) = tmp;
		
//		ins.isbnIndex.print();
	}
	
	void Bookstore::modify(const std::vector<std::string> &option) {
		auto &usIns = UserStatus::getInstance();
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::ADMIN) {
			throw PermissionError("modify");
		}
		auto &ins = getInstance();
		auto curBookPtr = std::get<1>(usIns.curUser.back());
		if (curBookPtr == -1) {
			throw RunningError("modify: book not selected");
		}
		auto curBook = ins.isbnIndex.getVal(curBookPtr);
		ins.deleteBook(curBook);
//		std::cerr << "before modify:\n";
//		ins.isbnIndex.print();
		for (int i = 1; i < option.size(); i += 2) {
			if (i + 1 >= option.size() || option[i][0] != '-' || option[i + 1][0] != '='){
				throw SyntaxError("modify");
			}
			std::string nextOption (option[i + 1].begin() + 1, option[i + 1].end());
			if (option[i] == "-ISBN") {
				if (nextOption == curBook.ISBN) {
					throw RunningError("isbn is not changed");
				}
				curBook.setISBN(nextOption);
			}
			else if (option[i] == "-name") {
				curBook.setName(nextOption);
			}
			else if (option[i] == "-author") {
				curBook.setAuthor(nextOption);
			}
			else if (option[i] == "-keyword") {
				curBook.setKwd(nextOption);
			}
			else if (option[i] == "-price") {
				double price;
				if (!IOTool::chkFloat(nextOption, price)) {
					throw SyntaxError("modify");
				}
				else {
					curBook.setPrice(price);
				}
			}
			else {
				throw SyntaxError("modify");
			}
		}
		ins.isbnIndex.modifyVal(curBookPtr, curBook);
		ins.insertBook(curBook, curBookPtr);
//		if (newBookPtr != curBookPtr) {
//			for (auto &i : usIns.curUser) {
//				if (std::get<1>(i) == curBookPtr) {
//					std::get<1>(i) = newBookPtr;
//				}
//			}
//		}
//		std::cerr << "after modify:\n";
//		ins.isbnIndex.print();
//		std::cerr << "curBookPtr = " << std::get<1>(usIns.curUser.back()) << '\n';
	}
	


	
	void Bookstore::addDeal(const Account &deal) {
		int cnt;
		Account total{};
		accountLog.seekg(0);
		accountLog.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
		accountLog.read(reinterpret_cast<char*>(&total), sizeof(total));
		++cnt;
		total += deal;
		accountLog.seekp(0);
		accountLog.write(reinterpret_cast<const char*>(&cnt), sizeof(cnt));
		accountLog.write(reinterpret_cast<const char*>(&total), sizeof(total));
		accountLog.seekp(0, std::ios::end);
		accountLog.write(reinterpret_cast<const char*>(&deal), sizeof(deal));
	}
	
	void Bookstore::import(const std::vector<std::string>& option) {
		if (option.size() != 3) {
			throw SyntaxError("import");
		}
		auto &usIns = UserStatus::getInstance();
		auto curBookPtr = std::get<1>(usIns.curUser.back());
		if (curBookPtr == -1) {
			throw RunningError("import: book not selected");
		}
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::ADMIN) {
			throw PermissionError("import");
		}
		auto &ins = getInstance();
		auto curBook = ins.isbnIndex.getVal(curBookPtr);
		
		int quantity;
		double totalPrice;
		if ((!IOTool::chkInt(option[1], quantity)) || (!IOTool::chkFloat(option[2], totalPrice))) {
			throw SyntaxError("import");
		}
		curBook.quantity += quantity;
		ins.isbnIndex.modifyVal(curBookPtr, curBook);
		Account deal{0, totalPrice};
		ins.addDeal(deal);
	}
	//import [quantity] [cost_price(in total)] #3:将上次选中的书以总共[cost_price]的价格进[quantity]本
//show -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] #1:参数可省略，也不要求输入顺序，将匹配的图书以ISBN号排序输出，需要注意该命令参数只支持单参数，如果参数是keyword，只支持单关键词查询
//show finance [time] #7:time项省略时，输出总的收入与支出;否则输出近[time]次进货、卖出操作(分别算一次)的合计收入支出。
//buy [ISBN] [quantity] #1:购买该ISBN号的图书[quantity]本
	//chk whether every item of option is empty!
	void Bookstore::show(const std::vector<std::string>& option) {
		if (option.size() != 1 && option.size() != 3) {
			throw SyntaxError("show");
		}
		auto &usIns = UserStatus::getInstance();
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::USER) {
			throw PermissionError("show");
		}
		auto &ins = getInstance();
		std::vector<int> res;
		if (option.size() == 1) {
			Data::cmpType = Data::CmpType::TRUE;
			res = ins.isbnIndex.route(Data());
		}
		else {
			std::string nextOption (option[2].begin() + 1, option[2].end());
			Data::cmpType = Data::CmpType::VAL;
			if (option[1] == "-ISBN") {
				res = ins.isbnIndex.route(nextOption);
			}
			else if (option[1] == "-name") {
				res = ins.nameIndex.route(nextOption);
			}
			else if (option[1] == "-author") {
				res = ins.authorIndex.route(nextOption);
			}
			else if (option[1] == "-keyword") {
//				ins.kwdIndex.print();
				res = ins.kwdIndex.route(nextOption);
			}
			else {
				Data::cmpType = Data::CmpType::VAL_ISBN;
				throw SyntaxError("show");
			}
		}
		Data::cmpType = Data::CmpType::VAL_ISBN;
		if (res.empty()) {
			std::cout << '\n';
		}
		else {
			std::vector<Book> resBook;
			resBook.reserve(res.size());
			for (auto i : res) {
				resBook.push_back(ins.isbnIndex.getVal(i));
			}
			if (!(option.size() == 1 || option[1] == "-ISBN")) {
				std::sort(resBook.begin(), resBook.end(), [](const Book &a, const Book &b) {
					return strcmp(a.ISBN, b.ISBN) < 0;
				});
			}
			for (const auto &i : resBook) {
				std::cout << i << '\n';
			}
		}
	}
	
	//IO can be refactored
	void Bookstore::showFinance(const std::vector<std::string>& option) {
		if (option.size() != 2 && option.size() != 3) {
			throw SyntaxError("showFinance");
		}
		auto &usIns = UserStatus::getInstance();
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::SUPERUSER) {
			throw PermissionError("showFinance");
		}
		auto &ins = getInstance();
		if (option.size() == 2) {
			Account total{};
			ins.accountLog.seekg(sizeof(int));
			ins.accountLog.read(reinterpret_cast<char*>(&total), sizeof(total));
			std::cout << total << '\n';
		}
		else {
			int val;
			if (!IOTool::chkInt(option[2], val)) {
				throw SyntaxError("showFinance");
			}
			int cnt;
			ins.accountLog.seekg(0);
			ins.accountLog.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
			if (cnt < val) {
				throw RunningError("showFinance: time is too large");
			}
			Account total{0, 0}, cur{};
			ins.accountLog.seekg(0, std::ios::end);
			for (int i = 0; i < val; ++i) {
				ins.accountLog.seekg(-sizeof(Account), std::ios::cur);
				ins.accountLog.read(reinterpret_cast<char*>(&cur), sizeof(cur));
				ins.accountLog.seekg(-sizeof(Account), std::ios::cur);
				total += cur;
			}
			std::cout << total << '\n';
		}
	}
	//buy [ISBN] [quantity] #1:购买该ISBN号的图书[quantity]本
	void Bookstore::buy(const std::vector<std::string>& option) {
		if (option.size() != 3) {
			throw SyntaxError("buy");
		}
		auto &usIns = UserStatus::getInstance();
		User curUser = usIns.getCurUser();
		if (curUser.perm < User::USER) {
			throw PermissionError("buy");
		}
		auto &ins = getInstance();
		int curBookPtr = ins.isbnIndex.find(option[1]);
		if (curBookPtr == -1) {
			throw RunningError("buy: Book not found");
		}
		int quantity;
		if (!IOTool::chkInt(option[2], quantity)) {
			throw SyntaxError("buy");
		}
		auto curBook = ins.isbnIndex.getVal(curBookPtr);
//		if (curBook.price == -1) {
//			if (quantity == 0) {
//				std::cout << std::fixed << std::setprecision(2) << 0.0 << '\n';
//			}
//			else {
//				throw RunningError("buy: price not specified");
//			}
//		}
		if (curBook.quantity < quantity) {
			throw RunningError("buy: not enough book available");
		}
		curBook.quantity -= quantity;
		ins.isbnIndex.modifyVal(curBookPtr, curBook);
		std::cout << std::fixed << std::setprecision(2) << quantity * curBook.price << '\n'; //and cancel
		Account deal{quantity * curBook.price, 0};
		ins.addDeal(deal);
	}
}