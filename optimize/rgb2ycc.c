#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <libattopng.h>

#define N 255
#define N_half 127
#define BGR 3
#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
#define RGB(r, g, b) RGBA(r, g, b, 0xff)

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
    uint8_t r, g, b;
    for(i = N; i; i--) {
        for (j = N; j; j--) {
            r = rgb[i][j][0];
            g = rgb[i][j][1];
            b = rgb[i][j][2];


            ycc[i][j][0] = (16000 + 257*r + 504*g + 98*b) / 1000;
            if(ycc[i][j][0] < 16)  {
                ycc[i][j][0] = 16;
            }
            else if (ycc[i][j][0] > 235){
                ycc[i][j][0] = 235;
            }

            if(i % 2 == 0 || j % 2 == 0) {
                temp_i = i/2;
                temp_j = j/2;

                ycc[temp_i][temp_j][1] = (128000 - 148*r - 291*g + 439*b) / 1000;
                if(ycc[temp_i][temp_j][1] < 16)  {
                    ycc[temp_i][temp_j][1] = 16;
                }
                else if (ycc[temp_i][temp_j][1] > 240){
                    ycc[temp_i][temp_j][1] = 240;
                }
                ycc[temp_i][temp_j][2] = (128000 + 439*r - 368*g - 71*b) / 1000;
                if(ycc[temp_i][temp_j][2] < 16)  {
                    ycc[temp_i][temp_j][2] = 16;
                }
                else if (ycc[temp_i][temp_j][2] > 240){
                    ycc[temp_i][temp_j][2] = 240;
                }
            }
        }
    }


    libattopng_t *png = libattopng_new(N_half, N_half, PNG_RGB);

	int p,q;

	for (p = N_half; p; p--) {
		for (q = N_half; q; q--) {
            libattopng_set_pixel(png, p, q, RGB(ycc[p][q][0] & 255, ycc[p][q][1] & 255, ycc[p][q][2] & 255));
		}
	}
    libattopng_save(png, "out_ycc.png");
    libattopng_destroy(png);

	return 0;
}
