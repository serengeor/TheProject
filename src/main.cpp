#include "application.h"
#include "test_application.h"
#include "timer.h"

int main(int argc, const char ** argv)
{
    application * app = new test_application(argc,argv);


    if(app->init("test",1024,720))
    {

        while(app->update())
            std::cout << "real time:" << app->get_timer()->get_time() << std::endl;

        app->exit();
        delete app;
    }

    return 0;
}
