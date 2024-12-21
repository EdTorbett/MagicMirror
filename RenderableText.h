//
// Created by Ed on 28/11/2023.
//

#ifndef RENDERABLETEXT_H
#define RENDERABLETEXT_H

#include "Renderable.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

void text_init();

typedef enum FontType {
    FONTTYPE_REGULAR = 0,
    FONTTYPE_MONO,
    FONTTYPE_SYMBOL
} FontType;

typedef enum TextAlignment {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT
} TextAlignment;

class RenderableText : public Renderable {
public:
    RenderableText(std::string text, int font_size, const SDL_Color &color, FontType type, TextAlignment alignment = ALIGN_LEFT);
    ~RenderableText() override;
    void render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) override;
    [[nodiscard]] int w() const override { return m_rect.w; };
    [[nodiscard]] int h() const override { return m_rect.h; };
    void set_text(const std::string &text);
    void set_color(const SDL_Color &color);
private:
    void update();
    std::string m_raw_text;
    TTF_Font *m_font;
    SDL_Color m_color;
    SDL_Texture *m_message;
    SDL_Surface *m_text;
    SDL_Rect m_rect;
    TextAlignment m_alignment;
};



#endif //RENDERABLETEXT_H
