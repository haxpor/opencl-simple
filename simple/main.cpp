#include "openclsimple.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <string>

#define STR_BUFFER_LENGTH 2048

int main() {
	// list out all platforms and devices
	std::string platforms_devices_str(STR_BUFFER_LENGTH, '\0');
	clsimple_listall(platforms_devices_str.data(), platforms_devices_str.capacity(), false);
	std::cout << platforms_devices_str << std::endl;

	std::vector<int> arr_1(1000, 1);
	std::vector<int> arr_2(1000, 1);
	std::vector<int> arr_3(1000);

	int ret_code = clsimple_compute(arr_1.data(), arr_2.data(), arr_3.data(), arr_1.size());
	assert(ret_code == 0);

	for (size_t i=0; i<arr_3.size(); ++i) {
		assert(arr_3[i] == 2);
	}
	std::cout << "Passed" << std::endl;
	return 0;
}
