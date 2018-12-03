#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <cstring>

namespace Palindrome {
typedef char Alpha;

std::vector<int> getDiameters(const Alpha *str, int n) {
	std::vector<int> diameters(n * 2 - 1);
	int center = 0, right = 0;
	for(int i = 0; i < n * 2 - 1; i ++) {
		int maxp = std::min(i + 1, n * 2 - 1 - i);
		int p = right > i ? std::min(right - i, diameters[2 * center - i]) : 0;
		if((i + p) % 2 == 0) p ++;
		while(p < maxp && str[(i + p + 1) / 2] == str[(i - p - 1) / 2]) p += 2;
		p = std::min(p, maxp);
		if(i + p > right) {
			center = i;
			right = i + p;
		}
		diameters[i] = p;
	}
	return diameters;
}

//(ŠJŽn, ’·‚³)
std::pair<int,int> longestPalindromicSubstring(const std::vector<int> &diameters) {
	int maxLen = 0, maxLenIndex = 0;
	for(int i = 0; i < (int)diameters.size(); i ++) {
		if(diameters[i] > maxLen) {
			maxLen = diameters[i];
			maxLenIndex = i;
		}
	}
	return std::make_pair((maxLenIndex - maxLen + 1) / 2, maxLen);
}

}

int main() {
	while(1) {
	std::string str;
	std::getline(std::cin, str);
	std::vector<int> diameters = Palindrome::getDiameters(str.c_str(), str.length());
	for(int i = 0; i < (int)diameters.size(); i ++) {
		std::cout << diameters[i];
		if(i+1 != diameters.size()) std::cout << ", ";
		else std::cout << std::endl;
	}
	std::pair<int,int> longest = Palindrome::longestPalindromicSubstring(diameters);
	std::cout << "longest: ";
	const char *p = str.c_str() + longest.first;
	std::copy(p, p+longest.second, std::ostreambuf_iterator<char>(std::cout));
	std::cout << std::endl;
	}
	return 0;
}
