#include "Precomp.h"

#include "utility/Util.h"
#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEvent.h"

#include "GUIEnvironment.h"

#include "GUISlider.h"

gui_slider::gui_slider(GUIEnvironment* env, Rect2D<int> dimensions, float min, float max, float pos, bool vertical):GUIElement(env,dimensions)
{
    this->Type=GUIET_SLIDER;
    environment=env;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_min=min;
    m_max=max;
    m_cur_value=m_old_value=pos;
    if(vertical)
    {
        m_slider_pos=absolute_rect.y-((pos-absolute_rect.h)/(max-min));
    }
    else
    {
        float f=(absolute_rect.w-12)/range();
        m_slider_pos=(m_cur_value-m_min)*f+absolute_rect.h/2;
    }
    m_vertical=vertical;

    this->SetParent(env);
}

gui_slider::~gui_slider()
{
}

void gui_slider::Render()
{
    if(m_vertical)
    {
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x+absolute_rect.w/4,absolute_rect.y,absolute_rect.w/2,absolute_rect.h),gui_skin_progress_h_bg,true);
        environment->draw_gui_quad(Rect2D<int>(absolute_rect.x,absolute_rect.y+m_slider_pos-6,absolute_rect.w,12),gui_skin_progress_h_bar,true);
    }
    else
    {
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x,absolute_rect.y+absolute_rect.h/4,absolute_rect.w,absolute_rect.h/2),gui_skin_scroll_h_bg,true);
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x+m_slider_pos-6,absolute_rect.y,12,absolute_rect.h),gui_skin_scroll_h_bar,true);
    }

    this->RenderChildren();
}

bool gui_slider::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.GetType())
    {
    case mouse_pressed:
        handle_mouse();

        if(m_cur_value!=m_old_value)
        {
            GUI_FIRE_EVENT(GUIEvent(scrollbar_changed,this,this))
            m_old_value=m_cur_value;
        }

        break;
    case mouse_dragged:
        handle_mouse();

        if(m_cur_value!=m_old_value)
        {
            GUI_FIRE_EVENT(GUIEvent(scrollbar_changed,this,this))
            m_old_value=m_cur_value;
        }
        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void gui_slider::handle_mouse()
{
    if(m_vertical)
    {
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.y - absolute_rect.y);

        if (m_slider_pos >= absolute_rect.h)
            m_slider_pos = absolute_rect.h;
        if (m_slider_pos < 0)
            m_slider_pos = 0;


        m_cur_value = ((m_max - m_min) * (absolute_rect.y-m_slider_pos)) / absolute_rect.h;
        m_cur_value = glm::clamp(m_cur_value,m_min,m_max);
    }
    else
    {
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.x - absolute_rect.x);

        if (m_slider_pos >= absolute_rect.w)
            m_slider_pos = absolute_rect.w;
        if (m_slider_pos < 0)
            m_slider_pos = 0;

        //m_cur_value = ((m_max - m_min) * m_slider_pos) / absolute_rect.w;
        m_cur_value =  m_slider_pos/absolute_rect.w * range() + m_min;
        m_cur_value = glm::clamp(m_cur_value,m_min,m_max);
    }
    //printf("value: %f\n",m_cur_value);
}

float gui_slider::get_value() const
{
    return m_cur_value;
}

void gui_slider::set_value(float value)
{
    m_cur_value = clamp(value,m_min,m_max);
}