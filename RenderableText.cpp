//
// Created by Ed on 28/11/2023.
//

#include "RenderableText.h"

#include <iostream>

std::string font_types[] = {
    std::string("../fonts/Poppins-Regular.ttf"),
    std::string("../fonts/RobotoMono-Regular.ttf"),
    std::string("../fonts/ionicons.ttf")
};

void text_init() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
}

RenderableText::RenderableText(const std::string& text, int font_size, const SDL_Color &color, FontType type, TextAlignment alignment) :
m_font(nullptr),
m_color(color),
m_message(nullptr),
m_text(nullptr),
m_alignment(alignment),
m_rect({0, 0, 0, 0}) {
    m_font = TTF_OpenFont(font_types[type].c_str(), font_size);
    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(m_font, 0);
    TTF_SetFontKerning(m_font, 1);
    TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);
    m_text = TTF_RenderUTF8_Blended(m_font, text.c_str(), m_color);
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

void RenderableText::set_text(const std::string &text) {
    if (m_message != nullptr) {
        SDL_DestroyTexture(m_message);
        m_message = nullptr;
    }
    if (m_text != nullptr) {
        SDL_FreeSurface(m_text);
        m_text = nullptr;
    }
    m_text = TTF_RenderUTF8_Blended(m_font, text.c_str(), m_color);

    const int orig_w = m_rect.w;
    m_rect.w = m_text->w;
    m_rect.h = m_text->h;
    if (m_alignment == 1) {
        m_rect.x += (orig_w >> 1) - (m_text->w >> 1);
    } else if (m_alignment == 2) {
        m_rect.x += orig_w - m_text->w;
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
    if (m_alignment == 1) {
        m_rect.x -= m_text->w >> 1;
    } else if (m_alignment == 2) {
        m_rect.x -= m_text->w;
    }

    m_rect.y = y;
}
