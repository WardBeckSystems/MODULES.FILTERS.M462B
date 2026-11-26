// M462B: its block of 2030.env/vst/core/CircuitDiagramDefinitions.hpp.

        case 3: // M462B Parametric EQ
            d.modelCode = "M462B";
            d.circuitTitle = "Ward-Beck M462B 4-Band Fully Parametric Console EQ";
            d.theoreticalExplanation =
                "Flagship 4-band Zero-Delay Feedback (ZDF) state-variable equalizer. Continuous frequency, gain, and Q control "
                "on LMF and HMF, with switchable bell/shelf on LF and HF. Powered by dynamic console power rail voltage modeling (±15V / ±22V / ±25V).";
            d.nodes = {
                { "IN",  "Line In",     "Transformer",   { 15, 20, 80, 48 }, true, Palette::CopperWire },
                { "LF",  "LF Param",    "30-400Hz Sh/Bl",{ 105, 20, 95, 48 }, true, Palette::TextGold },
                { "LMF", "LMF Param",   "100-1600Hz Q",  { 210, 20, 95, 48 }, true, Palette::TextGold },
                { "HMF", "HMF Param",   "800-12800Hz Q", { 315, 20, 95, 48 }, true, Palette::TextGold },
                { "HF",  "HF Param",    "1.5k-18kHz Sh/Bl",{ 420, 20, 95, 48 }, true, Palette::TextGold },
                { "OUT", "Rail Out",    "±15/22/25V PSU", { 525, 20, 85, 48 }, true, Palette::LedGreen }
            };
            d.wires = {
                { "IN", "LF", "" },
                { "LF", "LMF", "" },
                { "LMF", "HMF", "" },
                { "HMF", "HF", "" },
                { "HF", "OUT", "Rail Clip" }
            };
            break;
