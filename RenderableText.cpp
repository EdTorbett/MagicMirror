//
// Created by Ed on 28/11/2023.
//

#include "RenderableText.h"

#include <iostream>
#include <utility>

std::string font_types[] = {
    std::string("fonts/Poppins-Regular.ttf"),
    std::string("fonts/RobotoMono-Regular.ttf"),
    std::string("fonts/ionicons.ttf")
};

void text_init() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init(): " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }
}

RenderableText::RenderableText(std::string text, const int font_size, const SDL_Color &color, const FontType type, const TextAlignment alignment) :
m_raw_text(std::move(text)),
m_font(nullptr),
m_color(color),
m_message(nullptr),
m_text(nullptr),
m_rect({0, 0, 0, 0}),
m_alignment(alignment) {
    m_font = TTF_OpenFont(font_types[type].c_str(), font_size);
    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(m_font, 0);
    TTF_SetFontKerning(m_font, 1);
    TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);

    update();
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

void RenderableText::set_color(const SDL_Color &color) {
    m_color = color;
    update();
}


void RenderableText::set_text(const std::string &text) {
    m_raw_text = text;
    update();
}

void RenderableText::update() {
    if (m_message != nullptr) {
        SDL_DestroyTexture(m_message);
        m_message = nullptr;
    }
    if (m_text != nullptr) {
        SDL_FreeSurface(m_text);
        m_text = nullptr;
    }

    m_text = TTF_RenderUTF8_Blended(m_font, m_raw_text.c_str(), m_color);
    if (m_text != nullptr) {
        m_rect.w = m_text->w;
        m_rect.h = m_text->h;
    } else {
        m_rect.w = 0;
        m_rect.h = 0;
    }
    if (m_alignment == 1) {
        m_rect.x = -(m_rect.w >> 1);
    } else if (m_alignment == 2) {
        m_rect.x = -m_rect.w;
    } else {
        m_rect.x = 0;
    }
}

void RenderableText::render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) {
    if (skip_render(now)) {
        return;
    }
    if (m_message == nullptr && m_text != nullptr) {
        m_message = SDL_CreateTextureFromSurface(renderer, m_text);
    }

    if (m_message != nullptr) {
        SDL_Rect rect = m_rect;
        rect.x += x();
        rect.y += y();

        const SDL_Point texture_rotation = { rect.x, rect.y};
        const SDL_Point rotationCenter = { 540 - texture_rotation.x, 540 - texture_rotation.y };

        SDL_SetTextureAlphaMod(m_message, static_cast<uint8_t>(static_cast<float>(m_color.a) * get_brightness(now)));
        SDL_RenderCopyEx(renderer, m_message, nullptr, &rect, -90, &rotationCenter, SDL_FLIP_NONE);
        SDL_SetTextureAlphaMod(m_message, 0xFF);
    }
    Renderable::render(renderer, now);
}