#pragma once

#ifdef WINDOWS
	#ifdef CLSIMPLE_API_EXPORT
		#define CLSIMPLE_API __declspec(dllexport)
	#else
		#define CLSIMPLE_API __declspec(dllimport)
	#endif
#else
	#define CLSIMPLE_API
#endif

/**
 * We didn't define CL_HPP_ENABLE_EXCEPTIONS thus there would be no exceptions thrown
 * from any OpenCL related API.
 */
extern "C" {
	/**
	 * List all platforms, and devices available.
	 * If there any error occurs during the operation of this function, it will
	 * print error onto standard error. The resultant text output is still maintained
	 * separately.
	 *
	 * # Arguments
	 * - out - output c-string to be filled
	 * - len - length of output c-string to be filled
	 * - utf16 - whether or not to convert string to UTF-16 encoding. Default is true.
	 *			 If used on MetaTrader 5, this flag should be set to true.
	 */
	CLSIMPLE_API void clsimple_listall(char* out, int len, bool utf16=true) noexcept;

	/**
	 * Compute a summation of two input arrays then output into 3rd array limiting
	 * by the number of elements specified.
	 *
	 * # Arguments
	 * - arr_1 - first read-only array input holding integers
	 * - arr_2 - second read-only array input holding integers
	 * - arr_3 - output integer array to be filled with result of summation of both arr_1 and arr_2
	 * - num_elem - number of element to be processed for both arr_1 and arr_2
	 *
	 * # Return
	 * Returned code for result of operation. 0 means success, otherwise means failure.
	 */
	CLSIMPLE_API [[nodiscard]] int clsimple_compute(const int arr_1[], const int arr_2[], int arr_3[], int num_elem) noexcept;
};
