// **********************************************************************
//
// Copyright (c) Sari CloudBackup Group, All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

interface Datapipeline
{
    idempotent string fileTransfer(string s);
};

};

