#include <iostream>
#include "w11-client.h"


int main(){

  const char *name = "wayland-0";
  const char *display_name = std::getenv("WAYLAND_DISPLAY");

  std::cout<<"socket name : "<<display_name<<'\n';

  struct wl_surface *surface;
  struct wl_display *display = wl_display_connect(display_name);
  // struct wl_display *wl_display_connect(const char *name);//is not a function call.it as a function declaration.
    if (!display) {
      std::cerr<<"Failed to connect to Wayland display."<<'\n';
        return 1;
    }else {  
    std::cerr<<"Connection established!"<<'\n';
    }

    int fd = wl_display_get_fd(display);
    std::cout<<"fd : "<<fd<<'\n';

    if (fd == -1) {
      std::cout<<"can't get fd for the process"<<'\n';
      return -1;
    }

    wl_registry *registry = wl_display_get_registry(display);// binded wl_display to wl_registry

    //below is the event loop
    // while (wl_display_dispatch(display) != -1) {
    //   /* This space deliberately left blank */
    // }


    wl_display_disconnect(display);

  return 0;
}
