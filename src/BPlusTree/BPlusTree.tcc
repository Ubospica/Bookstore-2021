
#include "BPlusTree.hpp"

#include <iostream>
#include <vector>

namespace Bookstore {
	
	template <typename LValue, typename RValue, size_t M>
	struct BPlusTree<LValue, RValue, M>::Node{
		int cnt = 0;
		bool isRoot = false, isLeaf = false;
		int prev = -1, next = -1;
		Key vKey[M];
		int son[M + 1];
	};
	
	//-2 cur
	template <typename LValue, typename RValue, size_t M>
	template <typename T>
	inline void BPlusTree<LValue, RValue, M>::read(int pos, T &cur, std::fstream &fs) {
		int tmp = fs.rdstate();
		if(pos >= 0){
			fs.seekg(pos);
		}
		fs.read(reinterpret_cast<char*>(&cur), sizeof(cur));
	}
	template <typename LValue, typename RValue, size_t M>
	template <typename T>
	inline void BPlusTree<LValue, RValue, M>::read(int pos, T &cur) {
		read(pos, cur, treeDt);
	}
	
	//pos: -1 end; -2 cur
	template <typename LValue, typename RValue, size_t M>
	template <typename T>
	inline void BPlusTree<LValue, RValue, M>::write(int pos, const T &cur, std::fstream &fs) {
		if(pos >= 0){
			fs.seekp(pos);
		}
		else if(pos == -1) {
			fs.seekp(0, std::ios::end);
		}
		fs.write(reinterpret_cast<const char*>(&cur), sizeof(cur));
	}
	template <typename LValue, typename RValue, size_t M>
	template <typename T>
	inline void BPlusTree<LValue, RValue, M>::write(int pos, const T &cur) {
		write(pos, cur, treeDt);
	}
	
	template <typename LValue, typename RValue, size_t M>
	BPlusTree<LValue, RValue, M>::BPlusTree(const std::string& name) {
		std::string s1 = name + "Index.dat", s2 = name + "Data.dat";
		auto fl1 = std::ios::in | std::ios::out | std::ios::binary;
		auto fl2 = fl1;
		treeDt.open(s1, fl1);
		valueDt.open(s2, fl2);
		if ((!treeDt) || (!valueDt)) { //file not exists
			//trashy cpp grammar! `std::ofstream(s1)` won't compile
			std::ofstream(static_cast<std::string>(s1));
			std::ofstream(static_cast<std::string>(s2));
			treeDt.open(s1, fl1);
			valueDt.open(s2, fl2);
			if ((!treeDt) || (!valueDt)) {
				throw RuntimeError("File system error");
			}
			init();
		}
		else {
			read(0, root);
			read(-2, size);
			read(-2, height);
		}
	}
	
	
	template <typename LValue, typename RValue, size_t M>
	void BPlusTree<LValue, RValue, M>::init() {
		root = sizeof(root) * 3;
		height = size = 0;
		write(0, root);
		write(-2, size);
		write(-2, height);
		Node newRt {0, 1, 1};
		write(-2, newRt);
	}
	
	template <typename LValue, typename RValue, size_t M>
	int BPlusTree<LValue, RValue, M>::find(int pos, Key vKey) {
		Node cur;
		read(pos, cur);
		if (cur.cnt == 0) {
			return -1;
		}
		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
		if (!cur.isLeaf) { //simple node
			return find(cur.son[pl], vKey);
		}
		else {
			if (pl == 0 || cur.vKey[pl - 1] != vKey || cur.son[pl - 1] == -1) {
				return -1;
			}
			else {
				return cur.son[pl - 1];
			}
		}
	}
	
	template <typename LValue, typename RValue, size_t M>
	int BPlusTree<LValue, RValue, M>::find(const LValue &vl) {
		return find(root, ha(vl));
	}
	
	template <typename LValue, typename RValue, size_t M>
	RValue BPlusTree<LValue, RValue, M>::getVal(int pValue) {
		RValue res;
		read(pValue, res, valueDt);
		return res;
	}
	
