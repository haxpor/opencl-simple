#include "openclsimple.h"

#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <iostream>
#include <CL/cl2.hpp>
#include <vector>

#ifdef ENABLE_DEBUG
#include <cstdarg>
#endif

#ifdef ENABLE_DEBUG
const int LOG_BUFFER_SIZE = 2048;
char log_buffer[LOG_BUFFER_SIZE];

inline void DLOG(const char* ctx, const char* format, ...) {
	va_list args;
	va_start(args, format);
	std::vsnprintf(log_buffer, LOG_BUFFER_SIZE-1, format, args);
	va_end(args);

	std::cerr << "[DEBUG] [" << ctx << "] " << log_buffer << std::endl;
}
#else
	#define DLOG(...)
#endif

CLSIMPLE_API void clsimple_listall() noexcept {
	// Get the platform
	std::vector<cl::Platform> platforms;
	int ret_code = cl::Platform::get(&platforms);
	if (ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::Platform::get(), code=" << ret_code << std::endl;
		return;
	}

	std::string tmp_str;
	cl_device_type tmp_device_type;

	for (auto& p : platforms) {
		ret_code = p.getInfo(CL_PLATFORM_NAME, &tmp_str);
		if (ret_code != CL_SUCCESS)
			std::cerr << "Error cl::Platform::getInfo(), code=" << ret_code << std::endl;
		else
			std::cout << "Platform: " << tmp_str << std::endl;
		
		ret_code = p.getInfo(CL_PLATFORM_VENDOR, &tmp_str);
		if (ret_code != CL_SUCCESS)
			std::cerr << "Error cl::Platform::getInfo(), code=" << ret_code << std::endl;
		else
			std::cout << "Vendor: " << tmp_str << std::endl;

		std::vector<cl::Device> devices;
		ret_code = p.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		if (ret_code != CL_SUCCESS) {
			std::cerr << "Error cl::Platform::getDevices(), code=" << ret_code << std::endl;
			continue;
		}

		for (const auto& d : devices) {
			ret_code = d.getInfo(CL_DEVICE_NAME, &tmp_str);
			if (ret_code != CL_SUCCESS)
				std::cerr << "Error cl::Device::getInfo(), code=" << ret_code << std::endl;
			else
				std::cout << " -Device name: " << tmp_str << std::endl;

			ret_code = d.getInfo(CL_DEVICE_TYPE, &tmp_device_type);
			if (ret_code != CL_SUCCESS)
				std::cerr << "Error cl::Device::getInfo(), code=" << ret_code << std::endl;
			else {
				if (tmp_device_type & CL_DEVICE_TYPE_GPU)
					std::cout << " -Type: GPU" << std::endl;
				else if (tmp_device_type & CL_DEVICE_TYPE_CPU)
					std::cout << " -Type: CPU" << std::endl;
				else if (tmp_device_type & CL_DEVICE_TYPE_ACCELERATOR)
					std::cout << " -Type: Accelerator" << std::endl;
				else
					std::cout << " -Type: Unknown" << std::endl;
			}
		}
	}
}

CLSIMPLE_API int clsimple_compute(const int arr_1[], const int arr_2[], int arr_3[], int num_elem) noexcept {
	// TODO: extract this to outside common
	cl_int ret_code = CL_SUCCESS;

	// Get the platform
	std::vector<cl::Platform> platforms;
	ret_code = cl::Platform::get(&platforms);
	if (ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::Platform::get(), code=" << ret_code << std::endl;
		return ret_code;
	}
	DLOG(__FUNCTION__, "found %d platform(s)", platforms.size());
	cl::Platform platform = platforms[0];

	DLOG(__FUNCTION__, "%s", "passed getting platforms");

	// Get the device
	std::vector<cl::Device> devices;
	ret_code = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	if (ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::Platform::getDevices(), code=" << ret_code << std::endl;
		return ret_code;
	}
	DLOG(__FUNCTION__, "found %d GPU device(s)", devices.size());

	cl::Device device = devices[0];

	DLOG(__FUNCTION__, "%s", "passed getting a GPU device");

	// Create the context
	cl::Context context(device);

	DLOG(__FUNCTION__, "%s", "passed creating a context");

	// Create the command queue
	cl::CommandQueue queue(context, device);

	DLOG(__FUNCTION__, "%s", "passed creating command queue");

	// Create the kernel
	std::string kernelCode = "__kernel void add(__global int* a, __global int* b, __global int* c, int size) { "
							 "		int i = get_global_id(0);"
							 "		if (i < size)"
							 "			c[i] = a[i] + b[i];"
							 "}";
	cl::Program::Sources sources;
	sources.push_back({kernelCode.c_str(), kernelCode.length()});
	cl::Program program(context, sources);
	ret_code = program.build({device});
	if (ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::Program::build(), code=" << ret_code << std::endl;
		return ret_code;
	}

	DLOG(__FUNCTION__, "%s", "passed building a kernel program");

	// TODO: pre-create more kernel like this to save time in creating context, kernel, etc
	cl::Kernel kernel(program, "add");

	DLOG(__FUNCTION__, "%s", "passed adding kernel function");

	// Create buffers
	cl::Buffer buffer_a(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * num_elem, const_cast<int*>(arr_1));
	cl::Buffer buffer_b(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * num_elem, const_cast<int*>(arr_2));
	cl::Buffer buffer_c(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * num_elem, arr_3);

	kernel.setArg(0, buffer_a);
	kernel.setArg(1, buffer_b);
	kernel.setArg(2, buffer_c);
	kernel.setArg(3, num_elem);

	DLOG(__FUNCTION__, "%s", "passed setting all arguments");
	

	// transfer data from host to device
	// NOTE: in testing, I don't need to call these functions
	cl::Event event_buffer_a_write;
	cl::Event event_buffer_b_write;

	ret_code = queue.enqueueWriteBuffer(buffer_a, CL_FALSE, 0, sizeof(int) * num_elem, arr_1, nullptr, &event_buffer_a_write);
	ret_code = queue.enqueueWriteBuffer(buffer_b, CL_FALSE, 0, sizeof(int) * num_elem, arr_2, nullptr, &event_buffer_b_write);

	std::vector<cl::Event> to_wait_events = {event_buffer_a_write, event_buffer_b_write};
	cl::Event::waitForEvents(to_wait_events);

	// execute the kernel function
	// NOTE: this is a blocking call although enqueuing is async call but the current thread
	// will be blocked until he work is done. Work is done doesn't mean that the result buffer
	// will be written back at the same time.
	//
	// TODO: local size should be queried via opencl api
	ret_code = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(num_elem), cl::NullRange);
	if (ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::CommandQueue::enqueueNDRangeKernel(), code=" << ret_code << std::endl;
		return ret_code;
	}

	// CL_TRUE to make it blocking call
	// it requires for moving data from device back to host
	// NOTE: Important to call this function to make sure the result is sent back to host.
	ret_code = queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, sizeof(int) * num_elem, arr_3);
	if (ret_code != CL_SUCCESS)
		std::cerr << "Error cl::CommandQueue::enqueueReadBuffer(), code=" << ret_code << std::endl;

	return ret_code;
}