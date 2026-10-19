#ifndef XDG_SURFACE_HPP
#define XDG_SURFACE_HPP

#include <cstddef>
#include "w11-client.h"
#include "state.hpp"


void xdg_surface_configure(void*pool_data,struct xdg_surface *xdg_surface,uint32_t serial);
void xdg_wm_base_ping(void *pool_data,struct xdg_wm_base *xdg_wm_base,uint32_t serial);
extern const struct xdg_surface_listener xdg_surface_listener;
extern const struct xdg_wm_base_listener xdg_wm_base_listener;

#endif
