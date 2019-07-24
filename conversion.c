#include <stdio.h>
#define x 2
#define y 2

void RGBtoYCC(float rgb[x][y][3], float ycc[x][y][3]) {

	int i, j;
	float r, g, b;
	for( i = 0; i < x; i++) {
		for ( j = 0; j < y; j++) {
			r = rgb[i][j][0];
			g = rgb[i][j][1];
			b = rgb[i][j][2];
			ycc[i][j][0] = 16 + 0.257*r + 0.504*g + 0.098*b;
			ycc[i][j/4][1] = 128 - 0.148*r - 0.291*g + 0.439*b;
			ycc[i][j/4][2] = 128 + 0.439*r - 0.368*g - 0.071*b;
			if(j % 4 == 3) {
				ycc[i][j][1] /= 4;
				ycc[i][j][2] /= 4;
			}
		}
	}

}

void YCCtoRGB() {

}

int main(int argc, char *argv[]) {
	printf("converting\n");
}