	template <typename LValue, typename RValue, size_t M>
	void BPlusTree<LValue, RValue, M>::modifyVal(int pValue, const RValue &newVal) {
		write(pValue, newVal, valueDt);
	}
	
	
	//vSon == -1: don't have to insert nodes
	template <typename LValue, typename RValue, size_t M>
	bool BPlusTree<LValue, RValue, M>::insert(int pos, Key &vKey, int &vSon) {
		Node cur;
		read(pos, cur);
		if(cur.isLeaf) {
//				if (cur.cnt == 0) {
//					cur.vKey[cur.cnt] = vKey;
//					cur.son[cur.cnt] = vSon;
//					++cur.cnt;
//					write(pos, cur);
//					return true;
//				}
			int pl = std::lower_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
			if (pl < cur.cnt && cur.vKey[pl] == vKey) {
				if (cur.son[pl] >= 0) {
					return false;
				}
				else {
					cur.son[pl] = vSon;
					write(pos, cur);
					vSon = -1;
					return true;
				}
			}
			else {//can be improved
				for (int i = cur.cnt - 1; i >= pl; --i) {
					cur.vKey[i + 1] = cur.vKey[i];
					cur.son[i + 1] = cur.son[i];
				}
				cur.vKey[pl] = vKey;
				cur.son[pl] = vSon;
				++cur.cnt;
				if (cur.cnt < M) {
					write(pos, cur);
					vSon = -1;
					return true;
				}
				else { //split
					Node newNd {M - M / 2, 0, 1, pos};
					//1-M 1-m/2, m/2+1-m
					//0 - m/2-1, m/2 - m-1
					std::memcpy(newNd.vKey, cur.vKey + M / 2, newNd.cnt * sizeof(Key));
					std::memcpy(newNd.son, cur.son + M / 2, newNd.cnt * sizeof(int));
					cur.cnt = M / 2;
					if (!cur.isRoot) {
						treeDt.seekp(0, std::ios::end);
						int newP = treeDt.tellp();
						write(-1, newNd);
						cur.next = newP;
						write(pos, cur);
						vKey = newNd.vKey[0], vSon = newP;
//							argu = std::make_tuple(newP, newNd.vKey[0]);
					} else {
						cur.isRoot = 0;
						Node newRoot{1, 1, 0, -1, -1, {newNd.vKey[0]}, {pos}};
						treeDt.seekp(0, std::ios::end);
						int newP = treeDt.tellp();
						newRoot.son[1] = newP;
						cur.next = newP;
						write(-1, newNd);
						root = treeDt.tellp();
						write(-1, newRoot);
						write(pos, cur);
						write(0, root); //
						++height;
						treeDt.seekp(sizeof(int), std::ios::cur);
						write(-2, height);
						vKey = -1;
					}
					return true;
				}
			}
		}
		else { // non leaf
			int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
			if (insert(cur.son[pl], vKey, vSon) == false) {
				return false;
			}
			else if (vSon == -1) {
				return true;
			}
			else {//insert: son[pl+1] = get1 vkey[pl]=get2
				for (int i = cur.cnt - 1; i >= pl; --i) {
					cur.vKey[i + 1] = cur.vKey[i];
					cur.son[i + 2] = cur.son[i + 1];
				}
				cur.vKey[pl] = vKey;
				cur.son[pl + 1] = vSon;
				++cur.cnt;
				if (cur.cnt < M) {
					write(pos, cur); //
					vSon = -1;
					return true;
				}
				else {
					Node newNd {M - M / 2 - 1, 0, 0};
					//1-M 1-m/2, m/2+2-m
					//vkey:0 - m/2-1, m/2, m/2+1 - m-1
					//son: 0, m/2; m/2+1-m
					std::memcpy(newNd.vKey, cur.vKey + M / 2 + 1, newNd.cnt * sizeof(Key));
					std::memcpy(newNd.son, cur.son + M / 2 + 1, (newNd.cnt + 1) * sizeof(int));
					cur.cnt = M / 2;
					if (!cur.isRoot) {
						write(pos, cur);
						treeDt.seekp(0, std::ios::end);
						int newP = treeDt.tellp();
						write(-1, newNd);
						vKey = cur.vKey[cur.cnt];
						vSon = newP;
					} else {
						cur.isRoot = 0;
						write(pos, cur);
						Node newRoot{1, 1, 0, -1, -1, {cur.vKey[cur.cnt]}, {pos}}; //
						treeDt.seekp(0, std::ios::end);
						int newP = treeDt.tellp();
						newRoot.son[1] = newP;
						write(-1, newNd);
						root = treeDt.tellp();
						write(-1, newRoot);
						write(0, root); //
						++height;
						treeDt.seekp(sizeof(int), std::ios::cur);
						write(-2, height);
						vSon = -1;
					}
					return true;
				}
			}
		}
	}
	
