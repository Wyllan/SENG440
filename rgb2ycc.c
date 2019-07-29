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
	int rgb[3];
	char* token_comma;
	char line[W*H];
	while (fgets(line, sizeof line, fp) != NULL){
		char *string=line;
		token_comma = strtok(string, ",");
		while (token_comma != NULL) {
			/*char *token_space = strtok(token_comma, " ");
			while (token_space != NULL) {
				printf("%s ", token_space);
				token_space = strtok(NULL, " ");
			}
			printf("\n");*/
			printf("%s\n", token_comma);
			token_comma = strtok(NULL, ",");
		}
//		printf("%s", token);
//		token = strtok(NULL, ",");

	}

	











	int i,j,k;
	for (i=0;i<W;i++){
		for (j=0;j<H;j++) {
			for (k=0;k<BGR;k++){
				//	
			}
		}
	}
	fclose(fp);
	return 0;
}
