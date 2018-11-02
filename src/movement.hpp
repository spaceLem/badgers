// calculate migration matrix, containing the migration rate for each group
// migration is weighted towards emptier groups

#pragma once

#include "defs.hpp"
#include "nbhd.hpp"
#include "utils.hpp"

void movement(ivec const &N, Rates &rates, bvec const &update);

