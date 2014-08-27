#ifndef CHUNK_H
#define CHUNK_H

#include "VoxelMesh.h"
#include "Block.h"

class Chunk:public VoxelMesh
{
public:
    Chunk();
    ~Chunk();

    void SetupSphere();
    void Generate();

    void Update(float dt);

    static const int CHUNK_SIZE = 16;
    void method(glm::vec4 shittest);
    void Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
private:
    // The blocks data
    Block*** m_pBlocks;
};
#endif // CHUNK_H