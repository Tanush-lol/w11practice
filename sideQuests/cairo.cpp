#include <cairo/cairo.h>

int main(){
  cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,120,120);
  // 
  cairo_t *cr;

  cr = cairo_create(surface);//context is stored in cairo's central data type, called cairo_t
  cairo_set_line_width (cr, 0.1);
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
  cairo_stroke (cr);
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
  cairo_fill (cr);
  cairo_surface_write_to_png(surface, "output.png");
  return 0;
}
