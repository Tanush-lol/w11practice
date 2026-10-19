#ifndef STATE_HPP
#define STATE_HPP

struct our_state{
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *shm;
    struct wl_compositor *compositor;
    struct xdg_wm_base *wm_base;
    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

#endif
