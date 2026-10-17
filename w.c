#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wayland-client.h>
#include "wayland-protocol.h"

struct our_state {
  struct wl_compositor *compositor;
  struct wl_surface  *surface;
  struct wl_shm *shm;
  struct wl_shm_pool *pool;
  struct wl_buffer *buffer;
};

const int width = 1920;
const int height = 1080;
const int stride = width * 4;
const int shm_pool_size = height * stride * 2;
const int buffer_index = 0;
const int offset = height * stride * buffer_index;

uint32_t *pixels = (uint32_t *)&pool_data[offset];
memset(pixels, 0, width * height * 4);


static void
randname(char *buf)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) {
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}

static int
create_shm_file(void)
{
	int retries = 100;
	do {
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);
		--retries;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			shm_unlink(name);
			return fd;
		}
	} while (retries > 0 && errno == EEXIST);
	return -1;
}

int
allocate_shm_file(size_t size)
{
	int fd = create_shm_file();
	if (fd < 0)
		return -1;
	int ret;
	do {
		ret = ftruncate(fd, size);
	} while (ret < 0 && errno == EINTR);
	if (ret < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

static void registry_handle_global(void *data, struct wl_registry *registry,uint32_t name, const char *interface, uint32_t version){
  struct our_state *state = data;
  if(strcmp(interface,wl_compositor_interface.name) == 0 ){
    struct wl_surface *surface = wl_compositor_create_surface(state -> compositor =
                                wl_registry_bind(registry,name,&wl_compositor_interface,4));
    state->surface = surface;
    fprintf(stderr,"yo we got the surface\n");
  }

  if (strcmp(interface,wl_shm_interface.name) == 0) {
      int fd = allocate_shm_file(shm_pool_size);
    if (fd < 0) {
      fprintf(stderr, "failed to allocate shm file\n");
      return;
    }

  struct wl_shm_pool *pool = wl_shm_create_pool(state -> shm = 
                             wl_registry_bind(registry,name,&wl_shm_interface,1), fd, shm_pool_size);
  struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset,
    width, height, stride, WL_SHM_FORMAT_XRGB8888);
  }
}

static void registry_handle_global_remove(void *data, struct wl_registry *registry,uint32_t name){

}

static const struct wl_registry_listener
registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};



int main(){
  struct wl_display *display = wl_display_connect("wayland-1");
  if (!display) {
    fprintf(stderr, "we couldn't get the display on wayland server\n");
    return 1;
  }
  struct our_state state = { 0 };
  printf("connection established");

  // while (wl_display_dispatch(display) != -1) {
  //   printf("Doing smthng\n");
  // } //idk what does this do but they say for making the program alive untill the compositor returns -1 i.e close the program

  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry,&registry_listener,&state); //we add listener to the state i.e made by us 
  
  wl_display_roundtrip(display);

  // int wl_display_get_fd(struct wl_display *display); // to get the file descriptor  
  
  wl_display_disconnect(display);
  return 0;
}
