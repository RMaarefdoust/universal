#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <vector>
#include <fstream>
#include <cfloat>
//#include <bfloat>
//#include "https://github.com/RMaarefdoust/universal/blob/main/include/universal/number/cfloat/cfloat.hpp"
//#include <universal/number/posit/posit.hpp>
//#include <universal/number/bfloat/bfloat.hpp>
#include ".../universal/number/bfloat/bfloat.hpp"


// Heron's method - Original
template<typename Real>
Real heronSqrtOriginal(Real number, Real tolerance = 1e-7, int maxIterations = 1000) {
    if (number < 0) {
        std::cerr << "Error: Negative input to sqrt function." << std::endl;
        return -1;
    }

    // Initial guess
    Real guess = number * 0.5;
    Real difference, nextGuess;
    int iterations = 0;

    do {
        nextGuess = (guess + number / guess) * 0.5;
        difference = std::abs(nextGuess - guess);
        guess = nextGuess;
        iterations++;
    } while (difference > tolerance && iterations < maxIterations);

    return nextGuess;
}

// Heron's method - Improved
template<typename Real>
Real heronSqrtImproved(Real number, Real tolerance = 1e-7, int maxIterations = 1000) {
    if (number < 0) {
        std::cerr << "Error: Negative input to sqrt function." << std::endl;
        return -1;
    }

    if (number == 0) {
        return 0;
    }

    // Initial guess
    Real guess = number * 0.5;
    Real difference, nextGuess;
    int iterations = 0;

    do {
        Real guessSquared = guess * guess;
        Real a = (number - guessSquared) / (2.0 * guess);
        Real guessPlusA = guess + a;
        nextGuess = guessPlusA - (a * a) / (2.0 * guessPlusA);
        difference = std::abs(nextGuess - guess);
        guess = nextGuess;
        iterations++;
    } while (difference > tolerance * (nextGuess + guess) && iterations < maxIterations);

    return nextGuess;
}

int main() {
	using Real = float;
	
	//using Real = sw::universal::bfloat16;// #quad, quarter  ,bfloat16
   // using Posit = sw::universal::posit<16,2>; 
    //using Real = sw::universal::half; 
    //using Real = Posit;
    // Define your custom numbers here
    // Numbers between 0 and 1 for comparison
    std::vector<Real> numbers;
    for (int i = -100; i <= 100; ++i) {
        Real number = std::pow(10.0, i);
        if (number < 1.0)
            numbers.push_back(number);
    }

    std::vector<Real> resultsOriginal;
    std::vector<Real> resultsImproved;
    std::vector<Real> resultsSystem;

    std::cout << std::setw(15) << "Number"
              << std::setw(20) << "Original Result"
              << std::setw(20) << "Improved Result"
              << std::setw(20) << "System Result"
           //   << std::setw(20) << "Original Time"
            // << std::setw(20) << "Improved Time"
              << "\n";

    for (size_t i = 0; i < numbers.size(); i += 5) {
        Real number = numbers[i];

        // Measure time and result for the original method
        auto start = std::chrono::high_resolution_clock::now();
        Real sqrtResultOriginal = heronSqrtOriginal(number);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationOriginal = end - start;

        // Measure time and result for the improved method
        start = std::chrono::high_resolution_clock::now();
        Real sqrtResultImproved = heronSqrtImproved(number);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationImproved = end - start;

        // Measure time and result for the system method
        start = std::chrono::high_resolution_clock::now();
        Real sqrtResultSystem = std::sqrt(number);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationSystem = end - start;

        // Store results and times
        resultsOriginal.push_back(sqrtResultOriginal);
        resultsImproved.push_back(sqrtResultImproved);
        resultsSystem.push_back(sqrtResultSystem);

        // Print results in tabular format
        std::cout << std::setw(15) << number
                  << std::setw(20) << sqrtResultOriginal
                  << std::setw(20) << sqrtResultImproved
                  << std::setw(20) << sqrtResultSystem
                  << std::setw(20) << durationOriginal.count()
                  << std::setw(20) << durationImproved.count()
                  << "\n";
    }

    // Write results to CSV file
    std::ofstream outFile("comparison_results_heron.csv");
    outFile << "Number,Original Result,Improved Result,System Result\n";
    for (size_t i = 0; i < numbers.size(); ++i) {
        outFile << numbers[i] << ","
                << resultsOriginal[i] << ","
                << resultsImproved[i] << ","
                << resultsSystem[i] << "\n";
    }
    outFile.close();

    std::cout << "\nResults written to comparison_results_heron.csv\n";

    return 0;
}
