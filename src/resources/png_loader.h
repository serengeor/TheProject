#pragma once

#include "iimage_loader.h"
class logger;
class png_loader: public iimage_loader
{
public:
    png_loader(logger * l);
    virtual ~png_loader();

    virtual image_ptr load(void * buffer, const uint32_t size);
    virtual bool check_by_extension(const std::string & ext);

protected:
    logger * m_logger;
};
