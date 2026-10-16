#define _POSIX_C_SOURCE 200112L
#include <iostream>
#include "w11-client.h"
#include <string.h>
#include <shm.hpp>
#include <sys/mman.h>

struct our_state{
  struct wl_compositor *compositor;
  struct wl_shm *shm;
};

static void handle_reg_global(void *data,struct wl_registry *registry,
                              uint32_t name,const char *interface,uint32_t version)
{
  struct our_state *state = static_cast<our_state *>(data);//in paranthesis
                                                           
  if(strcmp(interface,wl_compositor_interface.name) == 0){
    std::cout<<"interface :"<<interface<<" | "<<"version :"<<version<<" | "<<"name :"<<name<<'\n';
    state->compositor = static_cast<wl_compositor *>(wl_registry_bind(registry,name, &wl_compositor_interface,4));
 }
    if (strcmp(interface, wl_shm_interface.name) == 0) {
    std::cout<<"interface :"<<interface<<" | "<<"version :"<<version<<" | "<<"name :"<<name<<'\n';
        state->shm = static_cast<wl_shm *>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
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

  const int width = 1920, height = 1080;
  const int stride = width * 4;
  const int shm_pool_size = height * stride * 2;


  int fd = allocate_shm_file(shm_pool_size);
  uint8_t *pool_data = static_cast<uint8_t *>(mmap(NULL, shm_pool_size,
      PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

  const char *display_name = std::getenv("WAYLAND_DISPLAY");
  struct wl_display *display = wl_display_connect(display_name);
  wl_registry *registry = wl_display_get_registry(display); 

  wl_registry_add_listener(registry, &registry_listener, &state);
	wl_display_roundtrip(display);
  struct wl_surface *surface = wl_compositor_create_surface(state.compositor);



  wl_registry_destroy(registry);
  wl_display_disconnect(display);
	return 0;
}
