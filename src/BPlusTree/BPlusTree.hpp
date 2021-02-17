//
// Created by Ubospica on 2021/2/12.
//

#ifndef BOOKSTORE_2021_BPLUSTREE_HPP
#define BOOKSTORE_2021_BPLUSTREE_HPP

#include "Exception.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <tuple>


namespace Bookstore{

	template <typename LValue, typename RValue, size_t M>
	class BPlusTree {
	private:
		using Key = size_t;
		struct Node;
	public:
		explicit BPlusTree(const std::string& name);
		~BPlusTree() = default;
		int find(const LValue &vl);
		RValue getVal(int pValue);
		void modifyVal(int pValue, const RValue &newVal);
		int insert(const LValue &vl, const RValue &vr);
		bool erase(const LValue &vl);
		std::vector<std::tuple<Key, int>> route();
		
		//debug
		void print (const Node &p);
		void print (int pos);
		void print ();
		
//	private:
	public:
		std::fstream treeDt, valueDt;
		std::hash<LValue> ha;
		int root, height, size;
		
		template <typename T> inline void read(int pos, T &cur, std::fstream &fs);
		template <typename T> inline void read(int pos, T &cur);
		
		template <typename T> inline void write(int pos, const T &cur, std::fstream &fs);
		template <typename T> inline void write(int pos, const T &cur);
		
		void init();
		int find(int pos, Key vKey);
		bool insert(int pos, Key &vKey, int &vSon);
		bool erase(int pos, Key vKey);
		
	};

} // namespace Bookstore

//template implementation
#include "BPlusTree.tcc"

#endif //BOOKSTORE_2021_BPLUSTREE_HPP
