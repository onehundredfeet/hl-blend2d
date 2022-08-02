package tests;

import blend2d.Native;

class Test {
    public static function main() {
        var i = new Image( 256, 256, BLFormat.BL_FORMAT_PRGB32);

        var ctx = new Context(i);

        ctx.setCompOp(BL_COMP_OP_SRC_COPY);
        ctx.fillAll();

        // Fill some path.
        //BLPath path;
        //path.moveTo(26, 31);
        //path.cubicTo(642, 132, 587, -136, 25, 464);
        //path.cubicTo(882, 404, 144, 267, 27, 31);

        ctx.setCompOp(BL_COMP_OP_SRC_OVER);
        ctx.setFillStyleColour(0xFFFFFFFF);
        //  ctx.fillPath(path);
        //  ctx.fillPolygon()
        // Detach the rendering context from `img`.
        ctx.end();

    }
}