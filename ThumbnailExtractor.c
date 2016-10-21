#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define	APP1_SIZE_LIMIT	(1L<<16)

int main(int argc, char *argv[])
{
    unsigned char c;
    unsigned int i;
    char *tn_name;
    FILE *fin, *fout;

    if(argc > 2) {
	printf("More than 1 argument. Please enter only JPG filename.");
	exit -1;
    }

    fin = fopen(argv[1],"r");
    if(!fin) {
	printf("Cannot open %s",argv[1]);
	exit -1;
    }
    
    if((fgetc(fin) == 0xFF) && (fgetc(fin) == 0xD8)) {
	printf("Valid JPEG file!!! :)\n");
    } else {
	printf("Not a valid JPEG file :(");
	exit -1;
    }

    if(!((fgetc(fin) == 0xFF) && (fgetc(fin) == 0xE1))) {
	printf("No Thumbnail present :(");
	exit -1;
    }

    /* Read till next 0xFFD8 for thumbnail */
    i = APP1_SIZE_LIMIT;
    while(!((fgetc(fin)==0xFF) && (fgetc(fin)==0xD8)) && i--);

    i = 0;
    while(argv[1][i++]!='\0');
    i--;

    tn_name = (char *)malloc(i+4);
    if(!tn_name) {
	printf("Error allocating memory!!!\n");
	exit -1;
    }

    if(argv[1][i-4]=='.') {
	argv[1][i-4] = '\0';
    } else {
	argv[1][i-5] = '\0';
    }
    strcpy(tn_name,argv[1]);
    strcat(tn_name,"_TN.jpg");
    
    fout = fopen(tn_name,"w");
    if(!fout) {
	printf("Cannot open tn.jpg\n");
	exit -1;
    }
    free(tn_name);

    fputc(0xFF,fout);
    fputc(0xD8,fout);

    do {
	c = fgetc(fin);
	if(c==0xFF) {
	    fputc(c,fout);
	    c = fgetc(fin);
	    if(c==0xD9) {
		printf("Found EOI\n");
		fputc(c,fout);
		break;
	    }
	}
	fputc(c,fout);
    } while(1);

    fclose(fout);
    fclose(fin);

    return 0;
}

