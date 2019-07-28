#include <libattopng.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

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
    png = libattopng_new(W, H, PNG_RGB);

    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            libattopng_set_pixel(png, x, y, RGB(x & 255, y & 255, 128));
//printf("%" PRIx32 ",", RGB(x & 255, y & 255, 128) );
        }
//	printf("\n");
    }
    libattopng_save(png, "test_rgb.png");
    //uint32_t rgbhex;
    uint32_t rgb[H][W];
    //char rgb[H][W][3];
        //FILE *test = fopen("rgblist.txt", "w");
	  //  if (test == NULL){
	//	                exit(1);
	//			    }
        for (y = 0; y < H; y++) {
		for (x = 0; x < W; x++){
			//fwrite(&p, PRIx32, 1, libattopng_get_pixel(png, x, y));
			//rgbhex = libattopng_get_pixel(png, x, y);
			rgb[y][x] =  libattopng_get_pixel(png, x, y);
			//rgb[y][x][0] = rgbhex >> 16;
			//rgb[y][x][1] = rgbhex >> 8;
			//rgb[y][x][2] = rgbhex;
			//fwrite(&rgb[y][x], sizeof(rgb[y][x]), 1, test);
			//printf("%d,", rgbhex >> 16);
			printf("%" PRIx32, (rgb[y][x] >> 16)& 0xFF );
//	printf(",%02x", (char)((rgb[y][x] >> 16)));
			if (x != (W-1)) {
				printf(",");
			}
		}
		printf("\n");
	}
    libattopng_destroy(png);
    /*FILE *test = fopen("rgblist.txt", "w");
    if (test == NULL){
	    exit(1);
    }
    fwrite(rgb, sizeof(uint32_t), 2, test);*/
    //fclose(test);


    return 0;
}
