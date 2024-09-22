#ifndef LED_STRIP_HPP
#define LED_STRIP_HPP

#include <Arduino.h>
#include <stddef.h>

namespace LedStrip
{

    struct LedValue
    {
        unsigned char value{0};
    };

    enum class Mode
    {
        Union,
        Bar,
        Position
    };

    template <uint8_t... pins>
    class LedStrip
    {
    public:
        static constexpr size_t ledCount{sizeof...(pins)};

        constexpr LedStrip()
        {
            size_t i{0};
            ((_leds[i++].value = pins ^ pins), ...);
        }

        void begin()
        {
            ((pinMode(pins, OUTPUT), analogWrite(pins, 0)), ...);
        }

        void loop()
        {
            if (!_hasChanged)
                return;

            size_t i{0};
            ((analogWrite(pins, _leds[i++].value)), ...);
        }

        void setValue(unsigned char value)
        {
            auto getValue{
                [value, this](auto i) -> unsigned char
                {
                    int val = static_cast<int>(value) * static_cast<int>(ledCount);
                    int dist = static_cast<int>(i) * 255 - val;
                    if (dist > 255)
                        return 0;
                    if (dist < -255)
                        return 255;
                    return abs(dist);
                }};

            _hasChanged = true;

            if (_mode == Mode::Union)
            {
                setUnionBrightness(value);
                return;
            }

            size_t i{0};
            ((_leds[i].value = getValue(i) + (pins ^ pins), i++), ...);
        }

        void setMode(Mode mode)
        {
            _mode = mode;
        }

        void setMaxBrightness(unsigned char value)
        {
            _maxBrightness = value;
            _hasChanged = true;
        }

    private:
        unsigned char scaledBrightness(unsigned char value) const
        {
            const auto highMaxBrightness{static_cast<uint16_t>(_maxBrightness)};
            const auto highValue{static_cast<uint16_t>(value)};
            const auto highResult{(highValue * highMaxBrightness) / 255};
            const auto result{static_cast<unsigned char>(highResult)};
            return result;
        }

        void setUnionBrightness(unsigned char value)
        {
            size_t i{0};
            const auto scaledValue{scaledBrightness(value)};
            ((_leds[i++].value = scaledValue + (pins ^ pins)), ...);
        }

        LedValue _leds[ledCount]{};
        unsigned char _maxBrightness{255};
        bool _hasChanged{false};
        Mode _mode{Mode::Union};
    };
}

#endif // LED_GRID_HPP