	template <typename LValue, typename RValue, size_t M>
	int BPlusTree<LValue, RValue, M>::insert(const LValue &vl, const RValue &vr) {
		valueDt.seekp(0, std::ios::end); //
		Key vKey = ha(vl);
		int pValue = valueDt.tellp(), vSon = pValue;
//			std::cerr << "hash of " << vl << " is " << ha(vl) << '\n';
		if (insert(root, vKey, vSon)) {
			write(-1, vr, valueDt);
			++size;
			treeDt.seekp(sizeof(int), std::ios::beg);
			write(-2, size);
			return pValue;
		}
		else {
			return -1;
		}
	}
	
	// an idiotic & easily-implemented version
	// cuz i cannot erase the data (RValve) in "Data.dat"
	// so erasing index only is useless
	template <typename LValue, typename RValue, size_t M>
	bool BPlusTree<LValue, RValue, M>::erase(int pos, Key vKey) {
		Node cur;
		read(pos, cur);
		if (cur.cnt == 0) {
			return false;
		}
		int pl = std::upper_bound(cur.vKey, cur.vKey + cur.cnt, vKey) - cur.vKey;
		if (!cur.isLeaf) { //simple node
			return erase(cur.son[pl], vKey);
		}
		else {
			if (pl == 0 || cur.vKey[pl - 1] != vKey || cur.son[pl - 1] == -1) {
				return false;
			}
			else {
				cur.son[pl - 1] = -1;
				write(pos, cur);
				return true;
			}
		}
	}
	
	template <typename LValue, typename RValue, size_t M>
	bool BPlusTree<LValue, RValue, M>::erase(const LValue &vl) {
		if (erase(root, ha(vl))) {
			--size;
			treeDt.seekp(sizeof(int), std::ios::beg);
			write(-2, size);
			return true;
		}
		else {
			return false;
		}
	}
	
	template <typename LValue, typename RValue, size_t M>
	auto BPlusTree<LValue, RValue, M>::route() ->
			std::vector<std::tuple<Key, int>>  {
		std::vector<std::tuple<Key, int>> res;
		Node cur;
		read(root, cur);
		while (!cur.isLeaf) {
			read(cur.son[0], cur);
		}
		for (int i = 0; i < cur.cnt; ++i) {
			if (cur.son[i] != -1){
				res.push_back(std::make_tuple(cur.vKey[i], cur.son[i]));
			}
		}
		while (cur.next != -1) {
			read(cur.next, cur);
			for (int i = 0; i < cur.cnt; ++i) {
				if (cur.son[i] != -1){
					res.push_back(std::make_tuple(cur.vKey[i], cur.son[i]));
				}
			}
		}
		return res;
	}
	
	template <typename LValue, typename RValue, size_t M>
	void BPlusTree<LValue, RValue, M>::print (const BPlusTree::Node &p) {
		using namespace std;
		cerr << "cnt=" << p.cnt << " isroot=" << p.isRoot << " isLeaf=" << p.isLeaf << "\n";
		cerr << "vKey: ";
		for (int i = 0; i < p.cnt; ++i) {
			cerr << p.vKey[i] << ' ';
		}
		cerr << "\nson: ";
		for (int i = 0; i <= p.cnt; ++i) {
			cerr << p.son[i] << ' ';
		}
		cerr << '\n';
	}
	template <typename LValue, typename RValue, size_t M>
	void BPlusTree<LValue, RValue, M>::print (int pos) {
		using namespace std;
		if (pos == 36 || pos == 460) {
			cerr << "tmp";
		}
		Node cur;
		read(pos, cur);
		cerr << "pos = " << pos << " ";
		print(cur);
		if (cur.isLeaf) {
			RValue vr;
			cerr << "real value is ";
			for (int i = 0; i < cur.cnt; ++i) {
				if (cur.son[i] >= 0) {
					read(cur.son[i], vr, valueDt);
					cerr << vr << ' ';
				}
				else {
					cerr << "-1 ";
				}
			}
			cerr << '\n';
		}
		else {
			for (int i = 0; i <= cur.cnt; ++i) {
				print(cur.son[i]);
			}
		}
	}
	
	template <typename LValue, typename RValue, size_t M>
	void BPlusTree<LValue, RValue, M>::print () {
		std::cerr << "root=" << root << "\n";
		print(root);
		std::cerr<<"\n\n";
	}
}