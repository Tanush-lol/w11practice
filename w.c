#include <stdio.h>
#include <wayland-client.h>
#include "wayland-protocol.h"

int main(){
  struct wl_display *display = wl_display_connect("wayland-1");
  if (!display) {
    fprintf(stderr, "we couldn't get the display on wayland server\n");
    return 1;
  }
  printf("connection established");
  while (wl_display_dispatch(display) != -1) {
    printf("Doing smthng");
  }

  int wl_display_get_fd(struct wl_display *display); // to get the file descriptor  

  wl_display_disconnect(display);
  return 0;
}
