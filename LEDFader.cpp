/*
 * LED.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: jeremy
 */

#include "LEDFader.h"
#include <TimerOne.h>

const uint16_t gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,
    3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,
    4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,
    6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,
    7,  8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,
    9, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12,
   12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14,
   15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17,
   18, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 20, 21, 21,
   21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25,
   25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 29, 29, 29,
   29, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 34, 34,
   34, 35, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39,
   40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 45, 45,
   45, 46, 46, 46, 47, 47, 48, 48, 48, 49, 49, 50, 50, 50, 51, 51,
   52, 52, 52, 53, 53, 54, 54, 55, 55, 55, 56, 56, 57, 57, 58, 58,
   58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 63, 64, 64, 65, 65,
   66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73,
   74, 74, 75, 75, 76, 76, 77, 77, 78, 79, 79, 80, 80, 81, 81, 82,
   82, 83, 83, 84, 85, 85, 86, 86, 87, 87, 88, 89, 89, 90, 90, 91,
   92, 92, 93, 93, 94, 95, 95, 96, 96, 97, 98, 98, 99, 99,100,101,
  101,102,103,103,104,105,105,106,106,107,108,108,109,110,110,111,
  112,112,113,114,115,115,116,117,117,118,119,119,120,121,122,122,
  123,124,124,125,126,127,127,128,129,130,130,131,132,132,133,134,
  135,136,136,137,138,139,139,140,141,142,143,143,144,145,146,146,
  147,148,149,150,151,151,152,153,154,155,155,156,157,158,159,160,
  161,161,162,163,164,165,166,167,167,168,169,170,171,172,173,174,
  175,175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,
  189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,
  205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,
  221,222,223,224,225,226,228,229,230,231,232,233,234,235,236,237,
  238,239,241,242,243,244,245,246,247,248,249,251,252,253,254,255,
  256,257,259,260,261,262,263,264,266,267,268,269,270,272,273,274,
  275,276,278,279,280,281,282,284,285,286,287,289,290,291,292,294,
  295,296,297,299,300,301,302,304,305,306,308,309,310,311,313,314,
  315,317,318,319,321,322,323,325,326,327,329,330,331,333,334,336,
  337,338,340,341,342,344,345,347,348,349,351,352,354,355,356,358,
  359,361,362,364,365,366,368,369,371,372,374,375,377,378,380,381,
  383,384,386,387,389,390,392,393,395,396,398,399,401,402,404,405,
  407,408,410,412,413,415,416,418,419,421,423,424,426,427,429,431,
  432,434,435,437,439,440,442,444,445,447,448,450,452,453,455,457,
  458,460,462,463,465,467,468,470,472,474,475,477,479,480,482,484,
  486,487,489,491,493,494,496,498,500,501,503,505,507,509,510,512,
  514,516,518,519,521,523,525,527,528,530,532,534,536,538,539,541,
  543,545,547,549,551,553,554,556,558,560,562,564,566,568,570,572,
  574,575,577,579,581,583,585,587,589,591,593,595,597,599,601,603,
  605,607,609,611,613,615,617,619,621,623,625,627,629,631,633,635,
  637,640,642,644,646,648,650,652,654,656,658,660,663,665,667,669,
  671,673,675,678,680,682,684,686,688,690,693,695,697,699,701,704,
  706,708,710,712,715,717,719,721,724,726,728,730,733,735,737,739,
  742,744,746,749,751,753,755,758,760,762,765,767,769,772,774,776,
  779,781,783,786,788,790,793,795,798,800,802,805,807,810,812,814,
  817,819,822,824,827,829,831,834,836,839,841,844,846,849,851,854,
  856,859,861,864,866,869,871,874,876,879,881,884,887,889,892,894,
  897,899,902,905,907,910,912,915,918,920,923,925,928,931,933,936,
  939,941,944,947,949,952,955,957,960,963,965,968,971,973,976,979,
  982,984,987,990,992,995,998,1001,1004,1006,1009,1012,1015,1017,1020,1023 };

LEDFader::LEDFader(uint8_t pwm_pin) {
  pin = pwm_pin;
  color = 0;
  to_color = 0;
  last_step_time = 0;
  interval = 0;
  duration = 0;
  percent_done = 0;
  curve = (curve_function)0;
}

void LEDFader::set_pin(uint8_t pwm_pin) {
  pin = pwm_pin;
}
uint8_t LEDFader::get_pin(){
  return pin;
}

void LEDFader::set_value(int value) {
  if (!pin) return;
  color = (uint16_t)constrain(value, 0, 1023);
  Timer1.setPwmDuty(pin, gamma[color]);
}

uint16_t LEDFader::get_value() {
  return color;
}

uint16_t LEDFader::get_target_value() {
  return to_color;
}

// Set curve to transform output
void LEDFader::set_curve(curve_function c) {
  curve = c;
}

// Get the current curve function pointer
LEDFader::curve_function LEDFader::get_curve() {
  return curve;
}

void LEDFader::slower(int by) {
  float cached_percent = percent_done;
  duration += by;
  fade(to_color, duration);
  percent_done = cached_percent;
}

void LEDFader::faster(int by) {
  float cached_percent = percent_done;

  // Ends the fade
  if (duration <= by) {
    stop_fade();
    set_value(to_color);
  }
  else {
    duration -= by;
    fade(to_color, duration);
  }
  percent_done = cached_percent;
}

void LEDFader::fade(uint16_t value, unsigned int time) {
  stop_fade();
  percent_done = 0;

  // No pin defined
  if (!pin) {
    return;
  }

  // Color hasn't changed
  if (value == color) {
    return;
  }

  if (time <= MIN_INTERVAL) {
    set_value(value);
    return;
  }

  duration = time;
  to_color = (uint16_t)constrain(value, 0, 1023);

  // Figure out what the interval should be so that we're chaning the color by at least 1 each cycle
  // (minimum interval is MIN_INTERVAL)
  float color_diff = abs(color - to_color);
  interval = round((float)duration / color_diff);
  if (interval < MIN_INTERVAL) {
    interval = MIN_INTERVAL;
  }

  last_step_time = millis();
}

bool LEDFader::is_fading() {
  if (!pin)
    return false;
  if (duration > 0)
    return true;
  return false;
}

void LEDFader::stop_fade() {
  percent_done = 100;
  duration = 0;
}

uint8_t LEDFader::get_progress() {
  return round(percent_done);
}

bool LEDFader::update() {

  // No pin defined
  if (!pin) {
    return false;
  }

  // No fade
  if (duration == 0) {
    return false;
  }

  unsigned long now = millis();
  unsigned int time_diff = now - last_step_time;

  // Interval hasn't passed yet
  if (time_diff < interval) {
    return true;
  }

  // How far along have we gone since last update
  float percent = (float)time_diff / (float)duration;
  percent_done += percent;

  // We've hit 100%, set LED to the final color
  if (percent >= 1) {
    stop_fade();
    set_value(to_color);
    return false;
  }

  // Move color to where it should be
  int color_diff = to_color - color;
  int increment = round(color_diff * percent);

  set_value(color + increment);

  // Update time and finish
  duration -= time_diff;
  last_step_time = millis();
  return true;
}

