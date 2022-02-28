RGB_MATRIX_EFFECT(explosion)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

#define EXPLOSION_COLOR_SEGMENTS 3
#define EXPLOSION_COLOR_COUNT (EXPLOSION_COLOR_SEGMENTS + 1)
#define EXPLOSION_SEGMENT_LENGTH (0xFF/EXPLOSION_COLOR_SEGMENTS)



#define EXPLOSION_RADIUS_GRADIENT 20
#define EXPLOSION_RADIUS_GRADIENT_MIN_EXP 3
#define EXPLOSION_RADIUS_GRADIENT_MIN (1 << EXPLOSION_RADIUS_GRADIENT_MIN_EXP)

#define EXPLOSION_RADIUS 69

#define EXPLOSION_TIME 10
#define EXPLOSION_TIME_MIN_EXP 2
#define EXPLOSION_TIME_MIN (1 << EXPLOSION_TIME_MIN_EXP)

#define EXPLOSION_FADE_DISTANCE 200
#define EXPLOSION_FADE_DISTANCE_MIN_EXP 6
#define EXPLOSION_FADE_DISTANCE_MIN (1 << EXPLOSION_FADE_DISTANCE_MIN_EXP)

#define EXPLOSION_FADE_TIME (EXPLOSION_TIME * 69)
#define EXPLOSION_FADE_TIME_MIN_EXP 7
#define EXPLOSION_FADE_TIME_MIN (1 << EXPLOSION_FADE_DISTANCE_MIN_EXP)

#define EXPLOSION_TOTAL_TIME (EXPLOSION_TIME + EXPLOSION_FADE_TIME)


static HSV explosion_colors[EXPLOSION_COLOR_COUNT]  = {
    {0,0,255},
    {42,255,255},
    {0,255,255},
    {0,255,0}
};

static HSV explosion_fade(uint8_t fade){
    uint8_t seg = fade / EXPLOSION_SEGMENT_LENGTH;
    if(seg >= EXPLOSION_COLOR_SEGMENTS){
        seg = EXPLOSION_COLOR_SEGMENTS - 1;
    }
    uint8_t seg_fade = mul8(EXPLOSION_COLOR_SEGMENTS, sub8(fade, mul8(EXPLOSION_SEGMENT_LENGTH, seg)));

    HSV hsv = explosion_colors[seg];

    hsv.h = blend8(explosion_colors[seg].h,explosion_colors[seg+1].h,seg_fade);
    hsv.s = blend8(explosion_colors[seg].s,explosion_colors[seg+1].s,seg_fade);
    hsv.v = blend8(explosion_colors[seg].v,explosion_colors[seg+1].v,seg_fade);

    return hsv;
}


static bool explosion(effect_params_t* params){
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t hit_count = g_last_hit_tracker.count;
    uint8_t led_count = sub8(led_max,led_min);

    HSV hsv_buffer[RGB_MATRIX_LED_PROCESS_LIMIT];
    uint8_t last_dist[RGB_MATRIX_LED_PROCESS_LIMIT];

    for (uint8_t i = 0; i < led_count; i++) {
        hsv_buffer[i].h = 0;
        hsv_buffer[i].s = 0;
        hsv_buffer[i].v = 0;
        last_dist[i] = 0xFF;
    }

    for (uint8_t j = 0; j < hit_count; j++) {

        uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));

        uint8_t radius;
        uint8_t radius_and_gradient;
        uint16_t fade_ratio;

        if(tick < EXPLOSION_TIME ){

            uint8_t explosion_ratio = (uint8_t)scale16by8(tick << (8 - EXPLOSION_TIME_MIN_EXP), (1 << (8 + EXPLOSION_TIME_MIN_EXP)) / EXPLOSION_TIME);

            radius = scale8_video(EXPLOSION_RADIUS,explosion_ratio);

            radius_and_gradient = add8(radius,EXPLOSION_RADIUS_GRADIENT);

        } else if (tick < EXPLOSION_TOTAL_TIME){
            radius = EXPLOSION_RADIUS;
            radius_and_gradient = EXPLOSION_RADIUS + EXPLOSION_RADIUS_GRADIENT;

            fade_ratio = scale16by8((tick - EXPLOSION_TIME) << (8 - EXPLOSION_FADE_TIME_MIN_EXP), (1 << (8 + EXPLOSION_FADE_TIME_MIN_EXP)) / EXPLOSION_FADE_TIME);
        }

        for (uint8_t i = led_min; i < led_max; i++) {
            RGB_MATRIX_TEST_LED_FLAGS();

            uint8_t buffer_index = sub8(i,led_min);

            uint16_t dx   = (uint16_t)abs((int16_t)g_led_config.point[i].x - (int16_t)g_last_hit_tracker.x[j]);
            uint16_t dy   = (uint16_t)abs((int16_t)g_led_config.point[i].y - (int16_t)g_last_hit_tracker.y[j]);
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            if(dist < radius_and_gradient && tick < EXPLOSION_TOTAL_TIME){

                uint16_t fade = scale16by8((uint16_t)dist << (8 - EXPLOSION_FADE_DISTANCE_MIN_EXP), (1 << (8 + EXPLOSION_FADE_DISTANCE_MIN_EXP)) / EXPLOSION_FADE_DISTANCE);
                if (tick >= EXPLOSION_TIME){
                    fade += fade_ratio;
                }
                if(fade > 255){
                    fade = 255;
                }

                HSV temp_hsv = explosion_fade((uint8_t)fade);
                HSV current_hsv = hsv_buffer[buffer_index];

                if(dist > radius){

                    uint8_t gradient = (uint8_t)scale16by8((uint16_t)sub8(dist,radius) << (8 - EXPLOSION_RADIUS_GRADIENT_MIN_EXP), (1 << (8 + EXPLOSION_RADIUS_GRADIENT_MIN_EXP)) / EXPLOSION_RADIUS_GRADIENT);

                    temp_hsv.v = scale8_video(temp_hsv.v,sub8(255,gradient));
                }

                if(
                    temp_hsv.v > current_hsv.v ||
                    (temp_hsv.v == current_hsv.v && temp_hsv.s < current_hsv.s) ||
                    (temp_hsv.v == current_hsv.v && temp_hsv.s >= current_hsv.s && dist < last_dist[buffer_index])
                ){

                    hsv_buffer[buffer_index] = temp_hsv;
                    last_dist[buffer_index] = dist;

                }
            }
        }
    }
    for (uint8_t i = 0; i < led_count; i++) {
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv_buffer[i]);
        rgb_matrix_set_color(add8(led_min,i), rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif
#endif