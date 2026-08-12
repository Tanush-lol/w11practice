#include <stdio.h>
#include <wayland-client.h>
#include "xdg-shell-protocol.h"
#include <wayland-client-protocol.h>
#include <string.h>
#include <wayland-client-core.h>

static struct wl_compositor *compositor = NULL;
static struct wl_shm *shm = NULL;
static struct xdg_wm_base *wm_base = NULL;
struct xdg_surface *surface_xdg;
static struct wl_surface *surface;

static void sendPing(void *data, struct xdg_wm_base *base, uint32_t serial) {
  xdg_wm_base_pong(base, serial);
}

static struct xdg_wm_base_add_listener wm_base_listener = {
    .ping = sendPing,
};

static void registry_add(void *data, struct wl_registry *registry,
                         uint32_t name, const char *interface,
                         uint32_t version) {
  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 4);
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
  } else if (strcmp(interface, &xdg_wm_base_interface.name) == 0) {
    xdg_wm_base = wl_registry_bind(interface, &xdg_wm_base_interface, 1);
    xdg_wm_base_add_listener(wm_base, wm_base_listener, NULL);
  }
}

static void registry_remove(void *data, struct wl_registry *registry,
                            uint32_t name, const char *interface,
                            uint32_t version) {}

static void xdg_surface_configure(void *data, struct xdg_surface *surface) {
  xdg_surface_ack_configure(surface, serial);
}

static const struct xdg_surface_listener surface_listener = {
    .configure = xdg_surface_configure,

};

static const struct wl_registry_listener reglistener = {
    .global = registry_add, 
  .global_remove = registry_remove
};

int main() {
  struct wl_display *display;
  if (!(display = wl_display_connect(NULL))) {
    fprintf(stderr, "failed to connect to the wayland display server \n");
  }
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_display_add_listener(registry, &reglistener, NULL);

  wl_display_roundtrip(display);
  if (!compositor || !wm_base || !shm) {
    fprintf(stderr, "waytask failed to initialize\n");
    return 1;
  }

  struct wl_surface *surface = wl_compositor_create_surface(compositor);
  *surface_xdg = xdg_wm_base_get_xdg_surface(wm_base, surface);
  xdg_surface_add_listener(*surface_xdg, *listener, void *data)

      printf("Hello Wrld\n");
  return 0;
}
