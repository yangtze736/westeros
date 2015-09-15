///////////////////////////////////////////////////////////
// Copyright (c) 2015, ShangHai xxxx Inc.
//
// FileName: mfcFile.cpp
//
// Description:
//
// Created: 2015年05月26日 星期二 09时14分56秒
// Revision: Revision: 1.0
// Compiler: g++
//
///////////////////////////////////////////////////////////

#include "mfcFile.h"
#include "trap.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

MfcFile::MfcFile()
{
}

MfcFile::~MfcFile()
{
}

unsigned int MfcFile::getFileSize(const std::string &fileName)
{
	struct stat statbuf;
	stat(fileName.c_str(), &statbuf);

	return statbuf.st_size;
}

bool MfcFile::reName(const std::string &oldPath, const std::string &newPath)
{
	if(rename(oldPath.c_str(), newPath.c_str()) != 0)
	{
		return false;
	}
	chmod(newPath.c_str() , 0666);
	return true;
}

bool MfcFile::moveFile(const std::string &src, const std::string &dst)
{
	char buf[1024] = {0};
	sprintf(buf,"mv %s %s",src.c_str(),dst.c_str());
	int ret = system(buf);
	
	chmod(dst.c_str() , 0666);
	return true;
}

bool MfcFile::copyFile(const std::string &src, const std::string &dst)
{
	char buf[1024] = {0};
	sprintf(buf,"cp %s %s",src.c_str(),dst.c_str());
	int ret = system(buf);

	chmod(dst.c_str() , 0666);
	return true;
}

bool MfcFile::delFile(const std::string &filePath)
{
	if(remove(filePath.c_str()) != 0)
	{
		return false;
	}

	return true;
}

// We create tmp file like this:
// /usr/tmp/demo.cpp --> /usr/tmp/.demo.cpp
std::string MfcFile::createTmpFile(const std::string &srcFile)
{
	std::string::size_type found = srcFile.find_last_of("/");
	return srcFile.substr(0,found) + "/." + srcFile.substr(found+1);
}

int MfcFile::mergeFile(const char *filename)
{
	FILE *fp_split, *fp_merged;
	size_t len, bytes_read;
	uoff_t merged_size, split_size, split_pos;

	int filenr, amount, startnr;
	char *split_fname, *orig_fname, 
		 *missing_fname, *c_bytes;
	struct stat buf;

	len = strlen(filename);
	if(len < 5 || filename[len - 4] != '.'){
		fprintf(stderr, "File name must end with .000 or .001\n");
		return 1;
	}
	switch(filename[len - 1]){
		case '0':
			startnr = 0;
			break;
		case '1':
			startnr = 1;
			break;
		default:
			fprintf(stderr, "File name must end with .000 or .001\n");
			return 1;
	}

	orig_fname = (char *)malloc((len - 4 + 1)*sizeof(char));
	split_fname = (char *)malloc((len + 1)*sizeof(char));

	strncpy(orig_fname, filename, len - 4);
	orig_fname[len - 4] = '\0';

	char merged_fname[128] = {0};
	strncpy(merged_fname, orig_fname, len - 4);

	amount = 0;
	merged_size = 0;
	missing_fname = NULL;
	
	for(filenr = startnr; filenr < 1000; filenr++)
	{
		if(filenr < 10)
			sprintf(split_fname, "%s.00%d", orig_fname, filenr);
		else if(filenr < 100)
			sprintf(split_fname, "%s.0%d", orig_fname, filenr);
		else
			sprintf(split_fname, "%s.%d", orig_fname, filenr);

		if(stat(split_fname, &buf) == -1){
			if(filenr == startnr){
				fprintf(stderr, "Cannot find %s\n", split_fname);
				return 1;
			}
			else{
				if(missing_fname == NULL){
					PR("...strdup...");
					missing_fname = strdup(split_fname);
				}
			}
		}
		else{
			if(!S_ISREG(buf.st_mode)){
				fprintf(stderr, "Cannot find %s\n", split_fname);
				return 1;
			}
			else{
				if(missing_fname != NULL){
					fprintf(stderr, "Missing file %s!\n", missing_fname);
					return 1;
				}
				merged_size += buf.st_size;
				amount++;
			}
		}
	}

	if(stat(merged_fname, &buf) == 0){
		fprintf(stderr, "%s already exists! Aborting...\n", merged_fname);
		return 1;
	}

	fp_merged = fopen(merged_fname, "w");
	if(fp_merged == NULL){
		perror("cannot create output file.\n");
		return 1;
	}

	c_bytes = (char *)malloc(READ_BUFSIZE*sizeof(char));
	for(filenr = startnr; filenr <= amount + (startnr - 1); filenr++)
	{
		if(filenr < 10)
			sprintf(split_fname, "%s.00%d", orig_fname, filenr);
		else if(filenr < 100)
			sprintf(split_fname, "%s.0%d", orig_fname, filenr);
		else if(filenr < 1000)
			sprintf(split_fname, "%s.%d", orig_fname, filenr);

		if(stat(split_fname, &buf) == -1){
			perror("cannot stat input file.\n");
			fclose(fp_merged);
			return 1;
		}
		split_size = buf.st_size;
		
		fp_split = fopen(split_fname, "r");
		if(fp_split == NULL){
			perror("cannot open input file.\n");
			fclose(fp_merged);
			return 1;
		}

		split_pos = 0;
		while(split_pos < split_size)
		{
			bytes_read = fread(c_bytes, 1, READ_BUFSIZE, fp_split);
			if(bytes_read != READ_BUFSIZE && !feof(fp_split)){
				perror("error while reading input file.\n");
				fclose(fp_split);
				fclose(fp_merged);
				return 1;
			}

			len = fwrite(c_bytes, 1, bytes_read, fp_merged);
			if(len != bytes_read){
				perror("error while writing to output file");
				fclose(fp_split);
				fclose(fp_merged);
				return 1;
			}
			split_pos += bytes_read;
		}

		fclose(fp_split);
	}

	free(orig_fname), orig_fname = NULL;
	free(split_fname), split_fname = NULL;
	free(missing_fname), missing_fname = NULL;
	free(c_bytes), c_bytes = NULL;
	fclose(fp_merged);
	chmod(split_fname , 0666);
	
	return 0;
}

