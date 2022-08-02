#include <blend2d.h>


inline void blend2_context_setFillStyleColourPacked( BLContext *ctx, unsigned int rgba ) {
    ctx->setFillStyle(BLRgba32(rgba));
}

inline void blend2_context_setFillStyleColour( BLContext *ctx, int r, int g, int b, int a ) {
    ctx->setFillStyle(BLRgba32(r,g,b,a));
}


void blah() {
    BLImage img(480, 480, BL_FORMAT_PRGB32);

  // Attach a rendering context into `img`.
  BLContext ctx(img);

  // Clear the image.
  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.fillAll();

  // Fill some path.
  BLPath path;
  path.moveTo(26, 31);
  path.cubicTo(642, 132, 587, -136, 25, 464);
  path.cubicTo(882, 404, 144, 267, 27, 31);

  ctx.setCompOp(BL_COMP_OP_SRC_OVER);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
//  ctx.fillPath(path);
  //  ctx.fillPolygon()
  // Detach the rendering context from `img`.
  ctx.end();

  // Let's use some built-in codecs provided by Blend2D.
  BLImageCodec codec;
  codec.findByName("PNG");
  img.writeToFile("bl-getting-started-1.png", codec);

}