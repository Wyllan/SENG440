#include <libattopng.h>
#include <inttypes.h>
#include <stdio.h>


#define W 255
#define H 255

#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r, g, b) RGBA(r, g, b, 0xff)

#define ALPHA(c, a) ((c) | ((a) << 8))


int main(int argc, char *argv[]) {
    libattopng_t *png = libattopng_new(W, H, PNG_PALETTE);
    uint32_t palette[] = {
            RGBA(0, 0, 0xff, 0xff),
            RGBA(0, 0xff, 0, 0x80),
            RGBA(0xff, 0, 0, 0xff),
            RGBA(0xff, 0, 0xff, 0x80)
    };
    libattopng_set_palette(png, palette, 4);

    int x, y;
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, (x % 16) / 4);
        }
    }

    libattopng_save(png, "test_palette.png");
    libattopng_destroy(png);

    // -----------------

    png = libattopng_new(W, H, PNG_RGBA);

    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, RGBA(x & 255, y & 255, 128, (255 - ((x / 2) & 255))));
        }
    }
    libattopng_save(png, "test_rgba.png");
    libattopng_destroy(png);

    // -----------------

    png = libattopng_new(W, H, PNG_RGB);

    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, RGB(x & 255, y & 255, 128));
	    printf("%" PRIx32 ",", RGB(x & 255, y & 255, 128) );
        }
	printf("\n");
    }
    libattopng_save(png, "test_rgb.png");
    libattopng_destroy(png);

    return 0;
}
