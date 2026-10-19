#define _POSIX_C_SOURCE 200112L
#include "xdg-shell-client-protocol.h"
#include <iostream>
#include "w11-client.h"
#include <string.h>
#include <shm.hpp>
#include <sys/mman.h>
#include <unistd.h>

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

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

static struct wl_buffer *draw_frame(struct our_state *state){
  const int width = 1920, height = 1080;
  const int stride = width * 4;
  int size = stride * height;

  const int shm_pool_size = height * stride * 2;

  int fd = allocate_shm_file(shm_pool_size);
  if(fd == -1){
    std::cout<<"err in allocate_shm_file()"<<'\n';
    return nullptr;// why?
  }
  uint8_t *pool_data = static_cast<uint8_t *>(mmap(NULL,shm_pool_size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0));
  if(pool_data == MAP_FAILED){
    std::cout<<"mmap() failed"<<'\n';
    close(fd);
    return NULL;
  }
  // uint8_t *pool_data = static_cast<uint8_t *>(mmap(NULL, shm_pool_size,3, MAP_SHARED, fd, 0)); // also valid 
  
  int index = 0;
  int offset = height * stride * index;

  struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, shm_pool_size);
  struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset,width, height, stride, WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);
    close(fd);

  uint32_t *pixels = (uint32_t *)&pool_data[offset];
  memset(pixels, 0, width * height * 4);// for pattern 

    munmap(pool_data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void xdg_surface_configure(void*pool_data,struct xdg_surface *xdg_surface,uint32_t serial){
  struct our_state *state = static_cast<our_state *>(pool_data);
  xdg_surface_ack_configure(xdg_surface,serial);

  struct wl_buffer *buffer = draw_frame(state);
  wl_surface_attach(state->wl_surface, buffer, 0, 0);
  // wl_surface_damage(state->wl_surface, 0, 0, UINT32_MAX, INT32_MAX);
  wl_surface_commit(state->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void xdg_wm_base_ping(void *pool_data,struct xdg_wm_base *xdg_wm_base,uint32_t serial){
  xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener= {
  .ping = xdg_wm_base_ping,
};

static void handle_reg_global(void *data,struct wl_registry *registry,
                              uint32_t name,const char *interface,uint32_t version)
{
  struct our_state *state = static_cast<our_state *>(data);//in paranthesis
                                                           
  if (strcmp(interface, wl_shm_interface.name) == 0) {
    std::cout<<"interface :"<<interface<<" | "<<"version :"<<version<<" | "<<"name :"<<name<<'\n';
        state->shm = static_cast<wl_shm *>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
    }

  if(strcmp(interface,wl_compositor_interface.name) == 0){
    std::cout<<"interface :"<<interface<<" | "<<"version :"<<version<<" | "<<"name :"<<name<<'\n';
    state->compositor = static_cast<wl_compositor *>(wl_registry_bind(registry,name, &wl_compositor_interface,4));
 }

    if (strcmp(interface,xdg_wm_base_interface.name)== 0) {
      std::cout<<"Binding to wm_base"<<'\n';

      uint32_t bind_version = std::min(version,static_cast<uint32_t>(xdg_wm_base_interface.version));
      state->wm_base = static_cast<xdg_wm_base *>(wl_registry_bind(registry,name,&xdg_wm_base_interface,1));
        xdg_wm_base_add_listener(state->wm_base,&xdg_wm_base_listener, state);
    }
}//handles flashing of available interfaces,which we can bind to

static void handle_reg_global_remove(void *data,struct wl_registry *registry,
                              uint32_t name)
{

}

static const struct wl_registry_listener registry_listener = {
  .global = handle_reg_global,
  .global_remove = handle_reg_global_remove,
};

int main(){
  struct our_state state = {0};


  const char *display_name = std::getenv("WAYLAND_DISPLAY");
  state.wl_display = wl_display_connect(display_name);
  wl_registry *registry = wl_display_get_registry(state.wl_display); 

  wl_registry_add_listener(registry, &registry_listener, &state);
	wl_display_roundtrip(state.wl_display);

  state.wl_surface = wl_compositor_create_surface(state.compositor);
  state.xdg_surface = xdg_wm_base_get_xdg_surface(state.wm_base,state.wl_surface);

  xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
  state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
  xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
  wl_surface_commit(state.wl_surface);


    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }

  // wl_registry_destroy(registry);
  // wl_display_disconnect(display);
	return 0;
}
