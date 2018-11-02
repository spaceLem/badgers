// calculate infection matrix, containing the infection rate for each group

#pragma once

#include "defs.hpp"
#include "utils.hpp"

void infect(ivec const &S, ivec const &I, Rates &rates, bvec const &update);

