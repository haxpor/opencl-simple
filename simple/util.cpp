#include "util.h"

#include <cuchar>
#include <locale>
#include <codecvt>
#include <cstring>

namespace util {
	/* converter of byte character to UTF-16 (2 bytes) */
	std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> ch_converter;

	void str_to_cstr(const std::string& str, char* out, unsigned len) {
		const char* str_cstr = str.c_str();
		const size_t capped_len = strlen(str_cstr) <= (len-1) ? strlen(str_cstr) : (len-1);
		std::memcpy(out, str_cstr, capped_len+1);
	}

	void str_to_cstr_u16(const std::string& str, char* out, unsigned len) {
		const char* str_cstr = str.c_str();

		std::u16string u16_str = ch_converter.from_bytes(str);
		const char16_t* u16_str_cstr = u16_str.c_str();
		
		const size_t capped_len = strlen(str_cstr) <= (len-1) ? strlen(str_cstr) : (len-1);
		std::memcpy(out, u16_str_cstr, capped_len*2+1);
	}
};
