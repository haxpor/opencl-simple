#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <CL/cl2.hpp>

#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <numeric>
#include <cassert>
#include <cmath>

int main() {
    // Get the platform
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0];

    // Get the device
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    cl::Device device = devices[0];

    // Create the context
    cl::Context context(device);

    // Create the command queue
    cl::CommandQueue queue(context, device);

    // Create the kernel
    std::string kernelCode = "__kernel void add(__global int* a, __global int* b, __global int* c, int size) { "
                             "    int i = get_global_id(0);"
							 "	  if (i < size)"
                             "		c[i] = a[i] + b[i];"
                             "}";
    cl::Program::Sources sources;
    sources.push_back({kernelCode.c_str(), kernelCode.length()});
    cl::Program program(context, sources);
    if (auto ret_code = program.build({device});
	    ret_code != CL_SUCCESS) {
		std::cerr << "Error cl::Program::build, code=" << ret_code << std::endl;
		return -1;
	}
    cl::Kernel kernel(program, "add");

    // Create the input and output arrays
	const int SIZE = 10000000;
	std::vector<int> a(SIZE);
	std::vector<int> b(SIZE);
	std::vector<int> c(SIZE, 0);

	// prepare data
	std::iota(a.begin(), a.end(), 1);
	
	std::iota(b.begin(), b.end(), 1);
	std::reverse(b.begin(), b.end());

    // Create the buffer
    cl::Buffer bufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * a.size(), a.data());
    cl::Buffer bufferB(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * b.size(), b.data());
    cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, sizeof(int) * c.size());

	{
		cl::Event write_bufferA_event, write_bufferB_event;

		if (auto ret_code = queue.enqueueWriteBuffer(bufferA, CL_FALSE, 0, sizeof(int) * a.size(), a.data(), nullptr, &write_bufferA_event);
			ret_code != CL_SUCCESS) {
			std::cerr << "1 Error enqueueWriteBuffer() code=" << ret_code << std::endl;
			return -1;
		}
		if (auto ret_code = queue.enqueueWriteBuffer(bufferB, CL_FALSE, 0, sizeof(int) * b.size(), b.data(), nullptr, &write_bufferB_event);
			ret_code != CL_SUCCESS) {
			std::cerr << "2 Error enqueueWriteBuffer() code=" << ret_code << std::endl;
			return -1;
		}

		cl::Event::waitForEvents({write_bufferA_event, write_bufferB_event});
	}

    // Set the kernel arguments
    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferC);
	kernel.setArg(3, SIZE);

	auto start = std::chrono::steady_clock::now();

	// Execute the kernel
	if (auto ret_code = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(SIZE), cl::NullRange);
		ret_code != CL_SUCCESS) {
		std::cerr << "Error enqueueNDRangeKernel() code=" << ret_code << std::endl;
		return -1;
	}
	
	// Read the result
	if (auto ret_code = queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(int) * c.size(), c.data());
		ret_code != CL_SUCCESS) {
		std::cerr << "Error enqueueReadBuffer() code=" << ret_code << std::endl;
		return -1;
	}

	// check the result
	for (int i = 0; i < SIZE; i++) {
		assert(c[i] == SIZE + 1);
	}

	std::chrono::duration<double, std::milli> exec_time = std::chrono::steady_clock::now() - start;
	std::cout << "elapsed time: " << exec_time.count() << "ms" << std::endl;

    return 0;
}

