﻿// exp_div_factorial.cpp: 
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the UNIVERSAL project, which is released under an MIT Open Source license.
#include "common.hpp"
// pull in the posit number system
#include <universal/number/posit/posit.hpp>

int main(int argc, char** argv)
try {
	using namespace sw::universal;

	//constexpr size_t nbits = 32;
	//constexpr size_t es = 2;
	//using Posit = posit<nbits,es>;

	// print detailed bit-level computational intermediate results
	// bool verbose = false;

	// preserve the existing ostream precision
	auto precision = std::cout.precision();
	std::cout << std::setprecision(12);



	// restore the previous ostream precision
	std::cout << std::setprecision(precision);

	return EXIT_SUCCESS;
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_arithmetic_exception& err) {
	std::cerr << "Uncaught posit arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::quire_exception& err) {
	std::cerr << "Uncaught quire exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_internal_exception& err) {
	std::cerr << "Uncaught posit internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (std::runtime_error& err) {
	std::cerr << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
