#include "titan_tube.h"
#include <math.h>

TitanTube create_tube(float x) {
    TitanTube tube = { .x = x, .color = BLACK, .cue_count = 0 };
    return tube;
}

void add_cue(TitanTube *tube, float start, float end, Color color, float intensity) {
    add_fade_cue(tube, start, end, color, intensity, INTENSITY_CONSTANT);
}

void add_fade_cue(TitanTube *tube, float start, float end, Color color, float intensity, IntensityMode mode) {
    if (tube->cue_count >= MAX_CUES) return;
    TubeCue *cue = &tube->cues[tube->cue_count++];
    cue->start_time = start;
    cue->end_time = end;
    cue->color = color;
    cue->intensity = intensity;
    cue->mode = mode;
}

void update_tube(TitanTube *tube, float time_now) {
    tube->color = BLACK;

    for (int i = 0; i < tube->cue_count; i++) {
        TubeCue *cue = &tube->cues[i];

        if (time_now >= cue->start_time && time_now <= cue->end_time) {
            float t = (time_now - cue->start_time) / (cue->end_time - cue->start_time);
            float f = cue->intensity;

            switch (cue->mode) {
                case INTENSITY_CONSTANT: break;
                case INTENSITY_FADE_IN:  f *= t; break;
                case INTENSITY_FADE_OUT: f *= (1.0f - t); break;
                case INTENSITY_PULSE:    f *= 0.5f * (1 + sinf(t * 2 * PI)); break;
            }

            Color c = cue->color;
            c.r = (unsigned char)(c.r * f);
            c.g = (unsigned char)(c.g * f);
            c.b = (unsigned char)(c.b * f);
            tube->color = c;
            return;
        }
    }
}

void draw_tube(TitanTube *tube, int index) {
    int x = 100 + index * 150;
    int y = 100;
    DrawRectangle(x, y, 100, 30, tube->color);
}

// === CYCLES ===

void Argile(TitanTube *tubes, int count) {
  // Bougie light
  for (int i = 0; i < count; i++) {
    add_fade_cue(&tubes[i], 0.0f, 3.0f, (Color){255, 255, 240, 255} , 1.0f, INTENSITY_FADE_IN);
    add_fade_cue(&tubes[i], 3.0f, 6.0f, (Color){255, 255, 240, 255}, 1.0f, INTENSITY_CONSTANT);
    add_fade_cue(&tubes[i], 6.0f, 9.0f, (Color){255, 255, 240, 255}, 1.0f, INTENSITY_FADE_OUT);
  }
}

void Papier(TitanTube *tubes, int count) {
  // Bougie light 
  for (int i = 0; i < count; i++) {
    add_fade_cue(&tubes[i], 0.0f, 3.0f, (Color){255, 160, 70, 255} , 1.0f, INTENSITY_FADE_IN);
    add_fade_cue(&tubes[i], 3.0f, 6.0f, (Color){255, 160, 70, 255}, 1.0f, INTENSITY_CONSTANT);
    add_fade_cue(&tubes[i], 6.0f, 9.0f, (Color){255, 160, 70, 255}, 1.0f, INTENSITY_FADE_OUT);
  }
}

void Machine_ecrire(TitanTube *tubes, int count) {
  // Lampe a fioul 
  for (int i = 0; i < count; i++) {
    add_fade_cue(&tubes[i], 0.0f, 3.0f, (Color){255, 120, 30, 200} , 1.0f, INTENSITY_FADE_IN);
    add_fade_cue(&tubes[i], 3.0f, 6.0f, (Color){255, 120, 30, 200}, 1.0f, INTENSITY_CONSTANT);
    add_fade_cue(&tubes[i], 6.0f, 9.0f, (Color){255, 120, 30, 200}, 1.0f, INTENSITY_FADE_OUT);
  }
}

void Old_comp(TitanTube *tubes, int count) {
  // Old electric light
  for (int i = 0; i < count; i++) {
    add_fade_cue(&tubes[i], 0.0f, 3.0f, (Color){255, 180, 100, 255} , 1.0f, INTENSITY_FADE_IN);
    add_fade_cue(&tubes[i], 3.0f, 6.0f, (Color){255, 180, 100, 255}, 1.0f, INTENSITY_CONSTANT);
    add_fade_cue(&tubes[i], 6.0f, 9.0f, (Color){255, 180, 100, 255}, 1.0f, INTENSITY_FADE_OUT);
  }
}

void Modern_comp(TitanTube *tubes, int count) {
  // Modfern electric light 
  for (int i = 0; i < count; i++) {
    add_fade_cue(&tubes[i], 0.0f, 3.0f, (Color){255, 200, 130, 255} , 1.0f, INTENSITY_FADE_IN);
    add_fade_cue(&tubes[i], 3.0f, 6.0f, (Color){255, 200, 130, 255}, 1.0f, INTENSITY_CONSTANT);
    add_fade_cue(&tubes[i], 6.0f, 9.0f, (Color){255, 200, 130, 255}, 1.0f, INTENSITY_FADE_OUT);
  }
}


static TubeCycle cycles[] = {
    { "Argile", Argile, 9.0f},
    { "Papier", Papier, 9.0f},
    { "Machine à ecrire", Machine_ecrire, 9.0f},
    { "Old Computer", Old_comp, 9.0f},
    { "Modern Computer", Modern_comp, 9.0f}
};

int get_available_cycles(const TubeCycle **list) {
    *list = cycles;
    return sizeof(cycles) / sizeof(TubeCycle);
}





