#ifndef VOXELOCTREEAPP_H
#define VOXELOCTREEAPP_H

///REFACTOR: Application currently is responsible for too many things.
#include "application/Application.h"
#include "AppForwardDecl.h"

class VoxelOctreeApp: public Application
{
private:
    static VoxelOctreeApp * m_instance;
public:
    static VoxelOctreeApp * Instance();

    VoxelOctreeApp(uint32_t argc, const char ** argv);
    virtual ~VoxelOctreeApp();

    virtual bool Init();
    virtual bool Update();
    virtual bool Exit();

    bool LoadLevel(const std::string & levelName);
    bool SaveLevel(const std::string & levelName);

    void InitResources();
    void AfterInit();
    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);
    virtual std::string GetApplicationId();

    ///Octree
    MortonOctTreePtr GetOctree();
    void ClearOctree();
    void GenerateOctreeMeshes();

    ///Script
    void RunScript(const Path & script);

    ///This should probably go to "GameManagerClass"
    void SetPlayerPosition(float x, float y, float z);
private:

    void RenderScene();
    void RenderGrid();
    
private:
    ShaderPtr sh, gridShader;
    CameraPtr cam;
    MortonOctTreePtr octree;
    BVoxLoaderPtr bvoxLoader;
    CollisionManager * collisionManager;
    VoxMeshManager * octreeGen;
    CubeMesh * cube;
    Player * player;
    RendererPtr renderer;
};


#endif // VOXELOCTREEAPP_H
