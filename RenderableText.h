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

class RenderableText : Renderable {
public:
    RenderableText(const std::string& text, int font_siz, const SDL_Color &color, bool mono);
    ~RenderableText();
    void render(SDL_Renderer *renderer) override;
    [[nodiscard]] int x() const override { return m_rect.x; };
    [[nodiscard]] int y() const override { return m_rect.y; };
    [[nodiscard]] int w() const override { return m_rect.w; };
    [[nodiscard]] int h() const override { return m_rect.h; };
    void set_pos(int x, int y) override;
private:
    TTF_Font *m_font;
    SDL_Texture *m_message;
    SDL_Surface *m_text;
    SDL_Rect m_rect;
};



#endif //RENDERABLETEXT_H
