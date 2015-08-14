// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <DatapipelineI.h>
#include "middleware.h"

using namespace std;

DatapipelineI::DatapipelineI(const string& name) : _name(name)
{
}

string
DatapipelineI::fileTransfer(const string& s, const Ice::Current&)
{
	// TODO
	cout << s << endl;
    cout << _name << " says Hello World!" << endl;
#if 0
	cout << "--- Test Middleware ---" << endl;
	MiddleWare *middleware = new MiddleWare;
	string responseStr;
	middleware->data_pipeline("networkRegister", string("xxxx"), responseStr);
	cout << "--- End of Middleware ---" << endl;
#endif
	return _name + " says Hello World!";
	//return responseStr;
}

