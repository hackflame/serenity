#pragma once

#include "Music.h"
#include <LibGUI/GWidget.h>

class GPainter;

class PianoWidget final : public GWidget {
    C_OBJECT(PianoWidget)
public:
    virtual ~PianoWidget() override;

    void fill_audio_buffer(uint8_t* stream, int len);

private:
    PianoWidget();
    virtual void paint_event(GPaintEvent&) override;
    virtual void keydown_event(GKeyEvent&) override;
    virtual void keyup_event(GKeyEvent&) override;
    virtual void custom_event(CCustomEvent&) override;
    virtual void mousedown_event(GMouseEvent&) override;
    virtual void mouseup_event(GMouseEvent&) override;
    virtual void mousemove_event(GMouseEvent&) override;

    double w_sine(size_t);
    double w_saw(size_t);
    double w_square(size_t);
    double w_triangle(size_t);
    double w_noise();

    Rect define_piano_key_rect(int index, PianoKey) const;
    PianoKey find_key_for_relative_position(int x, int y) const;

    void render_piano_key(GPainter&, int index, PianoKey, const StringView&);
    void render_piano(GPainter&);
    void render_knobs(GPainter&);
    void render_knob(GPainter&, const Rect&, bool state, const StringView&);

    void note(Music::PianoKey offset_n, KeyCode key_code);
    void update_keys();
    int octave_base() const;

    int m_sample_count { 0 };
    Sample* m_front_buffer { nullptr };
    Sample* m_back_buffer { nullptr };

#define note_count sizeof(note_frequency) / sizeof(double)

    bool m_note_on[note_count];
    double m_power[note_count];
    double m_sin_pos[note_count];
    double m_square_pos[note_count];
    double m_saw_pos[note_count];
    double m_triangle_pos[note_count];

    int m_octave_min { 1 };
    int m_octave_max { 6 };
    int m_octave { 4 };

    int m_width { 512 };
    int m_height { 512 };

    int m_wave_type { 0 };
    bool m_delay_enabled { false };
    bool m_release_enabled { false };

    bool keys[256];

    PianoKey m_piano_key_under_mouse { K_None };
    bool m_mouse_pressed { false };
};
