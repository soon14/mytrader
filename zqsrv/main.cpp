#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <XUtil/XXml.hpp>
#include "zqsrv.h"

int main(int argc, char* argv[])
{
	zqdb::Server::Inst().init(argc, argv);
	//getchar();
	zqdb::Server::Inst().run();
	zqdb::Server::Inst().term();
	return 0;
}

