// performance.cpp : performance benchmarking for abitrary fixed-precision reals
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <iostream>
#include <string>
#include <chrono>
// configure the bfloat arithmetic class
#define AREAL_THROW_ARITHMETIC_EXCEPTION 0
#include <universal/number/bfloat/bfloat>
// is representable
#include <universal/functions/isrepresentable.hpp>
#include <universal/verification/test_status.hpp> // ReportTestResult
#include <universal/verification/performance_runner.hpp>

/*
   The goal of the arbitrary fixed-precision reals is to provide a constrained 
   linear floating-point type to explore the benefits of multi-precision algorithms.
*/

// measure performance of conversion operators
void TestConversionPerformance() {
	using namespace std;
	using namespace sw::universal;
	cout << endl << "BFLOAT Conversion operator performance" << endl;

	uint64_t NR_OPS = 1000000;
}

template<typename Scalar>
void DecodeWorkload(uint64_t NR_OPS) {
	using namespace std;
	using namespace sw::universal;

	Scalar a{ 0 };
	a.set_raw_bits(0xEEEEEEEEEEEEEEEEull);
	size_t success{ 0 };
	bool first{ true };
	for (uint64_t i = 0; i < NR_OPS; ++i) {
		bool s{ false };
		blockbinary<a.es, typename Scalar::BlockType> e;
		blockbinary<a.fbits, typename Scalar::BlockType> f;
		sw::universal::decode(a, s, e, f);
		if (s != f.at(0)) {
			++success;
		}
		else {
			// this shouldn't happen, but found a bug this way with bfloat<64,11,uint64_t> as type
			if (first) {
				first = false;
				cout << typeid(a).name() << " :\n" 
					<< to_binary(a,true) << "\n" 
					<< "sign    : " << (s ? "-1\n" : "+1\n") 
					<< "exponent: " << to_binary(e,true) << "\n" 
					<< "fraction: " << to_binary(f,true) << "\n";
			}
		}
	}
	if (success == 0) cout << "DECODE FAIL\n"; // just a quick double check that all went well
}

