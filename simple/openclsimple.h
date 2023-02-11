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
 * We didn't define CL_HPP_ENABLE_EXCEPTIONS thus there would be no exceptions thorwn
 * from any OpenCL related API.
 */
extern "C" {
	/**
	 * List all platforms, and devices available.
	 * This will print onto standard output (stdout).
	 */
	CLSIMPLE_API void clsimple_listall() noexcept;

	/**
	 * Compute a summation of two input arrays then output into 3rd array limiting
	 * by the number of elements specified.
	 */
	CLSIMPLE_API [[nodiscard]] int clsimple_compute(const int arr_1[], const int arr_2[], int arr_3[], int num_elem) noexcept;
};
