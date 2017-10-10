#include "TextBox.h"
#include "Font.h"

void TextBox::dealEvent(BP_Event& e)
{
    if (e.type == BP_MOUSEBUTTONUP || e.type == BP_KEYUP)
    {
        setExit(true);
    }
}

void TextBox::setFontSize(int size)
{
    for (auto c : childs_)
    {
        auto t = dynamic_cast<TextBox*>(c);
        if (t)
        {
            t->setFontSize(size);
        }
    }
    font_size_ = size;
}

void TextBox::setText(std::string text)
{
    text_ = text;
    w_ = font_size_ * text.length() / 2;
    h_ = font_size_;
}

void TextBox::setTexture(const std::string& path, int normal_id, int pass_id /*= -1*/, int press_id /*= -1*/)
{
    if (pass_id < 0) { pass_id = normal_id; }
    if (press_id < 0) { press_id = normal_id; }
    texture_path_ = path;
    texture_normal_id_ = normal_id;
    texture_pass_id_ = pass_id;
    texture_press_id_ = press_id;
}

void TextBox::setTextColor(BP_Color c1, BP_Color c2, BP_Color c3)
{
    color_normal_ = c1;
    color_pass_ = c2;
    color_press_ = c3;
}

void TextBox::draw()
{
    if (!texture_path_.empty())
    {
        TextureManager::getInstance()->renderTexture(texture_path_, texture_normal_id_, x_, y_, { 255, 255, 255, 255 }, 255);
    }
    //实际上仅用了一个颜色，需要有颜色变化请用button
    if (!text_.empty())
    {
        if (have_box_)
        {
            Font::getInstance()->drawWithBox(text_, font_size_, x_ + text_x_, y_ + text_y_, color_normal_, 255);
        }
        else
        {
            Font::getInstance()->draw(text_, font_size_, x_ + text_x_, y_ + text_y_, color_normal_, 255);
        }
    }
}

void TextBoxAutoExit::dealEvent(BP_Event& e)
{
    TextBox::dealEvent(e);
    if (stay_frame_ > 0 && current_frame_ >= stay_frame_)
    {
        setVisible(false);
        setExit(true);
    }
    current_frame_++;
}
