/************************************************/
/* Application.h, (c) Rene Puchinger            */
/*                                              */
/* sample artihmetic compression program        */
/************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "SimpleBitIOStream.h"
#include "FileIOStream.h"
#include "ARI.h"

class Application {
	enum { ENCODE, DECODE, IDLE } state;
	ARI_encoder* enc;
	ARI_decoder* dec;
	char* fn_in;
	char* fn_out;
public:
	Application(int argc, char** argv);
	~Application();
	void run();
};

#endif
