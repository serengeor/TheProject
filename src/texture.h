#pragma once
#include "GLXW/glxw.h"
#include "glm.hpp"
#include "bit.h"
#include <string>

struct image
{
    std::string name;
    glm::vec2 size;
    uint32_t bpp; /// >bits< per pixel
};

struct texture
{
	std::string name;
	uint32_t obj;
	uint32_t type;

	static uint32_t bound_tex, active_slot;

	texture()
	{
        obj = 0;
        type = 0;
	}

	/**
	 * @brief
	 * @param clamp
	 * bit0 = s to edge, bit1 = t to edge
	 * @param filter
	 * bit0 = linear, bit1 = mipmap
	 */
	void generate(const uint8_t * data, uint32_t target, uint8_t bpp, int32_t w, int32_t h, uint8_t filter=bit<0>(), uint8_t clamp=bit<0>()|bit<1>())
	{
		type = target;

		glGenTextures(1, &obj);
		glBindTexture(type,obj);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, tbit(clamp,BIT0) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, tbit(clamp,BIT1) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, tbit(filter,BIT1) ? GL_LINEAR_MIPMAP_LINEAR : (tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST));

		/// either use texture with or without alpha channel
		if(bpp!=32) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		else glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glTexImage2D(type,0,GL_RGBA,w,h,0,bpp==32?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,data);

		if(tbit(filter,BIT1)) glGenerateMipmap(type);

		glBindTexture(type,bound_tex);
	}

	void set_filters(uint8_t filter=bit<0>())
	{
	    glBindTexture(type,obj);

        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, tbit(filter,BIT1) ? GL_LINEAR_MIPMAP_LINEAR : (tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST));

		glBindTexture(type,bound_tex);
	}

	void set_clamp(uint8_t clamp=bit<0>()|bit<1>())
	{
	    glBindTexture(type,obj);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, tbit(clamp,BIT0) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, tbit(clamp,BIT1) ? GL_CLAMP_TO_EDGE : GL_REPEAT);

		glBindTexture(type,bound_tex);
	}

	void set(uint8_t slot)
	{
		glActiveTexture(GL_TEXTURE0+slot);
		glBindTexture(type,obj);

		active_slot = slot;
		bound_tex = obj;
	}

	void free()
	{
		glDeleteTextures(1,&obj);
	}

};
