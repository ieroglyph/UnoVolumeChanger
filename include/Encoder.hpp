#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

namespace Encoder
{

    struct Config
    {
        const uint8_t ClkPin;
        const uint8_t DtPin;
        const uint8_t Debounce;
        const uint8_t MinValue;
        const uint8_t MaxValue;
        const int8_t Step;
    };

    template <Config config>
    class Encoder
    {
    public:
        using ThisT = Encoder<config>;

        static void begin()
        {
            pinMode(config.ClkPin, INPUT_PULLUP);
            pinMode(config.DtPin, INPUT_PULLUP);
            attachInterrupt(digitalPinToInterrupt(config.ClkPin), ThisT::encoderChange,
                            RISING);
        }

        static void encoderChange()
        {
            if (const auto now = millis(); now - lastChange < config.Debounce)
                return;
            else
                lastChange = now;

            const bool ccw = digitalRead(config.DtPin) == HIGH;
            value += config.Step - ccw * 2 * config.Step;
            value = value < 0 ? 0 : value > 255 ? 255
                                                : value;
        };

        static unsigned char getValue()
        {
            return static_cast<unsigned char>(value & 0xff);
        }

    private:
        static unsigned long lastChange;
        static int16_t value;
    };

    template <Config config>
    unsigned long Encoder<config>::lastChange = 0;

    template <Config config>
    int16_t Encoder<config>::value = 0;
}

#endif // ENCODER_HPP
