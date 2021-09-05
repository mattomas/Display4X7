#include <Arduino.h>

/**
  4 x 7 segment display
      A
      -
  F | G |  B
      -
  E |   |  C
      -   . DP
      D

  Segmeny pins:
  A,B,C,D,E,F,G,DP
  Digit pins:
  12, 9, 8, 6

PIN NO	SEGMENT/COMMON POINT
    BOTTOM DISPLAY PINS ->
      1	E
      2	D
      3	.
      4	C
      5	G
      6	D4
    TOP DISPLAY PINS <-
      7	B
      8	D3
      9	D2
      10 F
      11 A
      12 D1
*/

#ifndef DISPLAY_7SEMGENTS_4DIGITS_H
#define DISPLAY_7SEMGENTS_4DIGITS_H

class Display7Sements4Digits {

    public:
        Display7Sements4Digits(
            const int digit_pins[], // D1, D2, D3, D4
            const int segment_pins[] // A, B, C, D, E, F, G, DP
        );
        void init();
        void initPins();
        void set(const char * p_str);
        void setDigitsValue(int value);
        void setDigitsBlank();
        void setDigitsError();
        void setPins(int pin_map, int digit);
        int findDigitPinMap(char ch);
        void refresh();
    private:
        int *p_digit_pins;
        int *p_segment_pins;
        int *p_current_digits;
        int i_refresh;
        static const int kErrorChar;
        static const int kBlankChar;
        static const int kCharsMap[][2];
        static const int kNDigits;
        static const int kNSegments;
        static const int kNChars;
};

#endif
