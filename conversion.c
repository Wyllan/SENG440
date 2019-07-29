#include <stdio.h>
#define res_x 2
#define res_y 2

// Takes in a value, a lower bound, and an upper bound
// Saturation arithmetic (clamping)
int check_range(int val, int lb, int ub) {
	// rgb - [0,255]
	// y   - [16,235]
	// cr  - [16,240]
	// cb  - [16,240]

	if(val < lb)
		return lb;
	else if (val > ub)
		return ub;
	else
		return val;
}

void RGBtoYCC(unsigned char rgb[res_x][res_y][3], unsigned char ycc[res_x][res_y][3]) {

	int i, j, r, g, b;
	for(i = 0; i < res_x; i++) {
		for (j = 0; j < res_y; j++) {
			r = rgb[i][j][0];
			g = rgb[i][j][1];
			b = rgb[i][j][2];

			ycc[i][j][0] = 16 + (16843*r + 33030*g + 6423*b + 32768 ) >> 16;
			ycc[i][j/4][1] += 128 + (-9699*r - 19071*g + 28770*b + 32768 ) >> 16;
			ycc[i][j/4][2] += 128 + (28770*r - 24117*g - 4653*b + 32768 ) >> 16;

			if(j % 4 == 3) {
				ycc[i][j/4][1] = ycc[i][j/4][1] >> 2;
				ycc[i][j/4][2] = ycc[i][j/4][2] >> 2;
			}

			check_range(ycc[i][j][0], 16, 235);
			check_range(ycc[i][j][1], 16, 240);
			check_range(ycc[i][j][2], 16, 240);
		}
	}

}

void YCCtoRGB(unsigned char rgb[res_x][res_y][3], unsigned char ycc[res_x][res_y][3]) {

	int i, j, y, cr, cb;
	for(i = 0; i < res_x; i++) {
		for(j = 0; j < res_y; j++) {
			y = ycc[i][j][0] - 16;
			cr = ycc[i][j][1] - 128;
			cb = ycc[i][j][2] - 128;

			rgb[i][j][0] = (76284 * y  + 104595 * cr + 32768) >> 16;
			rgb[i][j][1] = (76284 * y - 25690 * cr - 53281 * cb + 32768) >> 16;
			rgb[i][j][2] = (76284 * y + 135725 * cb + 32768) >> 16;

			check_range(rgb[i][j][0], 0, 255);
			check_range(rgb[i][j][1], 0, 255);
			check_range(rgb[i][j][2], 0, 255);
		}
	}


}

int interpolate(int val, int i, int j) {
	// but actually just give same value
	return val;
}

int main(int argc, char *argv[]) {
	printf("converting\n");
}
