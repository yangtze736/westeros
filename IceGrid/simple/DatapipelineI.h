// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************

#ifndef DATAPIPELINE_I_H
#define DATAPIPELINE_I_H

#include <Datapipeline.h>

class DatapipelineI : public Demo::Datapipeline
{
public:

    DatapipelineI(const std::string&);

    virtual std::string fileTransfer(const std::string& s, const Ice::Current&);

private:

	// Required to prevent compiler warnings with MSVC++
	DatapipelineI& operator=(const DatapipelineI&);

    const std::string _name;
};

#endif
