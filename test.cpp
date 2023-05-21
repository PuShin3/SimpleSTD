#include<iostream>
#include<vector>
#include<unordered_map>
#include<chrono>
#include "vector.hpp"
#include "Array.hpp"
#include "Debug/Time.hpp"
#include "Debug/Debug.hpp"
#include "unordered_map.hpp"

// This is really just for testing

#define initTimeFunc() sstd::Clock _Clock_To_Time_Things
#define timeFunc(varname, func) func auto varname = _Clock_To_Time_Things.End().asMilli

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using sstd::Decimal;
using sstd::print;
using sstd::sizet;

struct v {
	int x, y;
	v() = default;
	v(int i):x(i),y(i){}
	v(v&& o) {
		x = o.x;
		y = o.y;
	}
	v(const v& o) {
		x = o.x;
		y = o.y;
	}
	v& operator=(const v& o) {
		x = o.x;
		y = o.y;
		return *this;
	}
};
struct KeyHasher
{
	int m_data[1000000];
	KeyHasher() {
		for (int i = 0; i < 1000000; ++i) {
			m_data[i] = i;
		}
	}
	std::size_t operator()(const v& k) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		return m_data[k.x];
	}
};
void test_vector() {
	initTimeFunc();
	sstd::vector<v> sstd_vector({ 1, 2, 3 });
	std::vector<v> std_vector;
	sstd::vector<v> boom({ 1, 2, 3, 4, 5 });
	for (int i = 0; i < 50000; ++i) {
		boom.emplace_back(i);
	}
	//for (int i = 1; i <= 10; i *= 10) {
	//	const sizet _Size = 5000000 * i;
	//	print(_Size, " elements: ");
	//	timeFunc(sstd_emplace, for (int i = 0; i < _Size; ++i) {
	//		sstd_vector.emplace_back(i);
	//	});
	//	timeFunc(sstd_insert, sstd_vector.insert(200, boom.begin(), boom.end()););
	//	timeFunc(sstd_clear, sstd_vector.clear(););

	//	timeFunc(std_emplace, for (int i = 0; i < _Size; ++i) {
	//		std_vector.emplace_back(i);
	//	});
	//	timeFunc(std_insert, std_vector.insert(std_vector.begin() + 200, boom.data(), boom.data() + 2000););
	//	timeFunc(std_clear, std_vector.clear(););

	//	print("		custom sstd::vector	emplace_back:		", sstd_emplace, " ms");
	//	print("		std::vector		emplace_back:		", std_emplace, " ms\n");
	//	print("		custom sstd::vector	insert:			", sstd_insert, " ms");
	//	print("		std::vector		insert:			", std_insert, " ms\n");
	//	print("		custom sstd::vector	clear:			", sstd_clear, " ms");
	//	print("		std::vector		clear:			", std_clear, " ms\n");
	//}

	for (int i = 1; i <= 10; i *= 10) {
		const sizet _Size = 5000000 * i;
		print(_Size, " elements with reserve: ");
		sstd_vector.reserve(_Size+100000);
		std_vector.reserve(_Size+100000);
		timeFunc(sstd_emplace, for (int i = 0; i < _Size; ++i) {
			sstd_vector.emplace_back(i);
		});
		timeFunc(sstd_insert, sstd_vector.insert(200, boom.begin(), boom.end()););
		timeFunc(sstd_clear, sstd_vector.clear(););

		timeFunc(std_emplace, for (int i = 0; i < _Size; ++i) {
			std_vector.emplace_back(i);
		});
		timeFunc(std_insert, std_vector.insert(std_vector.begin() + 200, boom.data(), boom.data() + 2000););
		timeFunc(std_clear, std_vector.clear(););

		print("		custom sstd::vector	emplace_back:		", sstd_emplace, " ms");
		print("		std::vector		emplace_back:		", std_emplace, " ms\n");
		print("		custom sstd::vector	insert:			", sstd_insert, " ms");
		print("		std::vector		insert:			", std_insert, " ms\n");
		print("		custom sstd::vector	clear:			", sstd_clear, " ms");
		print("		std::vector		clear:			", std_clear, " ms");
	}
}
void test_unordered_map() {
	initTimeFunc();
	sstd::unordered_map<int, int> sstd_map;
	std::unordered_map<int, int> std_map;
	for (int i = 1; i <= 100; i *= 10) {
		const sizet _Size = 500 * i;
		print(_Size, " elements: ");
		timeFunc(sstd_insert, for (int i = 0; i < _Size; ++i) {
			sstd_map.insert({ i, i });
		});
		timeFunc(sstd_search, for (int i = 0; i < _Size; ++i) {
			sstd_map[i] = i * 2;
		});
		timeFunc(sstd_clear, sstd_map.clear(););

		timeFunc(std_insert, for (int i = 0; i < _Size; ++i) {
			std_map.insert({ i, i });
		});
		timeFunc(std_search, for (int i = 0; i < _Size; ++i) {
			std_map[i] = i * 2;
		});
		timeFunc(std_clear, std_map.clear(););

		print("		custom sstd::unordered_map	insert:			", sstd_insert, " ms");
		print("		std::unordered_map		insert:			", std_insert, " ms\n");
		print("		custom sstd::unordered_map	search:			", sstd_search, " ms");
		print("		std::unordered_map		search:			", std_search, " ms\n");
		print("		custom sstd::unordered_map	clear:			", sstd_clear, " ms");
		print("		std::unordered_map		clear:			", std_clear, " ms\n");
	}
}
int main() {
	//test_vector();
	test_unordered_map();
	return 0;
}