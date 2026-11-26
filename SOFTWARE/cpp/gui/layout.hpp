// M462B: its block of 2030.env/vst/core/ModuleGuiLayouts.hpp (176 x 770 = the faceplate).

        case 3: { // M462B Parametric EQ (176 x 770, matching M462B.jpg)
            m.title = "M462B Parametric EQ";
            m.subtitle = "4-Band Fully Parametric Console Equalizer";
            m.modelCode = "M462B";

            // Band 1: High (Top is Red)
            m.knobs.push_back({ 9, "HF FREQ", "kHz", 50.0f, 90.0f, 20.0f, 3.0, 16.0, 10.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaRed });
            m.knobs.push_back({ 10, "HF GAIN", "dB", 126.0f, 90.0f, 20.0f, -14.0, 14.0, 0.0, false, KnobStyle::WbsElmaWing, Palette::ElmaRed });

            // Band 2: High-Mid (High-mid is Orange / Amber)
            m.knobs.push_back({ 6, "HMF FREQ", "Hz", 50.0f, 180.0f, 20.0f, 800.0, 8000.0, 3000.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaAmber });
            m.knobs.push_back({ 7, "HMF GAIN", "dB", 126.0f, 180.0f, 20.0f, -14.0, 14.0, 0.0, false, KnobStyle::WbsElmaWing, Palette::ElmaAmber });
            m.knobs.push_back({ 8, "HMF Q", "", 88.0f, 245.0f, 17.0f, 0.5, 3.0, 1.2, false, KnobStyle::WbsElmaRegular, Palette::ElmaSlate });

            // Band 3: Low-Mid (Mid is Yellow)
            m.knobs.push_back({ 3, "LMF FREQ", "Hz", 50.0f, 330.0f, 20.0f, 150.0, 2000.0, 500.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaYellow });
            m.knobs.push_back({ 4, "LMF GAIN", "dB", 126.0f, 330.0f, 20.0f, -14.0, 14.0, 0.0, false, KnobStyle::WbsElmaWing, Palette::ElmaYellow });
            m.knobs.push_back({ 5, "LMF Q", "", 88.0f, 395.0f, 17.0f, 0.5, 3.0, 1.2, false, KnobStyle::WbsElmaRegular, Palette::ElmaSlate });

            // Band 4: Low (Low is Green)
            m.knobs.push_back({ 0, "LF FREQ", "Hz", 50.0f, 480.0f, 20.0f, 30.0, 400.0, 80.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaGreen });
            m.knobs.push_back({ 1, "LF GAIN", "dB", 126.0f, 480.0f, 20.0f, -14.0, 14.0, 0.0, false, KnobStyle::WbsElmaWing, Palette::ElmaGreen });

            // EQ IN Button - RED button is eq ON
            m.buttons.push_back({ 12, "EQ IN", { 43.0f, 570.0f, 90.0f, 28.0f }, true, ButtonStyle::LatchingPush, Palette::LedRed });

            m.themeToggleRect = { 25.0f, 726.0f, 126.0f, 22.0f };
            break;
        }
