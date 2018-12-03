// collection of useful functions

#pragma once

#include <iomanip>
#include "defs.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::setw;

template <typename T>
inline int ifloor(T x) {
	return static_cast<int>(floor(x));
}

template <typename T>
inline int iceil(T x) {
	return static_cast<int>(ceil(x));
}

/*
template <typename T>
inline T clamp(T x, T xmin, T xmax) {
	if (x < xmin)
		return xmin;
	else if (x > xmax)
		return xmax;
	else
		return x;
}
*/

// divide and set 0 if denom is 0
template <typename S, typename T>
double div0(S numer, T denom) {
	return denom == 0. ? 0. : static_cast<double>(numer) / denom;
}

// add two matrices of ints
inline void add(ivec &C, ivec const &A, ivec const &B) {
	for (int i = 0; i < A.size(); ++i) {
		C[i] = A[i] + B[i];
	}
}

template <typename T>
inline int fuzzyint(T x) {
	return static_cast<int>(floor(x) + gsl_ran_bernoulli(gen, fmod(x, 1.0)));
}

// vector sum
template <typename T>
T sum(vector<T> const &A) {
	T sumA = 0;
	// for (typename vector<T>::const_iterator it = A.begin(); it != A.end(); ++it)
	// 	sumA += *it;
	for (auto a : A)
		sumA += a;
	return sumA;
}

// vector cumsum
template <typename T>
T cumsum(vector<T> const &A, vector<T> &B) {
	T tmp = 0;
	for (int i = 0; i < A.size(); ++i) {
		tmp += A[i];
		B[i] = tmp;
	}
	return tmp;
}

// fix the width of the display functions
inline int getwidth(int t)	{ return 3; }
inline int getwidth(float t) { return 4; }
inline int getwidth(double t) { return 4; }
inline int getwidth(bool t) { return 2; }

// print the contents of a 1d vector
template <typename T>
void display(vector<T> const &A) {
	int x = getwidth((T) 0);
	cout.precision(2);

	for (auto a : A)
		cout << setw(x) << a;
	cout << endl;
}

// print the contents of a 2d vector with width n
template <typename T>
void display(vector<T> const &A, int const n) {
	int x = getwidth((T) 0);
	cout.precision(2);

	for (int i = 0; i < A.size(); ++i) {
		if (A[i])
			cout << setw(x) << A[i];
		else
			cout << setw(x) << ".";

		if (i % n == n - 1)
			cout << "\n";
	}
	cout << endl;
}

