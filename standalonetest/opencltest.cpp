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
    std::string kernelCode = "__kernel void add(__global int* a, __global int* b, __global int* c) { "
                             "    int i = get_global_id(0); "
                             "    c[i] = a[i] + b[i]; "
                             "}";
    cl::Program::Sources sources;
    sources.push_back({kernelCode.c_str(), kernelCode.length()});
    cl::Program program(context, sources);
    program.build({device});
    cl::Kernel kernel(program, "add");

    // Create the input and output arrays
	std::vector<int> a(10000000);
	std::vector<int> b(10000000);
	std::vector<int> c(10000000, 0);

	std::iota(a.begin(), a.end(), 1);
	
	std::iota(b.begin(), b.end(), 1);
	std::reverse(b.begin(), b.end());

    // Create the buffer
    cl::Buffer bufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * a.size(), a.data());
    cl::Buffer bufferB(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * b.size(), b.data());
    cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, sizeof(int) * c.size());

    // Set the kernel arguments
    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferC);

	auto start = std::chrono::steady_clock::now();

	// Execute the kernel
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(10000000), cl::NullRange);
	
	// Read the result
	queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(int) * c.size(), c.data());

	// check the result
	for (int i = 0; i < 10000000; i++)
		assert(c[i] == 10000000 + 1);

	std::chrono::duration<double, std::milli> exec_time = std::chrono::steady_clock::now() - start;
	std::cout << "elapsed time: " << exec_time.count() << "ms" << std::endl;

    return 0;
}

