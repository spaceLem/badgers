// given a group x that has changed
// sets the list of groups that need to be updated

#pragma once

#include "defs.hpp"

void calc_nbhd(void);
void update_nbhd(bvec &update, int const x);
inline int get_nbhd(int const x, int const i) { return P.nbhd[4 * x + i]; }

