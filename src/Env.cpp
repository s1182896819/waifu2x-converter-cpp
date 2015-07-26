#include "Env.hpp"
#include "Buffer.hpp"

#ifdef __GNUC__
#include <cpuid.h>
#else
#include <intrin.h>
#endif

ComputeEnv::ComputeEnv()
    :num_cl_dev(0),
     num_cuda_dev(0),
     cl_dev_list(nullptr),
     cuda_dev_list(nullptr),
     transfer_wait(0)
{
	bool have_fma = false, have_avx = false;
	unsigned int eax=0, ebx=0, ecx=0, edx=0;

#ifdef __GNUC__
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
#else
	int cpuInfo[4];
	__cpuid(cpuInfo, 1);
	eax = cpuInfo[0];
	ebx = cpuInfo[1];
	ecx = cpuInfo[2];
	edx = cpuInfo[3];
#endif
	this->flags = 0;

	if ((ecx & 0x18000000) == 0x18000000) {
		this->flags |= HAVE_AVX;
	}

	if (ecx & (1<<12)) {
		this->flags |= HAVE_FMA;
	}

	this->pref_block_size = 4096;
}
