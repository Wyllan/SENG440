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
	int rgb[W][H][BGR];
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
	










	///The actual reading image part
	int x,y,z;
	for (x=0;x<W;x++){
		for (y=0;y<H;y++) {
			for (z=0;z<BGR;z++){
				//TODO: insert the rgb2ycc functions
				printf("%d ", rgb[x][y][z]);
			}
			printf("\n");
		}
	}
//	fclose(fp);
	return 0;
}
