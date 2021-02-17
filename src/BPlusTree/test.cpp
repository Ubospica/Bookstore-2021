//
// Created by Ubospica on 2021/2/12.
//


#include "BPlusTree.hpp"

#include <iostream>
#include <string>


int main() {
	using namespace Bookstore;
	using namespace std;
	BPlusTree<string, int, 3> bps("ww");
	int o, v;
	string s;
	cout<<boolalpha;
	while (cin>>o) {
		if (o == 1) {
			cin>>s;
			cin>>v;
			cout << bps.insert(s, v) << '\n';
//			bps.print();
		}
		else if (o == 2){
			cin>>s;
			int tmp = bps.find(s);
//			cerr << "find pos = " << tmp << '\n';
			if (tmp >= 0) cout << bps.getVal(tmp) << '\n';
			else cout << "Not found\n";
		}
		else if (o == 3) {
			cin>>s;
			cout << bps.erase(s) << '\n';
//			bps.print();
		}
		else if (o == 4) {
			auto rt = bps.route();
			for (const auto &i : rt) {
				auto tmp = get<0>(i);
				cout << get<0>(i) << ' ';
			}
			cout << '\n';
			for (const auto &i : rt) {
				cout << get<1>(i) << ' ';
			}
			cout << '\n';
			cout.flush();
		}
	}
}