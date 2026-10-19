#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstddef>
#include "w11-client.h"
#include "state.hpp"

struct wl_buffer *draw_frame(struct our_state *state);

#endif 
