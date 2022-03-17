RGB_MATRIX_EFFECT(explosion)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

#define EXPLOSION_COLOR_SEGMENTS 3
#define EXPLOSION_COLOR_COUNT (EXPLOSION_COLOR_SEGMENTS + 1)
#define EXPLOSION_SEGMENT_LENGTH (0xFF/EXPLOSION_COLOR_SEGMENTS)


#define EXPLOSION_CHARGE_RADIUS 50

#define EXPLOSION_RADIUS 200
#define EXPLOSION_RADIUS_RATIO_EXP 6
#define EXPLOSION_RADIUS_SCALE_EXP 2

#define EXPLOSION_CHARGE_SPOKES 4
#define EXPLOSION_CHARGE_THETA_PERIOD (256 / EXPLOSION_CHARGE_SPOKES)
#define EXPLOSION_CHARGE_SPOKE_SLOPE 2
#define EXPLOSION_CHARGE_SPOKE_GRADIENT 100
#define EXPLOSION_CHARGE_CIRCLE_GRADIENT 16

#define EXPLOSION_CHARGE_ROTATE_SPEED 140

#define EXPLOSION_CHARGE_TIME 50
#define EXPLOSION_CHARGE_TIME_RATIO_EXP 5
#define EXPLOSION_CHARGE_TIME_SCALE_EXP 2

#define EXPLOSION_CHARGE_HOLD_TIME 1600
#define EXPLOSION_CHARGE_HOLD_RATIO_TIME_EXP 7

#define EXPLOSION_CHARGE_FADE_TIME 400
#define EXPLOSION_CHARGE_FADE_TIME_RATIO_EXP 7

#define EXPLOSION_CHARGE_TOTAL_TIME (EXPLOSION_CHARGE_TIME + EXPLOSION_CHARGE_HOLD_TIME + EXPLOSION_CHARGE_FADE_TIME)

#define EXPLOSION_FADE_DISTANCE 215
#define EXPLOSION_FADE_DISTANCE_RATIO_EXP 6

#define EXPLOSION_TIME 40
#define EXPLOSION_TIME_RATIO_EXP 4
#define EXPLOSION_TIME_SCALE_EXP 4

#define EXPLOSION_FADE_TIME (EXPLOSION_TIME * 50)
#define EXPLOSION_FADE_TIME_RATIO_EXP 7
#define EXPLOSION_FADE_TIME_SCALE_EXP 0

#define EXPLOSION_SCALE_RATIO(a,b) scale16by8( (uint16_t) (a) << (8 - b ## _RATIO_EXP), (1 << (8 + b ## _RATIO_EXP)) / b)
#define EXPLOSION_SCALE(a,b,c) scale16by8( ( (uint16_t) (a) * (uint16_t) (b) ) >> ( 8 - c ## _SCALE_EXP), (1 << (16 - c ## _SCALE_EXP)) / c)

#define EXPLOSION_TOTAL_TIME (EXPLOSION_TIME + EXPLOSION_FADE_TIME)


// static HSV explosion_colors[EXPLOSION_COLOR_COUNT]  = {
//     {0,0,255},
//     {42,255,255},
//     {0,255,255},
//     {0,255,0}
// };

// static HSV explosion_fade(uint8_t fade){
//     uint8_t seg = fade / EXPLOSION_SEGMENT_LENGTH;
//     if(seg >= EXPLOSION_COLOR_SEGMENTS){
//         seg = EXPLOSION_COLOR_SEGMENTS - 1;
//     }
//     uint8_t seg_fade = mul8(EXPLOSION_COLOR_SEGMENTS, sub8(fade, mul8(EXPLOSION_SEGMENT_LENGTH, seg)));

//     HSV hsv = explosion_colors[seg];

//     hsv.h = blend8(explosion_colors[seg].h,explosion_colors[seg+1].h,seg_fade);
//     hsv.s = blend8(explosion_colors[seg].s,explosion_colors[seg+1].s,seg_fade);
//     hsv.v = blend8(explosion_colors[seg].v,explosion_colors[seg+1].v,seg_fade);

//     return hsv;
// }


