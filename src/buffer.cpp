#define _POSIX_C_SOURCE 200112L
#include "xdg-shell-client-protocol.h"
#include <iostream>
#include "w11-client.h"
#include <string.h>
#include "shm.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include "state.hpp"
#include "cario.hpp"

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

struct wl_buffer *draw_frame(struct our_state *state){
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

  // uint32_t *pixels = (uint32_t *)&pool_data[offset];
  // memset(pixels, 0, width * height * 4);// for pattern 
  //   /* Draw checkerboxed background */
  //   for (int y = 0; y < height; ++y) {
  //       for (int x = 0; x < width; ++x) {
  //           if ((x + ((y / 8 )* 8)) % 16 < 8)
  //               pixels[y * width + x] = 0xFF666666;
  //           else
  //               pixels[y * width + x] = 0xFFEEEEEE;
  //       }
  //   }
    drawCarioSquare(pool_data,width,height,offset,stride);
    munmap(pool_data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}
