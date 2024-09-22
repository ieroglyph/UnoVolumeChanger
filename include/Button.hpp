#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

namespace Button
{
    struct Config
    {
        const uint8_t Pin;
        const uint32_t DebounceTime;
    };
    template <Config config>
    class Button
    {
    public:
        using ThisT = Button;

        static void begin()
        {
            pinMode(config.Pin, INPUT_PULLUP);
            attachInterrupt(digitalPinToInterrupt(config.Pin), ThisT::interrupt, CHANGE);
        }

        static void interrupt()
        {
            static unsigned long lastChange;
            const auto now = millis();

            if (now - lastChange < config.DebounceTime)
                return;
            else
                lastChange = now;

            if (digitalRead(config.Pin) == LOW)
            {
                pushTime = now;
                releaseTime = now;
            }
            else
            {
                releaseTime = now;
            }
        }

        static unsigned long pressedTime()
        {
            if (!releaseTime > pushTime)
                return 0;
            const auto result = releaseTime - pushTime;
            releaseTime = 0;
            pushTime = 0;
            return result;
        }

    private:
        static unsigned long pushTime;
        static unsigned long releaseTime;
    };

    template <Config config>
    unsigned long Button<config>::pushTime = 0;
    template <Config config>
    unsigned long Button<config>::releaseTime = 0;
}

#endif // BUTTON_HPP