static bool explosion(effect_params_t* params){
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t hit_count = g_last_hit_tracker.count;
    uint8_t led_count = sub8(led_max,led_min);

    HSV hsv_buffer[RGB_MATRIX_LED_PROCESS_LIMIT];
    int8_t last_dist[RGB_MATRIX_LED_PROCESS_LIMIT];

    for (uint8_t i = 0; i < led_count; i++) {
        hsv_buffer[i].h = 0;
        hsv_buffer[i].s = 0;
        hsv_buffer[i].v = 0;
        last_dist[i] = 0xFF;
    }

    for (int8_t j = 0; j < hit_count; j++) {

        uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));

        uint8_t charge_radius = 0;
        // uint16_t theta_offset = 0;
        uint8_t charge_fade = 0;

        // uint8_t explosion_fade = 0;
        // uint8_t explosion_radius = 0;

        bool explosion = false;

        if(tick < EXPLOSION_CHARGE_TIME){
            // theta_offset = scale16by8(tick,EXPLOSION_CHARGE_ROTATE_SPEED);
            charge_radius = EXPLOSION_SCALE(tick,EXPLOSION_CHARGE_RADIUS,EXPLOSION_CHARGE_TIME);
            charge_fade = 0xFF;
        } else if (tick < (EXPLOSION_CHARGE_TIME + EXPLOSION_CHARGE_HOLD_TIME)){
            // theta_offset = scale16by8(tick,EXPLOSION_CHARGE_ROTATE_SPEED);
            charge_radius = EXPLOSION_CHARGE_RADIUS;
            charge_fade = 0xFF;
        } else if (tick < EXPLOSION_CHARGE_TOTAL_TIME){
            // theta_offset = scale16by8(tick,EXPLOSION_CHARGE_ROTATE_SPEED);
            charge_radius = EXPLOSION_CHARGE_RADIUS;
            charge_fade = 0xFF - EXPLOSION_SCALE_RATIO(tick-(EXPLOSION_CHARGE_TIME + EXPLOSION_CHARGE_HOLD_TIME),EXPLOSION_CHARGE_FADE_TIME);
        } else if(j == hit_count - 1){
            explosion = true;
            if (tick < (EXPLOSION_CHARGE_TOTAL_TIME + EXPLOSION_TIME)){
                // explosion_fade = EXPLOSION_SCALE_RATIO(tick-(EXPLOSION_CHARGE_TOTAL_TIME),EXPLOSION_TIME);
                // explosion_radius = EXPLOSION_SCALE(tick-(EXPLOSION_CHARGE_TOTAL_TIME),EXPLOSION_RADIUS,EXPLOSION_TIME);
            } else if (tick < (EXPLOSION_CHARGE_TOTAL_TIME + EXPLOSION_TIME + EXPLOSION_FADE_TIME)){
                // explosion_fade = 0xFF - EXPLOSION_SCALE_RATIO(tick-(EXPLOSION_CHARGE_TOTAL_TIME + EXPLOSION_TIME),EXPLOSION_FADE_TIME);
                // explosion_radius = EXPLOSION_RADIUS - EXPLOSION_SCALE(tick-(EXPLOSION_CHARGE_TOTAL_TIME + EXPLOSION_TIME),EXPLOSION_RADIUS,EXPLOSION_FADE_TIME);
            } else {
                continue;
            }
        } else {
            continue;
        }
        if (explosion){

        } else {
        
            for (uint8_t i = led_min; i < led_max; i++) {
                RGB_MATRIX_TEST_LED_FLAGS();

                uint8_t buffer_index = sub8(i,led_min);

                int16_t dxi = (int16_t)g_led_config.point[i].x - (int16_t)g_last_hit_tracker.x[j];
                int16_t dyi = (int16_t)g_led_config.point[i].y - (int16_t)g_last_hit_tracker.y[j];

                

                uint16_t dx = (uint16_t)abs(dxi);
                uint16_t dy = (uint16_t)abs(dyi);

                uint8_t r = sqrt16((uint16_t)dx * (uint16_t)dx + (uint16_t)dy * (uint16_t)dy);

                uint8_t theta = (uint8_t)(((uint16_t)atan2_8(dxi,dyi) << 2) + (uint16_t)(r << 2));

                HSV current_hsv = hsv_buffer[buffer_index];
                HSV temp_hsv = {0,0xFF,0x00};

                if(r < charge_radius){

                    temp_hsv.v = 0xff;

                    uint8_t fade = cos8((uint8_t)theta + (uint8_t)tick);

                    temp_hsv.h = fade > 0xFF - EXPLOSION_CHARGE_SPOKE_GRADIENT ? 0 : 8;

                } else if (r < charge_radius + EXPLOSION_CHARGE_CIRCLE_GRADIENT){
                    temp_hsv.v = 0xFF;
                }

                temp_hsv.v = scale8(temp_hsv.v,charge_fade);

                if(
                    temp_hsv.v > current_hsv.v ||
                    (temp_hsv.v == current_hsv.v && temp_hsv.s < current_hsv.s) ||
                    (temp_hsv.v == current_hsv.v && temp_hsv.s >= current_hsv.s && r < last_dist[buffer_index])
                ){

                    hsv_buffer[buffer_index] = temp_hsv;
                    last_dist[buffer_index] = r;

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