int MfcFile::splitFile(const char *filename, uoff_t s)
{
	FILE *fp_orig, *fp_split;
	uoff_t orig_pos, orig_size, split_pos, split_size,
		   bytes_left, piece_bytes;
	size_t bytes_read, allow_read, len;
	
	int filenr, pieces;
	struct stat buf;
	char *c_byte, *split_fname;

	orig_pos = orig_size = 0;
	split_pos = split_size = 0;
	filenr = 1;

	if(stat(filename, &buf) == -1){
		perror("cannot stat input file");
		return 1;
	}
	if(!S_ISREG(buf.st_mode)){
		fprintf(stderr, "%s is not a regular file.\n", filename);
		return 1;
	}
	orig_size = buf.st_size;

	// default split filesize
	if(s == 0){
		//s = 500*1024*1024; // 500M
		s = 300*1024*1024; // 300M
	}
	if(s >= orig_size){
		fprintf(stderr, "splitted filesize >= origin filesize.\n");
		return 1;
	}

	fp_orig = fopen(filename, "r");
	if(fp_orig == NULL){
		perror("error opening input file.\n");
		return -1;
	}

	split_size = s;
	bytes_left = orig_size;

	pieces = orig_size / split_size;
	if((orig_size % split_size) > 0)
		pieces++;

	if(pieces > 100){
		fprintf(stderr, "You will be ending up with over %d files, \
				could choose a bigger output size?.\n", pieces);
		return 1;
	}

	len = strlen(filename) + 5; // ".001" + null terminator
	split_fname = (char *)malloc(len * sizeof(char));
	if(split_fname == NULL){
		perror("malloc split_fname false.\n");
		return 1;
	}
	c_byte = (char *)malloc(READ_BUFSIZE * sizeof(char));
	if(c_byte == NULL){
		perror("malloc c_byte false.\n");
		return 1;
	}

	PR("Splitting %s into %d pipeces.", filename, pieces);

	while(orig_pos < orig_size)
	{
		if(filenr < 10)
			sprintf(split_fname, "%s.00%d", filename, filenr);
		else if(filenr < 100)
			sprintf(split_fname, "%s.0%d", filename, filenr);
		else
			sprintf(split_fname, "%s.%d", filename, filenr);

		fp_split = fopen(split_fname, "w");
		if(fp_split == NULL){
			perror("cannot create output file.\n");
			fclose(fp_orig);
			return 1;
		}
		
		split_pos = 0;
		bytes_read = 0;
		allow_read = READ_BUFSIZE;

		while(split_pos < split_size)
		{
			piece_bytes = split_size - split_pos; // how much to read

			if(piece_bytes < (uoff_t)allow_read)
				allow_read = (size_t)piece_bytes;

			bytes_read = fread(c_byte, 1, allow_read, fp_orig); // how much to read
			if(bytes_read != allow_read){
				if(feof(fp_orig))
					fprintf(stderr, "unexpected EOF!...\n");
				perror("error while reading input file.\n");
				fclose(fp_split);
				fclose(fp_orig);
				return 1;
			}
			len = fwrite(c_byte, 1, bytes_read, fp_split);
			if(len != bytes_read){
				perror("error while writing to output file.\n");
				fclose(fp_split);
				fclose(fp_orig);
				return 1;
			}

			orig_pos += bytes_read;
			split_pos += bytes_read;
		}

		bytes_left -= split_size;
		if(bytes_left < split_size)
			split_size = bytes_left;

		filenr++;
		fclose(fp_split);
	}

	free(c_byte), c_byte = NULL;
	free(split_fname), split_fname = NULL;
	fclose(fp_orig);

	return 0;
}

