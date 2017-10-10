#pragma once
#include "Element.h"

class TextBox : public Element
{
public:
    TextBox() {}
    virtual ~TextBox() {}
    void dealEvent(BP_Event& e) override;
protected:
    std::string text_ = "";
    int font_size_ = 20;
    int text_x_ = 0, text_y_ = 0;
    Texture* tex_ = nullptr;
    BP_Color color_normal_ = { 32, 32, 32, 255 };
    BP_Color color_pass_ = { 255, 255, 255, 255 };
    BP_Color color_press_ = { 255, 0, 0, 255 };
    bool have_box_ = true;

    std::string texture_path_ = "";
    int texture_normal_id_ = -1, texture_pass_id_ = -1, texture_press_id_ = -1; //三种状态的按钮图片
public:
    void setTexture(const std::string& path, int normal_id, int pass_id = -1, int press_id = -1);
    int getNormalTextureID() { return texture_normal_id_; }

    void setFontSize(int size);
    void setText(std::string text);
    void setTexture(Texture* t) { tex_ = t; }
    void setTextPosition(int x, int y) { text_x_ = x; text_y_ = y; }  //注意：这个会导致焦点出现问题，通常是为了实现一些其他效果，请勿任意使用
    void setTextColor(BP_Color c1, BP_Color c2, BP_Color c3);
    void setTextColor(BP_Color c1) { color_normal_ = c1; }

    virtual void draw() override;
    void setHaveBox(bool h) { have_box_ = h; }
};

//可以维持数帧后自动消失的文本框
class TextBoxAutoExit : TextBox
{
public:
    TextBoxAutoExit() {}
    ~TextBoxAutoExit() {}
private:
    int stay_frame_ = -1;
    int current_frame_ = 0;
public:
    void setStayFrame(int s) { stay_frame_ = s; }
    virtual void dealEvent(BP_Event& e) override;
};

