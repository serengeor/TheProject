#include "Precomp.h"
#include "VoxelOctreeBenchmarkApp.h"
#include "motree/VoxMeshManager.h"
#include "utility/SimplexNoise.h"
#include "utility/Timer.h"
#include "utility/Logger.h"
#include "application/SettingsManager.h"


VoxelOctreeBenchmarkApp::VoxelOctreeBenchmarkApp(uint32_t argc, const char ** argv): Application(argc,argv)
{
    std::cout << "Starting benchmark" << std::endl;   
}

VoxelOctreeBenchmarkApp::~VoxelOctreeBenchmarkApp()
{

}

void VoxelOctreeBenchmarkApp::InitOctree()
{
    cam=share(new Camera(glm::vec3(0,0,-5),glm::vec3(0,0,5),glm::vec3(0,1,0)));

    mesh = MeshPtr(new Mesh());
    IndexBufferObject<uint32_t> * ibo = new IndexBufferObject<uint32_t>();
    BufferObject<glm::vec3> *vbo = new BufferObject<glm::vec3>();
    BufferObject<glm::vec3> *cbo = new BufferObject<glm::vec3>();
    mesh->buffers[Mesh::POSITION] = vbo;
    mesh->buffers[Mesh::INDICES] = ibo;
    mesh->buffers[Mesh::COLOR] = cbo;
    mesh->Init();

    octree = share(new MortonOctTree());
    octreeGen = new VoxMeshManager(octree);
}

#define START(x) timer->tick(); x = timer->get_real_time()
#define END(x) timer->tick(); x = timer->get_real_time() - x
#define DEBUG_PRINT(x) GetContext().GetLogger()->log(LOG_LOG, "%s took %ums time", #x, x)
void VoxelOctreeBenchmarkApp::BuildOctree()
{
    uint32_t time, addTime, sortTime, rebuildTime, generationTime;


    uint32_t x, y, z;

    x=10;
    y=10;
    z=10;

    std::cout << "Testing area dimensions: [" << x << ", " << y << ", " << z << "]" << std::endl;

    START(addTime);
        octree->GetChildNodes().reserve(x*y*z);
        loop(i,z)
            loop(j,y)
                loop(k,x)
                    octree->AddOrphanNode(MNode(k,j,i));
    END(addTime);

    START(sortTime);
        octree->SortLeafNodes();
    END(sortTime);

    START(generationTime);
        octreeGen->GenAllChunks();
    END(generationTime);

    DEBUG_PRINT(addTime);
    DEBUG_PRINT(sortTime);
    DEBUG_PRINT(generationTime);
}

#undef START
#undef END
#undef DEBUG_PRINT

void VoxelOctreeBenchmarkApp::FreeOctree()
{
    octree = nullptr;
    delete octreeGen;
    mesh = MeshPtr();
}

void TestNodes(Logger * _logger)
{
    MNode node;
    _logger->log(LOG_LOG, "================ NODE TEST START ==================");

    loopxyz(3,2,2)
        _logger->log(LOG_LOG, "[%u, %u, %u] Edge [%u, %u]", x, y, z, (uint32_t)node.getEdgeStart(x,y,z), (uint32_t)node.getEdgeEnd(x,y,z));

    _logger->log(LOG_LOG, "================ NODE TEST END ====================");
}

bool VoxelOctreeBenchmarkApp::Init()
{
    if(!InitSimple("Octree benchmark app"))
        return false;
    
    timer = GetContext().GetTimer();

    uint32_t x, y, z;
    VarGroup & benchmark = GetContext().GetApplicationSettingsManager()->GetGroup("benchmark");

    x=benchmark.GetVar("x").ValueI();
    y=benchmark.GetVar("y").ValueI();
    z=benchmark.GetVar("z").ValueI();

    std::cout << "Testing area dimensions: [" << x << ", " << y << ", " << z << "]" << std::endl;

    /*InitOctree();
    BuildOctree();
    FreeOctree();*/

    AABB box(glm::vec3(10,10,10),glm::vec3(0.5,0.5,0.5));
    AABB box2(glm::vec3(8,10,10),glm::vec3(0.5,0.5,0.5));

    glm::vec3 nout;
    float time;
    time = box2.SweepCollidesWith(box, glm::vec3(2,0.1,0), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    box2.Translate(glm::vec3(4,0,0));
    time = box2.SweepCollidesWith(box, glm::vec3(-2,0.1,0), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    box2.SetCenter(glm::vec3(10,8,10));
    time = box2.SweepCollidesWith(box, glm::vec3(0.1,3,0), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    box2.SetCenter(glm::vec3(10,12,10));
    time = box2.SweepCollidesWith(box, glm::vec3(0.1,-3,0), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    box2.SetCenter(glm::vec3(10,10,8));
    time = box2.SweepCollidesWith(box, glm::vec3(0.1,0.3,3), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    box2.SetCenter(glm::vec3(10,10,12));
    time = box2.SweepCollidesWith(box, glm::vec3(0.1,-0.3,-3), nout);
    GetContext().GetLogger()->log(LOG_LOG, "Collision time: %f", time);
    GetContext().GetLogger()->log(LOG_LOG, "Collision normal: [%.3f,%.3f,%.3f]", nout.x, nout.y, nout.z);

    TestNodes(GetContext().GetLogger());

    return true;
}

bool VoxelOctreeBenchmarkApp::Update()
{
    return false;
}

bool VoxelOctreeBenchmarkApp::Exit()
{
    return true;
}

void VoxelOctreeBenchmarkApp::OnWindowClose()
{

}
void VoxelOctreeBenchmarkApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void VoxelOctreeBenchmarkApp::OnMouseMove(double x, double y)
{
}

void VoxelOctreeBenchmarkApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{

}

std::string VoxelOctreeBenchmarkApp::GetApplicationId()
{
    return "voxel_octree_benchmark";
}

void VoxelOctreeBenchmarkApp::InitVariables()
{
/*
    VarGroup & becnhGroup = this->AddGroup("benchmark");
    becnhGroup.AddVar(Var("x", (int)10));
    becnhGroup.AddVar(Var("y", (int)10));
    becnhGroup.AddVar(Var("z", (int)10));
*/
}