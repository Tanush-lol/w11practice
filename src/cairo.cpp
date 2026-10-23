#include <cairo/cairo.h>
#include <cstdint>
#include <cstring>

void drawCarioSquare(uint8_t *pool_data,int width,int height,int offset,int stride){
  // 
  
  uint32_t *pixels = (uint32_t *)&pool_data[offset];
  // memset(pixels, 0, width * height * 4);// for pattern 

  cairo_surface_t *surface = cairo_image_surface_create_for_data(reinterpret_cast<unsigned char *>(pixels)
      ,CAIRO_FORMAT_ARGB32,width,height,stride);
  

  cairo_t *cr = cairo_create(surface);//context is stored in cairo's central data type, called cairo_t
                                      //
    // Draw a background
    cairo_set_source_rgb(cr, 0.93, 0.93, 0.93);
    cairo_paint(cr);

    // Draw a black outlined square
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, 20, 20, 80, 80);
    cairo_stroke(cr);

    // Finish drawing before handing the buffer to Wayland
    cairo_destroy(cr);
    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);

}
