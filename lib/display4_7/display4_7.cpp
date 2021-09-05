#include "display4_7.h"

const int Display7Sements4Digits::kNDigits = 4;
const int Display7Sements4Digits::kNSegments = 8;
// mapping character -> pin positions
// 0, 1
const int Display7Sements4Digits::kNChars = 14;
const int Display7Sements4Digits::kCharsMap[13][2] = {
    {(int)'0', B00111111},
    {(int)'1', B00000110},
    {(int)'2', B01011011},
    {(int)'3', B01001111},
    {(int)'4', B01100110},
    {(int)'5', B01101101},
    {(int)'6', B01111101},
    {(int)'7', B00000111},
    {(int)'8', B01111111},
    {(int)'9', B01101111},
    {(int)'.', B10000000},
    {(int)'-', B01000000},
    {(int)' ', B00000000},
};

const int Display7Sements4Digits::kErrorChar = B01001111;
const int Display7Sements4Digits::kBlankChar = B00000000;

Display7Sements4Digits::Display7Sements4Digits(
    const int digit_pins[],
    const int segment_pins[]
) {
    this->p_digit_pins = new int[this->kNDigits];
    this->p_segment_pins = new int[this->kNSegments];
    this->p_current_digits = new int[this->kNDigits];
    for (int i = 0; i < this->kNDigits; i++) {
        this->p_digit_pins[i] = digit_pins[i];
    }

    for (int i = 0; i < this->kNSegments; i++) {
        this->p_segment_pins[i] = segment_pins[i];
    }

    this->i_refresh = 0;
}

void Display7Sements4Digits::init() {
    this->setDigitsBlank();
    this->initPins();
}

void Display7Sements4Digits::setDigitsValue(int value) {
    for (int i = 0; i < this->kNDigits; i++) {
        this->p_current_digits[i] = value;
    }
}

void Display7Sements4Digits::setDigitsBlank() {
    this->setDigitsValue(this->kBlankChar);
}

void Display7Sements4Digits::setDigitsError() {
    this->setDigitsValue(this->kErrorChar);
}

void Display7Sements4Digits::initPins() {
    for (int i = 0; i < this->kNSegments; i++) {
        pinMode(this->p_segment_pins[i], OUTPUT);
    }
}

void Display7Sements4Digits::setPins(int pin_map, int digit) {
    //Serial.print("\n--setPins--");
    //Serial.print(digit);
    //Serial.print(':');
    //Serial.print(pin_map, BIN);
    //Serial.print("--segments--");
    for (int i = 0; i < this->kNSegments; i++) {
         int state = (pin_map>>i) & 1;
         int pin = this->p_segment_pins[i];
         //Serial.print("p:");
         //Serial.print(pin);
         //Serial.print("s:");
         //Serial.print(state);
         if (state) {
             digitalWrite(pin, LOW);
         } else {
             digitalWrite(pin, HIGH);
         }
    }
    //Serial.print("--digits--");
    for (int i = 0; i < this->kNDigits; i++) {
        int state = (int)(digit == i);
        int pin = this->p_digit_pins[i];
        //Serial.print("sp:");
        //Serial.print(pin);
        //Serial.print("ss:");
        //Serial.print(state);
        if (state) {
            digitalWrite(pin, HIGH);
        } else {
            digitalWrite(pin, LOW);
        }
    }
}

int Display7Sements4Digits::findDigitPinMap(char ch) {
    int ch1 = (int) ch;
    for (int i = 0; i < this->kNChars; i++) {
        int ch2 = this->kCharsMap[i][0];
        if (ch1 == ch2) {
            return this->kCharsMap[i][1];
        }
    }
    return -1;
}

void Display7Sements4Digits::set(const char * p_str) {
    ////
    /// String aligned to the left
    /// 1234
    /// 1.2.3.4
    /// -1.2.3
    /// ....
    this->setDigitsBlank();

    int i = 0;
    int i_digit = 0;
    boolean was_dot = false;
    while(*p_str != '\0') {
        // Serial.print(*p_str);
        // Serial.print(":");
        boolean dot = *p_str == '.'? true: false;
        if (i > 0) {
            if (was_dot || (!was_dot && !dot)) {
                i_digit++;
            }
        }
        // Serial.print(i_digit);
        // Serial.print(";");
        if (i_digit >= this->kNDigits) {
            // TODO: SHOW ERROR
            break;
        }

        int digit_pin_map = this->findDigitPinMap(*p_str);
        if (digit_pin_map < 0) {
            this->setDigitsError();
            break;
        }

        this->p_current_digits[i_digit] = this->p_current_digits[i_digit] | digit_pin_map;

        was_dot = dot;
        p_str++;
        i++;
    }
}

void Display7Sements4Digits::refresh() {
    // Serial.print("\n--refresh--");
    // Serial.print(this->i_refresh);
    this->setPins(
        this->p_current_digits[this->i_refresh],
        this->kNDigits - 1 - this->i_refresh
    );
    this->i_refresh = (this->i_refresh + 1) % this->kNDigits;
}
