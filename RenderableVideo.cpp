//
// Created by Ed on 28/12/2024.
//

#include "RenderableVideo.h"
#include <iostream>
#include <gst/gstsample.h>


/* Callback: The appsink has received a buffer */
static GstFlowReturn new_buffer (GstAppSink *app_sink, gpointer data) {
    auto *renderable_video = static_cast<RenderableVideo *>(data);

    // Get the frame from video stream
    GstSample *sample = gst_app_sink_pull_sample(app_sink);

    if(!sample) {
        // Finished playing.
        renderable_video->set_finished();
        return GST_FLOW_ERROR;
    }

    // Get the frame format.
    GstCaps* caps = gst_sample_get_caps(sample);
    if (!caps) {
        g_print ("could not get snapshot format\n");
        exit (-1);
    }

    GstBuffer *buffer = gst_sample_get_buffer(sample);

    // Extract the width and height of the frame
    gint width, height;
    const GstStructure* s = gst_caps_get_structure (caps, 0);
    int res = gst_structure_get_int (s, "width", &width)
        | gst_structure_get_int (s, "height", &height);
    if (!res) {
        g_print ("could not get snapshot dimension\n");
        exit (-1);
    }

    GstMapInfo info;
    gst_buffer_map (buffer, &info, GST_MAP_READ);

    renderable_video->update_texture(info.data, width, height, info.size, info.maxsize);

    gst_buffer_unmap(buffer, &info);
    gst_sample_unref(sample);

    return GST_FLOW_OK;
}

RenderableVideo::RenderableVideo() :
    m_videoBuffer(nullptr),
    m_videoFrame(nullptr),
    m_rect{},
    m_pipeline(nullptr),
    m_video_sink(nullptr),
    m_finished(true) {
    set_ramp_time(500);
}

RenderableVideo::~RenderableVideo() {
    if (m_videoFrame != nullptr) {
        SDL_DestroyTexture(m_videoFrame);
        m_videoFrame = nullptr;
    }
    delete[] m_videoBuffer;
}

void RenderableVideo::update_texture(const void *data, const int width, const int height, const unsigned int size, const unsigned int max_size) {
    // Update the texture with the received data.
    if (m_videoBuffer != nullptr && (m_rect.w != width || m_rect.h != height)) {
        std::cout << "Deleting incorrectly sized buffer" << std::endl;
        delete m_videoBuffer;
    }

    if (m_videoBuffer == nullptr) {
        std::cout << "Creating buffer with dimensions: w=" << width << ", h=" << height << ", size=" << size << ", max_size=" << max_size << std::endl;
        m_videoBuffer = new uint8_t[max_size];
        m_rect.w = width;
        m_rect.h = height;
        set_visible(true, std::chrono::steady_clock::now());
    }

    m_last_buffer_update_time = std::chrono::steady_clock::now();
    m_bufSize = size;
    memcpy(m_videoBuffer, data, size);
}

void RenderableVideo::set_finished() {
    if (!m_finished) {
        std::cout << "Video has finished playing" << std::endl;
        m_finished = true;
    }
}

void RenderableVideo::initialise(const std::string &url) {
    std::cout << "Initialising video renderer with url: " << url << std::endl;
    m_last_buffer_update_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    m_finished = false;
    m_pipeline = gst_parse_launch(("playbin uri=" + url).c_str(), nullptr);

    const auto bin = gst_bin_new ("video_sink_bin");
    const auto convert = gst_element_factory_make ("v4l2convert", "video_convert");
    const auto filter = gst_element_factory_make ("capsfilter", "video_filter");
    const auto video_sink = gst_element_factory_make ("appsink", "video_sink");

    const auto filter_caps = gst_caps_new_simple ("video/x-raw",
                 "format", G_TYPE_STRING, "YUY2",
                 NULL);
    g_object_set (G_OBJECT(filter), "caps", filter_caps, NULL);
    gst_caps_unref (filter_caps);

    m_video_sink = GST_APP_SINK(video_sink);
    GstAppSinkCallbacks callbacks {NULL, NULL, new_buffer, NULL, 0};
    gst_app_sink_set_callbacks(m_video_sink, &callbacks, static_cast<void*>(this), NULL);

    gst_bin_add_many (GST_BIN(bin), convert, filter, video_sink, NULL);
    gst_element_link_many (convert, filter, video_sink, NULL);
    auto pad = gst_element_get_static_pad (convert, "sink");
    auto ghost_pad = gst_ghost_pad_new ("sink", pad);
    gst_pad_set_active (ghost_pad, TRUE);
    gst_element_add_pad (bin, ghost_pad);
    gst_object_unref (pad);

    g_object_set (GST_OBJECT (m_pipeline), "video-sink", bin, NULL);
    gst_element_set_state (m_pipeline, GST_STATE_PLAYING);
}

void RenderableVideo::teardown() {
    if (m_pipeline != nullptr) {
        set_visible(false, std::chrono::steady_clock::now());
        delete[] m_videoBuffer;
        m_videoBuffer = nullptr;
        std::cout << "Tearing down video player pipeline" << std::endl;
        gst_element_set_state (m_pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipeline);
        m_pipeline = nullptr;
        m_video_sink = nullptr;
    }
}

void RenderableVideo::render(SDL_Renderer *renderer, const std::chrono::time_point<std::chrono::steady_clock> &now) {
    if (visible() && !m_finished && (now - m_last_buffer_update_time) > std::chrono::milliseconds(200)) {
        set_finished();
    }

    if (skip_render(now)) {
        if (m_videoFrame != nullptr) {
            std::cout << "Destroying video texture" << std::endl;
            SDL_DestroyTexture(m_videoFrame);
            m_videoFrame = nullptr;
        }
        return;
    }

    if (!m_finished && m_videoFrame == nullptr && m_videoBuffer != nullptr && m_rect.w > 0 && m_rect.h > 0) {
        std::cout << "Creating video texture " << std::endl;
        m_videoFrame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YUY2, SDL_TEXTUREACCESS_STREAMING, m_rect.w, m_rect.h);
        SDL_SetTextureBlendMode(m_videoFrame, SDL_BLENDMODE_BLEND);
    }

    if (m_videoFrame != nullptr) {
        SDL_Rect rect = m_rect;
        if (!m_finished && m_videoBuffer != nullptr && m_bufSize > 0) {
            SDL_UpdateTexture(m_videoFrame, &rect, m_videoBuffer, GST_ROUND_UP_4(m_rect.w * 2));
        }
        rect.x += x() - m_rect.w / 2;
        rect.y += y() - m_rect.h / 2;

        const SDL_Point texture_rotation = { rect.x, rect.y};
        const SDL_Point rotationCenter = { 540 - texture_rotation.x, 540 - texture_rotation.y };

        SDL_SetTextureAlphaMod(m_videoFrame, static_cast<uint8_t>(255.f * get_brightness(now)));
        SDL_RenderCopyEx(renderer, m_videoFrame, nullptr, &rect, -90, &rotationCenter, SDL_FLIP_NONE);
    }
    Renderable::render(renderer, now);
}