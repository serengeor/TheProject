#include "precomp.h"
#include "ProceduralPlanetsTest.h"
#include "application/window.h"
#include "scenegraph/SGMeshObject.h"
#include "scenegraph/SGScenegraphLoader.h"
#include "scenegraph/SGProcPlaneMesh.h"

ProceduralPlanetsTest::ProceduralPlanetsTest(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

ProceduralPlanetsTest::~ProceduralPlanetsTest()
{

}

bool ProceduralPlanetsTest::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&ProceduralPlanetsTest::OnKeyEvent));

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.2,1,0.2,0);

    auto sg = _appContext->_scenegraph;
	// cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(_appContext,glm::vec3(0,0,-10),glm::vec3(0,0,10),glm::vec3(0,1,0)));

    sg::ScenegraphLoader * sg_loader = new sg::ScenegraphLoader();
    sg_loader->LoadScene(_appContext,"res/material_test/untitled.ibs",true);

    sg::SGProcPlaneMesh* mesha=new sg::SGProcPlaneMesh(sg);

    sg::sg_quad* meshb=new sg::sg_quad(sg);

    _appContext->_timer->tick();
    return true;
}

bool ProceduralPlanetsTest::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        _appContext->_timer->tick();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _appContext->_physicsManager->update((float)_appContext->_timer->get_delta_time()/1000.0f);

        _appContext->_scenegraph->update_all((float)_appContext->_timer->get_delta_time()/1000.0f);
        mesha->Render(_appContext->_scenegraph);
        _appContext->_scenegraph->render_all();

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void ProceduralPlanetsTest::Exit()
{
    Application::Exit();
}

void ProceduralPlanetsTest::OnWindowClose()
{

}

void ProceduralPlanetsTest::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    sg::SGScenegraph * scenegraph = _appContext->_scenegraph;
    sg::sg_camera_object * cam = scenegraph->get_active_camera().get();

    if(!cam) return;

    if(key==GLFW_KEY_W)
        cam->walk(1);
    else if(key==GLFW_KEY_S)
        cam->walk(-1);
    if(key==GLFW_KEY_A)
        cam->strafe(-1);
    if(key==GLFW_KEY_D)
        cam->strafe(1);
}
