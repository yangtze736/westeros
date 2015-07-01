///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: renameFile.cpp
//
// Description:
//
// Created: 2015年05月21日 星期四 09时10分51秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	// read file to buffer
	FILE *fr = fopen(argv[1], "rb");
	fseek(fr, 0, SEEK_END);
	long lsize = ftell(fr);
	rewind(fr);

	char *buffer = (char*)malloc(sizeof(char)*lsize);
	if(buffer == NULL)
		return -1;

	size_t result = fread(buffer, sizeof(char), lsize, fr);
	if(result != lsize)
		return -1;

	strcat(argv[1], "_bak");

	// buffer write to file
	FILE *fw = fopen(argv[1], "wb");
	fwrite(buffer, sizeof(char), lsize, fw);

	//
	free(buffer);
	fclose(fr);
	fclose(fw);

	return 0;
}
