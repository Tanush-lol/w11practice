wayland-project/
├── include/
│   ├── client.h
│   ├── compositor.h
│   ├── surface.h
│   ├── buffer.h
│   ├── protocol.h
│   └── types.h

g++ -Iinclude src/main.cpp -o app -lwayland-client
