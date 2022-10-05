RGB_MATRIX_EFFECT(explosion)
RGB_MATRIX_EFFECT(cast_explosion)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

#define EX_CAST_TICKS 256
#define EX_HOLD_TICKS 2048
#define EX_BOOM_TICKS 512
#define EX_FADE_TICKS 1024
#define EX_TOTAL_TICKS (EX_CAST_TICKS + EX_HOLD_TICKS + EX_BOOM_TICKS + EX_FADE_TICKS)

#define EX_SPELL_RADIUS 86
#define EX_SPELL_RADIUS_EXP 8
#define EX_SPIRAL_COUNT 4
#define EX_SPIRAL_RADIUS 13

#define EX_FADE_SPEED 4

#define EX_COLOR_COUNT 4


const uint8_t gnoise_data[16][8][8] = {{{229, 156, 205, 233, 73, 187, 222, 103}, {30, 236, 241, 214, 169, 193, 37, 50}, {202, 60, 36, 16, 196, 171, 70, 226}, {215, 82, 48, 8, 55, 137, 78, 154}, {197, 69, 68, 167, 168, 16, 199, 172}, {192, 32, 102, 112, 47, 126, 224, 212}, {68, 57, 224, 110, 215, 106, 213, 97}, {218, 116, 191, 104, 127, 146, 59, 65}}, 
{{93, 145, 91, 158, 226, 103, 174, 197}, {130, 231, 170, 73, 163, 132, 41, 185}, {61, 77, 120, 163, 220, 188, 91, 230}, {81, 21, 52, 18, 183, 126, 93, 170}, {133, 33, 189, 44, 212, 76, 74, 60}, {117, 221, 143, 62, 116, 141, 189, 199}, {165, 135, 156, 44, 127, 36, 52, 79}, {201, 230, 152, 177, 210, 93, 42, 220}}, {{105, 235, 101, 65, 197, 235, 22, 204}, {23, 113, 230, 166, 190, 134, 24, 31}, {217, 172, 184, 224, 170, 210, 116, 223}, {16, 108, 46, 137, 191, 192, 159, 212}, {40, 198, 115, 180, 189, 121, 113, 86}, {62, 23, 94, 208, 62, 165, 162, 204}, {185, 128, 24, 199, 185, 201, 208, 203}, {37, 47, 99, 112, 67, 39, 230, 45}}, {{207, 129, 114, 15, 205, 68, 117, 180}, {36, 113, 137, 233, 124, 104, 143, 78}, {128, 125, 133, 167, 96, 124, 53, 139}, {16, 137, 220, 115, 87, 14, 188, 106}, {217, 218, 106, 142, 147, 66, 49, 48}, {95, 64, 231, 20, 72, 123, 40, 118}, {40, 137, 107, 162, 190, 38, 31, 234}, {55, 97, 162, 75, 202, 62, 114, 169}}, {{235, 208, 59, 24, 187, 83, 228, 100}, {59, 240, 119, 123, 151, 160, 104, 233}, {39, 137, 92, 38, 137, 35, 204, 25}, {177, 139, 205, 175, 33, 144, 109, 200}, {166, 19, 150, 117, 48, 239, 128, 101}, {180, 173, 79, 218, 112, 133, 141, 186}, {22, 84, 67, 206, 94, 121, 112, 166}, {161, 155, 139, 179, 188, 125, 136, 
26}}, {{236, 169, 47, 173, 185, 220, 183, 46}, {83, 188, 118, 17, 126, 39, 237, 211}, {76, 83, 118, 18, 232, 117, 102, 146}, {217, 177, 180, 113, 172, 173, 141, 186}, {148, 113, 163, 154, 25, 224, 236, 213}, {59, 77, 125, 31, 89, 238, 23, 19}, {20, 29, 161, 19, 161, 144, 99, 170}, {45, 210, 40, 143, 31, 122, 43, 37}}, {{222, 171, 236, 134, 46, 108, 115, 205}, {81, 91, 143, 95, 170, 179, 55, 35}, {95, 186, 69, 118, 135, 227, 163, 118}, {187, 107, 74, 147, 48, 119, 104, 72}, {80, 77, 35, 34, 157, 48, 158, 149}, {22, 152, 37, 170, 169, 22, 48, 197}, {205, 145, 232, 105, 84, 179, 173, 37}, {180, 92, 220, 240, 153, 84, 134, 42}}, {{112, 37, 179, 131, 27, 107, 16, 200}, {70, 57, 141, 205, 80, 110, 130, 15}, {164, 39, 43, 71, 101, 25, 105, 17}, {121, 60, 142, 193, 114, 67, 26, 133}, {82, 48, 205, 63, 232, 147, 167, 142}, {109, 52, 213, 229, 54, 229, 150, 176}, {89, 69, 20, 158, 108, 66, 203, 163}, {188, 62, 180, 74, 125, 32, 84, 84}}, {{107, 122, 103, 179, 78, 214, 14, 202}, {201, 24, 11, 88, 97, 238, 172, 129}, {142, 32, 60, 14, 54, 139, 52, 105}, {64, 92, 115, 242, 104, 212, 186, 132}, {60, 226, 68, 222, 25, 81, 49, 87}, {98, 163, 77, 26, 92, 242, 202, 154}, {174, 80, 235, 177, 204, 69, 231, 32}, {99, 192, 231, 37, 220, 19, 46, 151}}, {{176, 24, 115, 108, 162, 23, 172, 161}, {106, 147, 204, 118, 72, 83, 93, 58}, {194, 175, 215, 194, 224, 216, 25, 63}, {135, 233, 109, 102, 57, 231, 100, 19}, {77, 107, 169, 35, 84, 216, 37, 188}, {74, 55, 196, 81, 63, 103, 130, 105}, {28, 19, 131, 129, 160, 79, 190, 156}, {171, 178, 163, 42, 241, 161, 59, 131}}, {{173, 49, 59, 97, 141, 38, 111, 184}, {146, 204, 216, 138, 199, 182, 17, 63}, {29, 199, 77, 145, 22, 23, 189, 143}, {230, 150, 170, 219, 225, 110, 177, 32}, {179, 199, 27, 126, 88, 111, 23, 118}, {102, 14, 15, 21, 28, 215, 238, 223}, {103, 68, 28, 127, 112, 196, 168, 176}, {57, 58, 147, 146, 178, 168, 207, 125}}, {{218, 81, 136, 102, 155, 20, 93, 109}, {178, 99, 80, 189, 103, 85, 41, 80}, {108, 225, 98, 195, 183, 55, 131, 41}, {226, 119, 139, 176, 181, 153, 163, 103}, {73, 231, 107, 26, 90, 131, 65, 131}, {102, 14, 164, 184, 102, 105, 120, 190}, {53, 73, 120, 215, 97, 212, 171, 201}, {107, 74, 146, 56, 214, 149, 231, 170}}, {{152, 
31, 99, 172, 40, 48, 145, 165}, {216, 64, 231, 131, 78, 124, 186, 111}, {172, 213, 28, 45, 115, 78, 120, 158}, {86, 178, 185, 82, 216, 40, 190, 144}, {147, 208, 150, 108, 230, 133, 19, 42}, {221, 187, 148, 45, 204, 237, 124, 161}, {100, 112, 16, 78, 226, 70, 217, 172}, {153, 176, 160, 190, 114, 93, 123, 215}}, {{141, 214, 97, 26, 163, 184, 93, 70}, {121, 227, 28, 178, 113, 51, 63, 176}, {152, 74, 203, 213, 67, 87, 111, 182}, {182, 97, 113, 182, 23, 222, 166, 136}, {42, 96, 145, 78, 74, 61, 151, 53}, {194, 107, 108, 156, 32, 90, 195, 202}, {148, 233, 201, 135, 235, 45, 201, 149}, {62, 197, 73, 32, 81, 72, 111, 162}}, {{26, 
109, 104, 21, 130, 154, 142, 58}, {82, 224, 68, 163, 173, 130, 152, 162}, {160, 37, 51, 100, 190, 202, 104, 36}, {168, 240, 234, 25, 11, 121, 41, 214}, {120, 235, 46, 103, 180, 42, 147, 108}, {126, 156, 55, 59, 17, 200, 223, 240}, {227, 70, 172, 48, 184, 185, 239, 54}, {87, 140, 93, 100, 99, 229, 245, 233}}, {{216, 170, 13, 88, 187, 35, 168, 59}, {34, 39, 35, 211, 163, 195, 54, 168}, {167, 102, 177, 123, 209, 191, 227, 78}, {72, 75, 231, 55, 76, 230, 55, 179}, {57, 199, 99, 210, 164, 228, 178, 76}, {93, 144, 219, 98, 173, 142, 21, 134}, {36, 43, 132, 95, 204, 26, 117, 212}, {238, 236, 151, 43, 130, 54, 39, 234}}};

