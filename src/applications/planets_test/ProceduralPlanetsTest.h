#ifndef PROCEDURALPLANETSTEST_H
#define PROCEDURALPLANETSTEST_H

#include "application/Application.h"
#include "gui/GuiEventListener.h"

#include "scenegraph/SGScenegraph.h"
#include "scenegraph/SGGraphicsManager.h"
namespace sg{
class SGProcPlaneMesh;
}
class ProceduralPlanetsTest: public Application
{
public:
    ProceduralPlanetsTest(uint32_t argc, const char ** argv);
    virtual ~ProceduralPlanetsTest();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool Update();
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
private:
    sg::SGProcPlaneMesh *mesha;
protected:
};

#endif // PROCEDURALPLANETSTEST_H
