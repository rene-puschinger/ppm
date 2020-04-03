/************************************************/
/* main.cpp, (c) Rene Puchinger                 */
/************************************************/

#include "Application.h"

int main(int argc, char** argv) {
	Application *app = new Application(argc, argv);
	app->run();
	delete app;
	return 0;
}