static uint8_t gnoise(uint8_t x,uint8_t y,uint8_t z){

    uint8_t xb = (x >> 4);
    uint8_t yb = (y >> 5);
    uint8_t zb = (z >> 5);

    uint8_t xi = (xb + ((x >> 3) & 0x01)) % 16;
    uint8_t yi = (yb + ((y >> 4) & 0x01)) % 8;
    uint8_t zi = (zb + ((z >> 4) & 0x01)) % 8;

    uint8_t mid_value = avg8(gnoise_data[xb][yb][zb],gnoise_data[xb == 15 ? 0 : xb + 1][yb == 15 ? 0 : yb + 1][zb == 15 ? 0 : zb + 1]);
    
    uint16_t weight = 0;
    weight += (uint16_t)((x & 0x08 ? x & 0x07 : 0x07 - (x & 0x07)) << 1);
    weight += (uint16_t)(y & 0x10 ? y & 0x0f : 0x0f - (y & 0x0f));
    weight += (uint16_t)(z & 0x10 ? z & 0x0f : 0x0f - (z & 0x0f));
    weight = (weight * 16) / 3;

    return blend8(mid_value,gnoise_data[xi][yi][zi],(uint8_t)weight);

}

static bool cast_explosion(effect_params_t* params){
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t hit_count = g_last_hit_tracker.count;
    uint8_t led_count = sub8(led_max,led_min);

    HSV hsv_buffer[RGB_MATRIX_LED_PROCESS_LIMIT];
    uint16_t last_tick[RGB_MATRIX_LED_PROCESS_LIMIT];

    for (uint8_t i = 0; i < led_count; i++) {
        hsv_buffer[i].h = 0;
        hsv_buffer[i].s = 0;
        hsv_buffer[i].v = 0;
        last_tick[i] = 0xffff;
    }


    for (uint8_t hit_index = 0; hit_index < hit_count; hit_index++) {

        uint16_t tick = g_last_hit_tracker.tick[hit_index];

        uint16_t radius;
        bool exploding = false;
        bool casting = false;
        uint8_t fade_ratio = 0;
        uint8_t overdrive_ratio = 0;

        if(tick < (EX_CAST_TICKS + EX_HOLD_TICKS)){
            casting = true;
            if (tick < EX_CAST_TICKS ){
                uint8_t cast_ratio = (uint8_t)tick;
                radius = scale16by8(EX_SPELL_RADIUS,cast_ratio);
            } else {
                radius = EX_SPELL_RADIUS;
            }
        } else if (tick < EX_TOTAL_TICKS){
            exploding = true;
            if(tick < (EX_CAST_TICKS + EX_HOLD_TICKS + EX_BOOM_TICKS)){
                radius = (tick - (EX_CAST_TICKS + EX_HOLD_TICKS))>>1;
                overdrive_ratio = sub8(255,(uint8_t)((tick - (EX_CAST_TICKS + EX_HOLD_TICKS))>>1));

            } else {
                radius = EX_BOOM_TICKS>>1;
                fade_ratio = (uint8_t)((tick - (EX_CAST_TICKS + EX_HOLD_TICKS + EX_BOOM_TICKS)) >> 2);
            }
        } else {
            continue;
        }

        for (uint8_t i = led_min; i < led_max; i++) {

            uint8_t buffer_index = sub8(i,led_min);
            if(last_tick[buffer_index] <= tick){
                continue;
            }



            // RGB_MATRIX_TEST_LED_FLAGS();
            int16_t dx    = (int16_t)g_led_config.point[i].x - (int16_t)g_last_hit_tracker.x[hit_index];
            int16_t dy    = (int16_t)g_led_config.point[i].y - (int16_t)g_last_hit_tracker.y[hit_index];
            uint16_t udx = (uint16_t)abs(dx);
            uint16_t udy = (uint16_t)abs(dy);
            uint8_t  dist  = sqrt16(udx * udx + udy * udy);
            dist = dist ? dist : 1;


            if(dist < radius){
                

                if(casting){

                    last_tick[buffer_index] = tick;


                    if (dist < 16){
                        hsv_buffer[buffer_index].h = 0;
                        hsv_buffer[buffer_index].s = 255;
                        hsv_buffer[buffer_index].v = 255;
                    } else {

                        uint8_t theta_offset = (tick >> 3) % (256 / EX_SPIRAL_COUNT);

                        int16_t theta = (int16_t)(atan2_8(dy,dx) % (256 / EX_SPIRAL_COUNT)) + (int16_t)theta_offset - (int16_t)(dist >> 1);

                        theta %= (256 / EX_SPIRAL_COUNT);
                        if(theta < 0){
                            theta += (256 / EX_SPIRAL_COUNT);
                        }
                        uint8_t color_ratio = qmul8((uint8_t)abs((256 / EX_SPIRAL_COUNT) / 2 - theta),EX_SPIRAL_COUNT*2);
                        
                        hsv_buffer[buffer_index].h = qsub8(42,scale8(42, color_ratio));
                        hsv_buffer[buffer_index].s = 255;
                        hsv_buffer[buffer_index].v = qsub8(255,color_ratio>>1);

                        uint16_t border_radius = radius - 16;
                        if(dist >= border_radius){
                            uint8_t border_ratio = (uint8_t)(dist - border_radius)<<4;
                            hsv_buffer[buffer_index].h = blend8(42,  hsv_buffer[buffer_index].h,border_ratio);
                            hsv_buffer[buffer_index].s = blend8(255,hsv_buffer[buffer_index].s,border_ratio);
                            hsv_buffer[buffer_index].v = blend8(255,hsv_buffer[buffer_index].v,border_ratio);
                        }
                    }
                } else if (exploding){


                    last_tick[buffer_index] = tick;

                    uint8_t theta = atan2_8(dy,dx);

                    uint8_t tick8 = (uint8_t)(tick % 256);
                    uint8_t slow_tick8 = (uint8_t)((tick >> 2) % 256);

                    hsv_buffer[buffer_index].h = scale8(42, gnoise(theta,sub8(dist,tick8),slow_tick8));
                    hsv_buffer[buffer_index].s = 255;
                    hsv_buffer[buffer_index].v = qadd8(scale8(gnoise(theta,sub8((dist),add8(tick8,42)),slow_tick8),fade_ratio & 0x80 ? sub8(255,qadd8(dist,fade_ratio<<1)) : sub8(255,scale8_video(dist,fade_ratio<<1))),qsub8(overdrive_ratio,dist));
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

static bool explosion(effect_params_t* params){
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t hit_count = g_last_hit_tracker.count;
    uint8_t led_count = sub8(led_max,led_min);

    HSV hsv_buffer[RGB_MATRIX_LED_PROCESS_LIMIT];
    uint16_t last_tick[RGB_MATRIX_LED_PROCESS_LIMIT];

    for (uint8_t i = 0; i < led_count; i++) {
        hsv_buffer[i].h = 0;
        hsv_buffer[i].s = 0;
        hsv_buffer[i].v = 0;
        last_tick[i] = 0xffff;
    }


    for (uint8_t hit_index = 0; hit_index < hit_count; hit_index++) {

        
        uint16_t tick = g_last_hit_tracker.tick[hit_index];

        uint16_t radius;
        uint8_t fade_ratio = 0;
        uint8_t overdrive_ratio = 0;

        if (tick < (EX_FADE_TICKS + EX_BOOM_TICKS)){
            if(tick < EX_BOOM_TICKS){
                radius = tick>>1;
                overdrive_ratio = sub8(255,(uint8_t)(tick>>1));

            } else {
                radius = EX_BOOM_TICKS>>1;
                fade_ratio = (uint8_t)((tick - EX_BOOM_TICKS) >> 2);
            }
        } else {
            continue;
        }


        for (uint8_t i = led_min; i < led_max; i++) {

            uint8_t buffer_index = sub8(i,led_min);
            if(last_tick[buffer_index] <= tick){
                continue;
            }

            // RGB_MATRIX_TEST_LED_FLAGS();
            int16_t dx    = (int16_t)g_led_config.point[i].x - (int16_t)g_last_hit_tracker.x[hit_index];
            int16_t dy    = (int16_t)g_led_config.point[i].y - (int16_t)g_last_hit_tracker.y[hit_index];
            uint16_t udx = (uint16_t)abs(dx);
            uint16_t udy = (uint16_t)abs(dy);
            uint8_t  dist  = sqrt16(udx * udx + udy * udy);
            dist = dist ? dist : 1;


            if(dist < radius){
                
                last_tick[buffer_index] = tick;

                uint8_t theta = atan2_8(dy,dx);

                uint8_t tick8 = (uint8_t)(tick % 256);
                uint8_t slow_tick8 = (uint8_t)((tick >> 2) % 256);

                hsv_buffer[buffer_index].h = scale8(42, gnoise(theta,sub8(dist,tick8),slow_tick8));
                hsv_buffer[buffer_index].s = 255;
                hsv_buffer[buffer_index].v = qadd8(scale8(gnoise(theta,sub8((dist),add8(tick8,42)),slow_tick8),fade_ratio & 0x80 ? sub8(255,qadd8(dist,fade_ratio<<1)) : sub8(255,scale8_video(dist,fade_ratio<<1))),qsub8(overdrive_ratio,dist));

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