#include <Arduino.h>
#include <LedStrip.hpp>
#include <Encoder.hpp>
#include <Button.hpp>

namespace
{
  constexpr auto LedPin1{11};
  constexpr auto LedPin2{10};
  constexpr auto LedPin3{9};

  constexpr Encoder::Config EncoderConfig{
      .ClkPin{2},
      .DtPin{4},
      .Debounce{100},
      .MinValue{0},
      .MaxValue{255},
      .Step{25}};

  constexpr Button::Config ButtonConfig{
      .Pin{3},
      .DebounceTime{100}};

  using LedStripT = LedStrip::LedStrip<LedPin1, LedPin2, LedPin3>;
  using EncoderT = Encoder::Encoder<EncoderConfig>;
  using ButtonT = Button::Button<ButtonConfig>;
}

::LedStripT ledStrip;
::EncoderT encoder;
::ButtonT button;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  ledStrip.begin();
  ledStrip.setMaxBrightness(255);
  ledStrip.setMode(LedStrip::Mode::Bar);

  encoder.begin();

  button.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  static int c{0};
  static int increment{4};
  using LedStrip::Mode;
  static Mode mode{Mode::Bar};
  static auto lastUpdate{millis()};

  ledStrip.setValue(encoder.getValue());
  if (button.pressedTime() > 0)
  {
    mode = mode == Mode::Position ? Mode::Union : mode == Mode::Union ? Mode::Bar
                                                                      : Mode::Position;
    ledStrip.setMode(mode);
  }
  Serial.println(encoder.getValue());

  /*
    if (const auto now{millis()}; now - lastUpdate > 15)
    {
      lastUpdate = now;
      ledStrip.setValue(c);
      if (c > 250)
        increment = -1;
      c += increment;
      if (c < 5)
      {
        increment = 1;

        mode = mode == Mode::Position ? Mode::Union : mode == Mode::Union ? Mode::Bar
                                                                          : Mode::Position;

        ledStrip.setMode(mode);
      }
    }
    */

  ledStrip.loop();
}
