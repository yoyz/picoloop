#include "Logger.h"
#include <iostream>

void StdOutLogger::AddLine(const char *line)
{
	std::cout << line << std::endl ;
}

// ----------------------------------------------
/*
FileLogger::FileLogger(const Path &path)
:path_(path)
,file_(0)
*/
//FileLogger::FileLogger(const Path &path)
FileLogger::FileLogger()
{
}

FileLogger::~FileLogger()
{
  /*
  if (file_)
  {
    fclose(file_);
  }
  */
}

Result FileLogger::Init()
{
  /*
  file_= fopen(path_.GetPath().c_str(),"w") ;
  if (!file_)
  {
    return Result("Failed to open log file");
  }
  fclose(file_);
  return Result::NoError;
  */
  return Result::NoError;
}

void FileLogger::AddLine(const char *line)
{
  printf("LGPT_LOGGER:%s\n",line);
  //file_= fopen(path_.GetPath().c_str(),"a") ;
  //fprintf(file_,"%s\n",line) ;
  //fclose(file_);
}