// measure performance of conversion operators
void TestDecodePerformance() {
	using namespace std;
	using namespace sw::universal;
	cout << endl << "BFLOAT decode operator performance" << endl;

	uint64_t NR_OPS = 1000000;
	PerformanceRunner("bfloat<8,2,uint8_t>      decode         ", DecodeWorkload< sw::universal::bfloat<8, 2, uint8_t> >, NR_OPS);
	PerformanceRunner("bfloat<16,5,uint16_t>    decode         ", DecodeWorkload< sw::universal::bfloat<16, 5, uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<32,8,uint32_t>    decode         ", DecodeWorkload< sw::universal::bfloat<32, 8, uint32_t> >, NR_OPS);
	PerformanceRunner("bfloat<64,11,uint64_t>   decode         ", DecodeWorkload< sw::universal::bfloat<64, 11, uint64_t> >, NR_OPS);
/* 
1/4/2021
AREAL decode operator performance: this is a decode that enumerates the bits, thus slowest possible algorithm
bfloat<8,2,uint8_t>      decode             1000000 per        0.012412sec ->  80 Mops/sec
bfloat<16,5,uint16_t>    decode             1000000 per       0.0287893sec ->  34 Mops/sec
bfloat<32,8,uint32_t>    decode             1000000 per       0.0649867sec ->  15 Mops/sec
bfloat<64,11,uint64_t>   decode             1000000 per        0.129481sec ->   7 Mops/sec

1/5/2021
AREAL decode operator performance: this is an exponent block move if there is no straddle
bfloat<8,2,uint8_t>      decode             1000000 per       0.0082558sec -> 121 Mops/sec
bfloat<16,5,uint16_t>    decode             1000000 per       0.0185946sec ->  53 Mops/sec
bfloat<32,8,uint32_t>    decode             1000000 per       0.0465827sec ->  21 Mops/sec
bfloat<64,11,uint64_t>   decode             1000000 per        0.104031sec ->   9 Mops/sec

AREAL decode operator performance: this is an exponent and fraction block move if there is no straddle
bfloat<8,2,uint8_t>      decode             1000000 per       0.0002446sec ->   4 Gops/sec
bfloat<16,5,uint16_t>    decode             1000000 per       0.0002446sec ->   4 Gops/sec
bfloat<32,8,uint32_t>    decode             1000000 per       0.0002675sec ->   4 Gops/sec
bfloat<64,11,uint64_t>   decode             1000000 per       0.0003521sec ->   4 Gops/sec
*/
}

// measure performance of arithmetic operators
void TestArithmeticOperatorPerformance() {
	using namespace std;
	using namespace sw::universal;
	cout << endl << "AREAL Arithmetic operator performance" << endl;

	uint64_t NR_OPS = 1000000;

	PerformanceRunner("bfloat<8,2,uint8_t>      add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<8,2,uint8_t> >, NR_OPS);
	PerformanceRunner("bfloat<16,5,uint16_t>    add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<16,5,uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<32,8,uint32_t>    add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<32,8,uint32_t> >, NR_OPS);
	PerformanceRunner("bfloat<64,11,uint64_t>   add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<64,11,uint64_t> >, NR_OPS);
//	PerformanceRunner("bfloat<128,15,uint64_t>  add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<128,15,uint64_t> >, NR_OPS / 2);
//	PerformanceRunner("bfloat<256,15,uint64_t   add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<256,15,uint64_t> >, NR_OPS / 4);
//	PerformanceRunner("bfloat<512,15,uint64_t>  add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<512,15,uint64_t> >, NR_OPS / 8);
//	PerformanceRunner("bfloat<1024,15,uint64_t> add/subtract   ", AdditionSubtractionWorkload< sw::universal::bfloat<1024,15,uint64_t> >, NR_OPS / 16);

	NR_OPS = 1024 * 32;
	PerformanceRunner("bfloat<8,2,uint16_t>     division       ", DivisionWorkload< sw::universal::bfloat<8,2,uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<16,5,uint16_t>    division       ", DivisionWorkload< sw::universal::bfloat<16,5,uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<32,8,uint32_t>    division       ", DivisionWorkload< sw::universal::bfloat<32,8,uint32_t> >, NR_OPS);
	PerformanceRunner("bfloat<64,11,uint64_t>   division       ", DivisionWorkload< sw::universal::bfloat<64,11,uint64_t> >, NR_OPS);
//	PerformanceRunner("bfloat<128,15,uint64_t>  division       ", DivisionWorkload< sw::universal::bfloat<128,15,uint64_t> >, NR_OPS / 2);
//	PerformanceRunner("bfloat<256,15,uint64_t   division       ", DivisionWorkload< sw::universal::bfloat<256,15,uint64_t> >, NR_OPS / 4);
//	PerformanceRunner("bfloat<512,15,uint64_t>  division       ", DivisionWorkload< sw::universal::bfloat<512,15,uint64_t> >, NR_OPS / 8);
//	PerformanceRunner("bfloat<1024,15,uint64_t> division       ", DivisionWorkload< sw::universal::bfloat<1024,15,uint64_t> >, NR_OPS / 16);

	// multiplication is the slowest operator

	NR_OPS = 1024 * 32;
	PerformanceRunner("bfloat<8,2,uint16_t>     multiplication ", MultiplicationWorkload< sw::universal::bfloat<8,2,uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<16,5,uint16_t>    multiplication ", MultiplicationWorkload< sw::universal::bfloat<16,5,uint16_t> >, NR_OPS);
	PerformanceRunner("bfloat<32,8,uint32_t>    multiplication ", MultiplicationWorkload< sw::universal::bfloat<32,8,uint32_t> >, NR_OPS);
	PerformanceRunner("bfloat<64,11,uint64_t>   multiplication ", MultiplicationWorkload< sw::universal::bfloat<64,11,uint64_t> >, NR_OPS);
//	PerformanceRunner("bfloat<128,15,uint64_t>  multiplication ", MultiplicationWorkload< sw::universal::bfloat<128,15,uint64_t> >, NR_OPS / 2);
//	PerformanceRunner("bfloat<256,15,uint64_t   multiplication ", MultiplicationWorkload< sw::universal::bfloat<256,15,uint64_t> >, NR_OPS / 4);
//	PerformanceRunner("bfloat<512,15,uint64_t>  multiplication ", MultiplicationWorkload< sw::universal::bfloat<512,15,uint64_t> >, NR_OPS / 8);
//	PerformanceRunner("bfloat<1024,15,uint64_t> multiplication ", MultiplicationWorkload< sw::universal::bfloat<1024,15,uint64_t> >, NR_OPS / 16);

}

// conditional compilation
#define MANUAL_TESTING 0
#define STRESS_TESTING 0

int main()
try {
	using namespace std;
	using namespace sw::universal;

	std::string tag = "AREAL operator performance benchmarking";

#if MANUAL_TESTING

	using Scalar = bfloat<64, 11, uint64_t>;
	Scalar a;
	a.set_raw_bits(0xEEEEEEEEEEEEEEEEull);
	bool s{ false };
	blockbinary<a.es, typename Scalar::BlockType> e;
	blockbinary<a.fbits, typename Scalar::BlockType> f;
	sw::universal::decode(a, s, e, f);
	cout << typeid(a).name() << " :\n"
		<< to_binary(a, true) << "\n"
		<< "sign    : " << (s ? "-1\n" : "+1\n")
		<< "exponent: " << to_binary(e, true) << "\n"
		<< "fraction: " << to_binary(f, true) << "\n";

	cout << "nbits: " << a.nbits << endl;
	cout << "es   : " << a.es << endl;
	cout << "fbits: " << a.fbits << endl;

	cout << "done" << endl;

	return EXIT_SUCCESS;
#else
	std::cout << tag << std::endl;

	int nrOfFailedTestCases = 0;
	   
	TestDecodePerformance();
	TestArithmeticOperatorPerformance();

#if STRESS_TESTING

#endif // STRESS_TESTING
	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);

#endif // MANUAL_TESTING
}
catch (char const* msg) {
	std::cerr << msg << '\n';
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << '\n';
	return EXIT_FAILURE;
}

/*
ETLO
Date run : 2/23/2020
Processor: Intel Core i7-7500 CPU @ 2.70GHz, 2 cores, 4 threads, 15W mobile processor
Memory   : 16GB
System   : 64-bit Windows 10 Pro, Version 1803, x64-based processor, OS build 17134.165

*/