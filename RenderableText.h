//
// Created by Ed on 28/11/2023.
//

#ifndef RENDERABLETEXT_H
#define RENDERABLETEXT_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

void text_init();

class RenderableText {
public:
    RenderableText(const std::string& text, float font_size);
    void render(SDL_Renderer *renderer);
    [[nodiscard]] int x() const { return m_rect.x; };
    [[nodiscard]] int y() const { return m_rect.y; };
    [[nodiscard]] int w() const { return m_rect.w; };
    [[nodiscard]] int h() const { return m_rect.h; };
    void set_pos(int x, int y);
private:
    TTF_Font *m_font;
    SDL_Texture *m_message;
    SDL_Surface *m_text;
    SDL_Rect m_rect;
};



#endif //RENDERABLETEXT_H
