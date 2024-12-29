//
// Created by Ed on 28/12/2024.
//

#ifndef RENDERABLEVIDEO_H
#define RENDERABLEVIDEO_H

#include "Renderable.h"
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

class RenderableVideo : public Renderable {
public:
    explicit RenderableVideo();
    ~RenderableVideo() override;
    void render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) override;
    void update_texture(const void *data, int width, int height, unsigned int size, unsigned int max_size);
    [[nodiscard]] int w() const override { return m_rect.w; }

    void set_finished();
    [[nodiscard]] int h() const override { return m_rect.h; };
    void initialise(const std::string &url);
    void teardown();
    [[nodiscard]] bool finished() const { return m_finished;}
private:
    uint8_t *m_videoBuffer;
    unsigned int m_bufSize{};
    SDL_Texture *m_videoFrame;
    SDL_Rect m_rect;
    GstElement *m_pipeline;
    GstAppSink *m_video_sink{};
    bool m_finished;
    std::chrono::time_point<std::chrono::steady_clock> m_last_buffer_update_time;
};



#endif //RENDERABLEVIDEO_H
