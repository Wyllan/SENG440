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
	if (x % 1000 > 499)
		return x;
	else return x;
}

void RGBtoYCC(int rgb[W][H][BGR], int ycc[W][H/4][BGR]) {

	int i, j;
	int r, g, b;
	for(i = 0; i < W; i++) {
		for (j = 0; j < H; j++) {
			r = rgb[i][j][0];
			// r = (int) (rgb[i][j][0] < 12 ? 0 : rgb[i][j][0]) << 8;
			g = rgb[i][j][1];
			b = rgb[i][j][2];

			// ycc[i][j][0] = 4096 + (16843*r + 33030*g + 6423*b >> 16) >> 8;
			// ycc[i][j][0] = 16 + (16843*r + 33030*g + 6423*b ) >> 16;
			// ycc[i][j][1] = 128 + (-9699*r - 19071*g + 28770*b ) >> 16;
			// ycc[i][j][2] = 128 + (28770*r - 24117*g - 4653*b ) >> 16;

			ycc[i][j][0] = rounding(16000 + 257*r + 504*g + 98*b) / 1000;
			// ycc[i][j][0] = 16.0 + 0.257*r + 0.504*g + 0.098*b;
			if(j % 4 == 3) {
				ycc[i][j/4][1] = rounding(128000 - 148*r - 291*g + 439*b) / 1000;
				ycc[i][j/4][2] = rounding(128000 + 439*r - 368*g - 71*b) / 1000;
			}

			// if(j % 4 == 3) {
			// 	ycc[i][j/4][1] = ycc[i][j/4][1] >> 2;
			// 	ycc[i][j/4][2] = ycc[i][j/4][2] >> 2;
			// }

			ycc[i][j][0] = check_range(ycc[i][j][0], 16, 235);
			ycc[i][j][1] = check_range(ycc[i][j][1], 16, 240);
			ycc[i][j][2] = check_range(ycc[i][j][2], 16, 240);

			// if (i < 5 && j == 0) {
			// 	printf("%d %d %d\n", rgb[i][j][0], rgb[i][j][1], rgb[i][j][2]);
			// 	printf("%d %d %d\n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			// }
		}
	}
	// printf("\n");
}

void YCCtoRGB(int ycc[W][H/4][BGR], int rgb[W][H][BGR]) {

	int i, j;
	int y, cr, cb;
	for(i = 0; i < W; i++) {
		for(j = 0; j < H; j++) {
			if( i == 0 && j == 0) printf("\nthis: %d %d %d \n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			y = ycc[i][j][0] - 16;
			cb = ycc[i][j/4][1] - 128;
			cr = ycc[i][j/4][2] - 128;

			// rgb[i][j][0] = (76284 * y  + 104595 * cr + 32768) >> 16;
			// rgb[i][j][1] = (76284 * y - 25690 * cr - 53281 * cb + 32768) >> 16;
			// rgb[i][j][2] = (76284 * y + 135725 * cb + 32768) >> 16;

			rgb[i][j][0] = rounding(1164 * y + 1596 * cr) / 1000;
			rgb[i][j][1] = rounding(1164 * y - 813 * cr - 391 * cb) / 1000;
			rgb[i][j][2] = rounding(1164 * y + 2018 * cb) / 1000;

			rgb[i][j][0] = check_range(rgb[i][j][0], 0, 255);
			rgb[i][j][1] = check_range(rgb[i][j][1], 0, 255);
			rgb[i][j][2] = check_range(rgb[i][j][2], 0, 255);

			// if (i < 5 && j == 0) {
			// 	printf("%d %d %d \n", rgb[i][j][0], rgb[i][j][1], rgb[i][j][2]);
			// 	printf("%d %d %d \n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			// }
		}
	}


}

int interpolate(int val, int i, int j) {
	// but actually just give same value
	return val;
}

int main(int argc, char *argv[]) {
	FILE *fp;
	fp = fopen("rgbMatrix.txt", "r"); // read mode
	if (fp == NULL){
		perror("NO such file\n");
		exit(EXIT_FAILURE);
	}
	int rgb[W][H][BGR];
	int ycc[W][H][BGR];
	char* token_comma;
//	char* token_space;
	char* end_comma;
	char line[W*H];
	int i=0;
    int j=0;
	int k=0;
	while ((fgets(line, sizeof line, fp) != NULL) ||  i < W){
		char *newline=strtok(line,"\n");	
		token_comma = strtok_r(newline, ",", &end_comma);
		//printf("%s\n", end_comma);
		j=0;
		while ((token_comma != NULL) || j < H) {
			char *end_space;
			//printf("%d", j);
			char *token_space = strtok_r(token_comma, " ", &end_space);
			//printf("%s\n", token_space);
			k=0;
			while ((token_space != NULL)|| k<BGR) {
				//printf("%d", k);
				//printf("%d %d %d\n",i,j,k);
				rgb[i][j][k] = atoi(token_space);
	//			printf("%d ", rgb[i][j][k]);
				token_space = strtok_r(NULL, " ", &end_space);
				k++;
			}
	///		printf("\n");
			//printf("%s\n", token_comma);
			token_comma = strtok_r(NULL, ",", &end_comma);
			j++;
		}
//		printf("%s", token);
//		token = strtok(NULL, ",");
		i++;

	}
	fclose(fp);
	








	// rgb = {255, 255, 255};
	// ycc = 

	///The actual reading image part
	// int x,y,z;
	// for (x=0;x<W;x++){
	// 	for (y=0;y<H;y++) {
	// 		for (z=0;z<BGR;z++){
				//TODO: insert the rgb2ycc functions
	// printf("\n%d %d %d \n\n", ycc[0][0][0], ycc[0][0][1], ycc[0][0][2]);
	RGBtoYCC(rgb,ycc);
	// printf("\n%d %d %d \n\n", ycc[0][0][0], ycc[0][0][1], ycc[0][0][2]);
	// printf("%d ", rgb[x][y][z]);
	// YCCtoRGB(ycc,rgb);
	// printf("\n%d %d %d \n\n", ycc[0][0][0], ycc[0][0][1], ycc[0][0][2]);
	// printf("%d ", rgb[x][y][z]);
	// 		}
	// 		printf("\n");
	// 	}
	// }
//	fclose(fp);
	//return 0;






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
	libattopng_save(png, "supposeycc.png");
	libattopng_destroy(png);
	return 0;
}
