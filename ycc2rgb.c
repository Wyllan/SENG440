#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <libattopng.h>

#define W 255
#define H 255
#define BGR 3
#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r, g, b) RGBA(r, g, b, 0xff)

// Takes in a value, a lower bound, and an upper bound
// Saturation arithmetic (clamping)
int check_range(int val, int lb, int ub) {
	// rgb - [0,255]
	// y   - [16,235]
	// cr  - [16,240]
	// cb  - [16,240]
	// printf("range: %d ", val);
	if(val < lb) { return lb; }
	else if (val > ub) { return ub; }
	else { return val; }
}

int rounding(int x) {
	// if (x % 1000 > 499)
	// 	return x;
	// else 
		return x;
}

void YCCtoRGB(int ycc[W][H][BGR], int rgb[W][H][BGR]) {

	int i, j;
	int y, cr, cb;
	for(i = 0; i < W; i++) {
		for(j = 0; j < H; j++) {
			if( i == 0 && j == 0) printf("\nthis: %d %d %d \n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			y = ycc[i][j][0] - 16;
			cb = ycc[i][j][1] - 128;
			cr = ycc[i][j][2] - 128;

			rgb[i][j][0] = (1164 * y + 1596 * cr) / 1000;
			rgb[i][j][1] = (1164 * y - 813 * cr - 391 * cb) / 1000;
			rgb[i][j][2] = (1164 * y + 2018 * cb) / 1000;

			rgb[i][j][0] = check_range(rgb[i][j][0], 0, 255);
			rgb[i][j][1] = check_range(rgb[i][j][1], 0, 255);
			rgb[i][j][2] = check_range(rgb[i][j][2], 0, 255);

		}
	}


}

int interpolate(int val, int i, int j) {
	// but actually just give same value
	return val;
}

int main(int argc, char *argv[]) {
	FILE *fp;
	fp = fopen("yccMatrix.txt", "r"); // read mode
	if (fp == NULL){
		perror("NO such file\n");
		exit(EXIT_FAILURE);
	}
	int rgb[W][H][BGR];
	int ycc[W][H][BGR];
	char *token_comma, *end_comma;
	char line[W*H];
	int i, j, k;
	for(i = 0; (fgets(line, sizeof line, fp) != NULL) || i < W; i++){
		char *newline=strtok(line,"\n");	
		token_comma = strtok_r(newline, ",", &end_comma);
		for(j = 0; (token_comma != NULL) || j < H; j++) {
			char *end_space;
			char *token_space = strtok_r(token_comma, " ", &end_space);
			for (k = 0; (token_space != NULL) || k < BGR; k++) {
				ycc[i][j][k] = atoi(token_space);
				token_space = strtok_r(NULL, " ", &end_space);
			}
			token_comma = strtok_r(NULL, ",", &end_comma);
		}
	}
	fclose(fp);

	// printf("\n%d %d %d \n\n", ycc[0][0][0], ycc[0][0][1], ycc[0][0][2]);
	// printf("%d ", rgb[x][y][z]);
	YCCtoRGB(ycc,rgb);
	// printf("\n%d %d %d \n\n", ycc[0][0][0], ycc[0][0][1], ycc[0][0][2]);

    //Create the image
    libattopng_t *png = libattopng_new(W, H, PNG_PALETTE);
	uint32_t palette[] = {
		RGBA(0, 0, 0xff, 0xff),
		RGBA(0, 0xff, 0, 0x80),
		RGBA(0xff, 0, 0, 0xff),
		RGBA(0xff, 0, 0xff, 0x80)
	};
	libattopng_set_palette(png, palette, 4);

	png = libattopng_new(W, H, PNG_RGB);
	int p=0;
	int q=0;
	for (p = 0; p < H; p++) {
		for (q = 0; q < W; q++) {
			libattopng_set_pixel(png, p, q, RGB(ycc[p][q][0] & 255, ycc[p][q][1] & 255, ycc[p][q][2] & 255));
			//printf("%" PRIx32 ",", RGB(x & 255, y & 255, 128) );
		}
		//printf("\n");
	}
	libattopng_save(png, "out_rgb.png");
	libattopng_destroy(png);
	return 0;
}
