#pragma once

#include <string>

namespace util {
	/**
	 * Convert via copying from std::string to C-string.
	 *
	 * # Arguments
	 * - str - input string
	 * - out - destination c-string pointer to copy the content of string to
	 * - len - length of string to copy from
	 */
	void str_to_cstr(const std::string& str, char* out, unsigned len);

	/**
	 * Convert via copying from std::string to UTF-16 string.
	 *
	 * # Arguments
	 * - str - input string
	 * - out - destination c-string pointer to copy the content of converted string
	 *		   of UTF-16 to
	 * - len - length of string to copy from
	 */
	void str_to_cstr_u16(const std::string& str, char* out, unsigned len);
};
