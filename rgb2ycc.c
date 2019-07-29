#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define W 255
#define H 255
#define BGR 3

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

void RGBtoYCC(int rgb[W][H][BGR], int ycc[W][H][BGR]) {

	int i, j;
	int r, g, b;
	for(i = 0; i < W; i++) {
		for (j = 0; j < H; j++) {
			r = rgb[i][j][0];
			g = rgb[i][j][1];
			b = rgb[i][j][2];

			ycc[i][j][0] = 4096 + (16843*r + 33030*g + 6423*b >> 16) >> 8;
			// ycc[i][j][0] = 16 + (16843*r + 33030*g + 6423*b ) >> 16;
			ycc[i][j][1] = 128 + (-9699*r - 19071*g + 28770*b ) >> 16;
			ycc[i][j][2] = 128 + (28770*r - 24117*g - 4653*b ) >> 16;

			// ycc[i][j][0] = 16.0 + 0.257*r + 0.504*g + 0.098*b;
			// ycc[i][j][1] = 128.0 - 0.148*r - 0.291*g + 0.439*b;
			// ycc[i][j][2] = 128.0 + 0.439*r - 0.368*g - 0.071*b;

			// if(j % 4 == 3) {
			// 	ycc[i][j/4][1] = ycc[i][j/4][1] >> 2;
			// 	ycc[i][j/4][2] = ycc[i][j/4][2] >> 2;
			// }

			ycc[i][j][0] = check_range(ycc[i][j][0], 16, 235);
			ycc[i][j][1] = check_range(ycc[i][j][1], 16, 240);
			ycc[i][j][2] = check_range(ycc[i][j][2], 16, 240);

			if (i < 5 && j == 0) {
				printf("%d %d %d\n", rgb[i][j][0], rgb[i][j][1], rgb[i][j][2]);
				printf("%d %d %d\n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			}
		}
	}
	printf("\n");
}

void YCCtoRGB(int rgb[W][H][BGR], int ycc[W][H][BGR]) {

	int i, j;
	float y, cr, cb;
	for(i = 0; i < W; i++) {
		for(j = 0; j < H; j++) {
			y = ycc[i][j][0] - 16;
			cr = ycc[i][j][1] - 128;
			cb = ycc[i][j][2] - 128;

			// rgb[i][j][0] = (76284 * y  + 104595 * cr + 32768) >> 16;
			// rgb[i][j][1] = (76284 * y - 25690 * cr - 53281 * cb + 32768) >> 16;
			// rgb[i][j][2] = (76284 * y + 135725 * cb + 32768) >> 16;

			rgb[i][j][0] = 1.164 * y + 1.596 * cr;
			rgb[i][j][1] = 1.164 * y - 0.813 * cr - 0.391 * cb;
			rgb[i][j][2] = 1.164 * y + 2.018 * cb;

			rgb[i][j][0] = check_range(rgb[i][j][0], 0, 255);
			rgb[i][j][1] = check_range(rgb[i][j][1], 0, 255);
			rgb[i][j][2] = check_range(rgb[i][j][2], 0, 255);

			if (i < 5 && j == 0) {
				printf("%d %d %d \n", rgb[i][j][0], rgb[i][j][1], rgb[i][j][2]);
				// printf("%d %d %d \n", ycc[i][j][0], ycc[i][j][1], ycc[i][j][2]);
			}
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
	RGBtoYCC(rgb,ycc);
	// printf("%d ", rgb[x][y][z]);
	YCCtoRGB(ycc,rgb);
	// printf("%d ", rgb[x][y][z]);
	// 		}
	// 		printf("\n");
	// 	}
	// }
//	fclose(fp);
	return 0;
}
