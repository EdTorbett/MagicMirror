//
// Created by Ed on 28/11/2023.
//

#include "RenderableText.h"

#include <iostream>
#include <utility>

char default_font[] = "../fonts/Poppins-Regular.ttf";
char mono_font[] = "../fonts/RobotoMono-Regular.ttf";


void text_init() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
}

RenderableText::RenderableText(const std::string& text, int font_size, const SDL_Color &color, bool mono) :
m_font(nullptr),
m_message(nullptr),
m_text(nullptr),
m_rect({0, 0, 0, 0}) {
    m_font = TTF_OpenFont(mono ? mono_font : default_font, font_size);
    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(m_font, 0);
    TTF_SetFontKerning(m_font, 1);
    TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);

    m_text = TTF_RenderUTF8_Blended_Wrapped(m_font, text.c_str(), color, 400);
    m_rect.w = m_text->w;
    m_rect.h = m_text->h;
}

RenderableText::~RenderableText() {
    if (m_message != nullptr) {
        SDL_DestroyTexture(m_message);
        m_message = nullptr;
    }
    if (m_text != nullptr) {
        SDL_FreeSurface(m_text);
        m_text = nullptr;
    }
    if (m_font != nullptr) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
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
