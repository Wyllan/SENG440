#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <libattopng.h>

#define W 255
#define H 255
#define H_down 63
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
	unsigned int rgb[W][H][BGR];
	unsigned int ycc[W][H_down][BGR];
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
				rgb[i][j][k] = atoi(token_space);
				token_space = strtok_r(NULL, " ", &end_space);
			}
			token_comma = strtok_r(NULL, ",", &end_comma);
		}
	}
	fclose(fp);	

    int temp;
    uint8_t r, g, b;
    for(i = 0; i < W; i++) {
        for (j = 0; j < H; j++) {
            r = rgb[i][j][0];
            g = rgb[i][j][1];
            b = rgb[i][j][2];

            // ycc[i][j][0] = 16 + (16843*r + 33030*g + 6423*b ) >> 16;
            // ycc[i][j][1] = 128 + (-9699*r - 19071*g + 28770*b ) >> 16;
            // ycc[i][j][2] = 128 + (28770*r - 24117*g - 4653*b ) >> 16;

            ycc[i][j][0] = (16000 + 257*r + 504*g + 98*b) / 1000;
            if(ycc[i][j][0] < 16)  {
                ycc[i][j][0] = 16;
            }
            else if (ycc[i][j][0] > 235){
                ycc[i][j][0] = 235;
            }

            if(j % 4 == 3) {
                temp = j/4;

                ycc[i][temp][1] = (128000 - 148*r - 291*g + 439*b) / 1000;
                if(ycc[i][temp][1] < 16)  {
                    ycc[i][temp][1] = 16;
                }
                else if (ycc[i][temp][1] > 240){
                    ycc[i][temp][1] = 240;
                }
                ycc[i][temp][2] = (128000 + 439*r - 368*g - 71*b) / 1000;
                if(ycc[i][temp][2] < 16)  {
                    ycc[i][temp][2] = 16;
                }
                else if (ycc[i][temp][2] > 240){
                    ycc[i][temp][2] = 240;
                }
            }
        }
    }


    libattopng_t *png = libattopng_new(W, H_down, PNG_RGB);

	int p=0;
	int q=0;

	for (p = 0; p < W; p++) {
		for (q = 0; q < H_down; q++) {
            libattopng_set_pixel(png, p, q, RGB(ycc[p][q][0] & 255, ycc[p][q][1] & 255, ycc[p][q][2] & 255));
		}
	}
    libattopng_save(png, "out_ycc.png");
    libattopng_destroy(png);

	return 0;
}
