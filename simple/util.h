#pragma once

#include <string>

namespace util {
	/**
	 * Convert via copying from std::string to C-string.
	 */
	void str_to_cstr(const std::string& str, char* out, unsigned len);

	/**
	 * Convert via copying from std::string to UTF-16 string.
	 */
	void str_to_cstr_u16(const std::string& str, char* out, unsigned len);
};