int MfcFile::splitFile(const char *filename, std::list<std::string> &splitFileList, uoff_t s)
{
	FILE *fp_orig, *fp_split;
	uoff_t orig_pos, orig_size, split_pos, split_size,
		   bytes_left, piece_bytes;
	size_t bytes_read, allow_read, len;
	
	int filenr, pieces;
	struct stat buf;
	char *c_byte, *split_fname;

	orig_pos = orig_size = 0;
	split_pos = split_size = 0;
	filenr = 1;

	if(stat(filename, &buf) == -1){
		perror("cannot stat input file");
		return 1;
	}
	if(!S_ISREG(buf.st_mode)){
		fprintf(stderr, "%s is not a regular file.\n", filename);
		return 1;
	}
	orig_size = buf.st_size;

	// default split filesize
	if(s == 0){
		//s = 500*1024*1024; // 500M
		s = 300*1024*1024; // 300M
	}
	if(s >= orig_size){
		fprintf(stderr, "splitted filesize >= origin filesize.\n");
		return 1;
	}

	fp_orig = fopen(filename, "r");
	if(fp_orig == NULL){
		perror("error opening input file.\n");
		return -1;
	}

	split_size = s;
	bytes_left = orig_size;

	pieces = orig_size / split_size;
	if((orig_size % split_size) > 0)
		pieces++;

	if(pieces > 100){
		fprintf(stderr, "You will be ending up with over %d files, \
				could choose a bigger output size?.\n", pieces);
		return 1;
	}

	len = strlen(filename) + 5; // ".001" + null terminator
	split_fname = (char *)malloc(len * sizeof(char));
	if(split_fname == NULL){
		perror("malloc split_fname false.\n");
		return 1;
	}
	c_byte = (char *)malloc(READ_BUFSIZE * sizeof(char));
	if(c_byte == NULL){
		perror("malloc c_byte false.\n");
		return 1;
	}

	PR("Splitting %s into %d pipeces.", filename, pieces);

	while(orig_pos < orig_size)
	{
		if(filenr < 10)
			sprintf(split_fname, "%s.00%d", filename, filenr);
		else if(filenr < 100)
			sprintf(split_fname, "%s.0%d", filename, filenr);
		else
			sprintf(split_fname, "%s.%d", filename, filenr);

		fp_split = fopen(split_fname, "w");
		if(fp_split == NULL){
			perror("cannot create output file.\n");
			fclose(fp_orig);
			return 1;
		}
		// insert into list
		splitFileList.push_back(std::string(split_fname));
		
		split_pos = 0;
		bytes_read = 0;
		allow_read = READ_BUFSIZE;

		while(split_pos < split_size)
		{
			piece_bytes = split_size - split_pos; // how much to read

			if(piece_bytes < (uoff_t)allow_read)
				allow_read = (size_t)piece_bytes;

			bytes_read = fread(c_byte, 1, allow_read, fp_orig); // how much to read
			if(bytes_read != allow_read){
				if(feof(fp_orig))
					fprintf(stderr, "unexpected EOF!...\n");
				perror("error while reading input file.\n");
				fclose(fp_split);
				fclose(fp_orig);
				return 1;
			}
			len = fwrite(c_byte, 1, bytes_read, fp_split);
			if(len != bytes_read){
				perror("error while writing to output file.\n");
				fclose(fp_split);
				fclose(fp_orig);
				return 1;
			}

			orig_pos += bytes_read;
			split_pos += bytes_read;
		}

		bytes_left -= split_size;
		if(bytes_left < split_size)
			split_size = bytes_left;

		filenr++;
		fclose(fp_split);
	}

	free(c_byte), c_byte = NULL;
	free(split_fname), split_fname = NULL;
	fclose(fp_orig);

	return 0;
}

int MfcFile::split(const std::string &srcFile, std::list<std::string> &fileList)
{
	char dest_file[256] = {0};

	unsigned long file_size = 100*1024*1024; // 100M
	unsigned long FILE_SIZE = file_size;

	snprintf(dest_file,256,"%s.dst",srcFile.c_str());


	FILE* src_file = fopen(srcFile.c_str(),"rb");
	if(src_file == NULL)
	{
		fprintf(stderr, "open %s file error!\n",srcFile.c_str());
		return -1;
	}

	char* data_block = new char[file_size];
	FILE* dst_file = NULL;
	char split_file[256];
	unsigned int count = 0;

	//write the same size file
	while((file_size = fread(data_block,1,FILE_SIZE,src_file)) == FILE_SIZE)
	{
		snprintf(split_file,256,"%s%04d",dest_file,count++);
		fileList.push_back(std::string(split_file));
		dst_file = fopen(split_file,"wb");
		if(dst_file == NULL)
		{
			fprintf(stderr, "open %s error!\n",split_file);
			return -1;
		}
		fwrite(data_block,1,file_size,dst_file);
		fclose(dst_file);
	}

	//write the last block data 
	snprintf(split_file,256,"%s%04d",dest_file,count);
	fileList.push_back(std::string(split_file));
	dst_file = fopen(split_file,"wb");
	if(dst_file == NULL)
	{
		fprintf(stderr, "open %s error!\n",split_file);
		return -1;
	}
	fwrite(data_block,1,file_size,dst_file);
	fclose(dst_file);

	delete[] data_block;
	//close the source file
	fclose(src_file);

	return 0;
}
