#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define W 255
#define H 255
#define BGR 3

int main(int argc, char *argv[]) {
	FILE *fp;
	fp = fopen("rgbMatrix.txt", "r"); // read mode
	if (fp == NULL){
		perror("NO such file\n");
		exit(EXIT_FAILURE);
	}
	char* rgb[W][H][BGR];
	char* token_comma;
//	char* token_space;
	char* end_comma;
	char line[W*H];
	int i=0;
       	int j=0;
	int k=0;
	while (fgets(line, sizeof line, fp) != NULL || i < W){
		token_comma = strtok_r(line, ",", &end_comma);
		while (token_comma != NULL || j < H) {
			char *end_space;
			char *token_space = strtok_r(token_comma, " ", &end_space);
			//printf("%s\n", token_space);
			while (token_space != NULL || k < BGR) {
				//rgb[i][j][k] = token_space;
				printf("%s ", token_space);
				token_space = strtok_r(NULL, " ", &end_space);
				k++;
			}
			printf("\n");
			//printf("%s\n", token_comma);
			token_comma = strtok_r(NULL, ",", &end_comma);
			j++;
		}
//		printf("%s", token);
//		token = strtok(NULL, ",");
		i++;

	}

	











	/*int i,j,k;
	for (i=0;i<W;i++){
		for (j=0;j<H;j++) {
			for (k=0;k<BGR;k++){
				//	
			}
		}
	}*/
	fclose(fp);
	return 0;
}
