#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <libattopng.h>

#define N 255
#define N_half 127
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

int main(int argc, char *argv[]) {

	FILE *fp;
	fp = fopen("rgbMatrix.txt", "r"); // read mode
	if (fp == NULL){
		perror("NO such file\n");
		exit(EXIT_FAILURE);
	}
	unsigned int rgb[N][N][BGR];
	unsigned int ycc[N][N][BGR];
	char *token_comma, *end_comma;
	char line[N*N];
	int i, j, k;

	for(i = 0; (fgets(line, sizeof line, fp) != NULL) || i < N; i++){
		char *newline=strtok(line,"\n");	
		token_comma = strtok_r(newline, ",", &end_comma);
		for(j = 0; (token_comma != NULL) || j < N; j++) {
			char *end_space;
			char *token_space = strtok_r(token_comma, " ", &end_space);
			for (k = 0; (token_space != NULL) || k < BGR; k++) {
				rgb[i][j][k] = atoi(token_space);
				token_space = strtok_r(NULL, " ", &end_space);
			}
			token_comma = strtok_r(NULL, ",", &end_comma);
		}
	}
	fclose(fp);

	int temp_i, temp_j;

	// RGBtoYCC

    uint8_t r, g, b;
    for(i = N-1; i; i--) {
        for (j = N-1; j; j--) {
            r = rgb[i][j][0];
            g = rgb[i][j][1];
            b = rgb[i][j][2];

            ycc[i][j][0] = (16000 + 257*r + 504*g + 98*b) / 1000;
			ycc[i][j][0] = ycc[i][j][0] < 16 ? 16 : (ycc[i][j][0] > 240 ? 240 : ycc[i][j][0]);

            if(i % 2 == 0 || j % 2 == 0) {
                temp_i = i/2;
                temp_j = j/2;

                ycc[temp_i][temp_j][1] = (128000 - 148*r - 291*g + 439*b) / 1000;
				ycc[temp_i][temp_j][1] = ycc[temp_i][temp_j][1] < 16 ? 16 : (ycc[temp_i][temp_j][1] > 240 ? 240 : ycc[temp_i][temp_j][1]);

                ycc[temp_i][temp_j][2] = (128000 + 439*r - 368*g - 71*b) / 1000;
				ycc[temp_i][temp_j][2] = ycc[temp_i][temp_j][2] < 16 ? 16 : (ycc[temp_i][temp_j][2] > 240 ? 240 : ycc[temp_i][temp_j][2]);

            }
        }
    }
	

	// YCCtoRGB

	uint32_t y, cr, cb;

	for(i = N-1; i; i--) {
		for(j = N-1; j; j--) {

			y = 1164 * (ycc[i][j][0] - 16);

			if( i % 2 == 0 || j % 2 == 0) {
				temp_i = i/2;
				temp_j = j/2;
				cb = ycc[temp_i][temp_j][1] - 128;
				cr = ycc[temp_i][temp_j][2] - 128;
			}

			rgb[i][j][0] = (y + 1596 * cr) / 1000;
			rgb[i][j][1] = (y - 813 * cr - 391 * cb) / 1000;
			rgb[i][j][2] = (y + 2018 * cb) / 1000;

			rgb[i][j][0] = rgb[i][j][0] > 255 ? 255 : rgb[i][j][0];
			rgb[i][j][1] = rgb[i][j][1] > 255 ? 255 : rgb[i][j][1];
			rgb[i][j][2] = rgb[i][j][2] > 255 ? 255 : rgb[i][j][2];

		}
	}


	libattopng_t *png = libattopng_new(N, N, PNG_RGB);
	int p, q;
	for(p = N; p; p--) {
		for (q = N; q; q--)
			libattopng_set_pixel(png, p, q, RGB(rgb[q][p][0] & 255, rgb[q][p][1] & 255, rgb[q][p][2] & 255));
	}
	libattopng_save(png, "out_rgb.png");
	libattopng_destroy(png);
	return 0;
}
