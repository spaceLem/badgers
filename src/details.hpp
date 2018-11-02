#pragma once

#include "defs.hpp"
#include "utils.hpp"

void show_time(double const t);

void show_progress(double const t, int const itn);

void show_finished(void);

void show_recording(int const current_record, Rates const &rates);

void describe_event(int const last_event, int const event_info, int const x, int const y);

void describe_situation(ivec const &S, ivec const &I, ivec const &N,
		Rates const &rates, bvec const &update, bool const failure);

