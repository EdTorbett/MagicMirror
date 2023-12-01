//
// Created by Ed on 28/11/2023.
//

#include "RenderableText.h"

#include <iostream>
#include <utility>

char default_font[] = "/home/magicmirror/Documents/freetype-gl/fonts/Vera.ttf";


void text_init() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
}

RenderableText::RenderableText(const std::string& text, float font_size) :
m_font(nullptr),
m_message(nullptr),
m_text(nullptr),
m_rect({0, 0, 0, 0}) {
    SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
    m_font = TTF_OpenFont(default_font, font_size);
    if (m_font == nullptr) {
        std::cerr << "RenderableText(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(m_font, 0);
    TTF_SetFontKerning(m_font, 1);
    TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);

    m_text = TTF_RenderText_Blended(m_font, text.c_str(), white);
    if (m_text == nullptr) {
        std::cerr << "RenderableText(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
    m_rect.w = m_text->w;
    m_rect.h = m_text->h;
}

void RenderableText::render(SDL_Renderer *renderer) {
    if (m_message == nullptr) {
        m_message = SDL_CreateTextureFromSurface(renderer, m_text);
    }
    SDL_RenderCopy(renderer, m_message, nullptr, &m_rect);
}

void RenderableText::set_pos(const int x, const int y) {
    m_rect.x = x;
    m_rect.y = y;
}
