#include "precomp.h"
#include "ibuffer_object.h"

ibuffer_object::ibuffer_object(): id(0), usage_hint(STATIC)
{

}

ibuffer_object::~ibuffer_object()
{
    if(id!=0)
    {
        glDeleteBuffers(1,&id);
    }
}
