// Program to get vertices from an LBA2 model, and scale them according to the user.

// Libraries.
#include <iostream>

// Macros.
#define MAXFILE    50
#define VERTEXNUM  0x28
#define VERTEXOFF  0x2C
#define TEXTURENUM 0x58
#define TEXTUREOFF 0x5C
#define HEADERSIZE 0x60

// Variables.
FILE *fileIn;
FILE *fileOut;

int scale;

short vertexNum;
short vertexOff;

short textureNum;
short textureOff;

char fileName[MAXFILE];

// Structs.
typedef struct
	{
	short x;
	short y;
	short z;
	short bone;
	} vertex;

void loadModel(char fileName[MAXFILE])
	{
	printf("Loading model: %s \n", fileName);
	
	fileIn = fopen(fileName, "rb");
	fileOut = fopen("newTest.lun", "wb");
	
	// get vertex number
	fseek(fileIn, VERTEXNUM, SEEK_SET);
	fread(&vertexNum, sizeof(short), 0x1, fileIn);
	
	// get vertex offset
	fseek(fileIn, VERTEXOFF, SEEK_SET);
	fread(&vertexOff, sizeof(short), 0x1, fileIn);
	
	// get texture number
	fseek(fileIn, TEXTURENUM, SEEK_SET);
	fread(&textureNum, sizeof(short), 0x1, fileIn);
	
	// get texture offset
	fseek(fileIn, TEXTUREOFF, SEEK_SET);
	fread(&textureOff, sizeof(short), 0x1, fileIn);
	
	// get byte size
	short byteLength;
	byteLength = ((textureNum*4) + textureOff);
	
	// create array of byte length
	char allBytes[byteLength];
	
	// go to beginning of file
	fseek(fileIn, 0x0, SEEK_SET);
	// copy whole file
	for (int i = 1; i <= byteLength; i++)
		{
		fseek(fileIn, 0x0, SEEK_CUR);
		fread(&allBytes[i], sizeof(char), 0x1, fileIn);
		fwrite(&allBytes[i], sizeof(char), 0x1, fileOut);
		};
	
	// create vertices
	vertex theVertices[vertexNum];
	
	// go to vertex offset
	fseek(fileIn, vertexOff, SEEK_SET);
	
	// get vertices
	for (int i = 1; i <= vertexNum; i++)
		{
		fread(&theVertices[i].x,    sizeof(short), 0x1, fileIn);
		fread(&theVertices[i].y,    sizeof(short), 0x1, fileIn);
		fread(&theVertices[i].z,    sizeof(short), 0x1, fileIn);
		fread(&theVertices[i].bone, sizeof(short), 0x1, fileIn);
		
		printf("x: %x \n",    theVertices[i].x);
		printf("y: %x \n",    theVertices[i].y);
		printf("z: %x \n",    theVertices[i].z);
		printf("bone: %x \n", theVertices[i].bone);
		};
	
	// create new vertices
	vertex newVertices[vertexNum];
	
	// convert vertices
	for (int i = 1; i <= vertexNum; i++)
		{
		newVertices[i].x =    (theVertices[i].x * (short)scale);
		newVertices[i].y =    (theVertices[i].y * (short)scale);
		newVertices[i].z =    (theVertices[i].z * (short)scale);
		newVertices[i].bone = (theVertices[i].bone);
		
		/*
		newVertices[i].x =    (theVertices[i].x << 0x3 && scale >> 0x1);
		newVertices[i].y =    (theVertices[i].y << 0x3 && scale >> 0x1);
		newVertices[i].z =    (theVertices[i].z << 0x3 && scale >> 0x1);
		newVertices[i].bone = (theVertices[i].bone);
		*/
		};
	
	// go to vertex offset
	fseek(fileOut, vertexOff, SEEK_SET);
	
	// set vertices
	for (int i = 1; i <= vertexNum; i++)
		{
		fwrite(&newVertices[i].x,    sizeof(short), 0x1, fileOut);
		fwrite(&newVertices[i].y,    sizeof(short), 0x1, fileOut);
		fwrite(&newVertices[i].z,    sizeof(short), 0x1, fileOut);
		fwrite(&newVertices[i].bone, sizeof(short), 0x1, fileOut);
		};
	
	fclose(fileIn);
	fclose(fileOut);
	
	printf("Model done loading. \n");
	};

int main()
	{
	printf("Type in the name of the file: ");
	scanf("%s", fileName);
	
	printf("Type in the scale of the model: ");
	scanf("%d", &scale);
	
	loadModel(fileName);
	
	return 0;
	};
