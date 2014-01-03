#include "precomp.h"
#include "isg_object.h"

namespace sg
{

isg_object::isg_object()
{
    //ctor
}

isg_object::~isg_object()
{
    //dtor
}

glm::mat4x4 & isg_object::get_transform()
{
    return m_transform;
}

std::string & isg_object::get_name()
{
    return m_name;
}

}
