// Copyright (c) 2026 Ward-Beck Systems (Ward-Beck.com - Ward-Beck.ca). All rights reserved.
// M462B: its part of 2030.env/vst/clap_wrapper/ClapModuleRegistry.cpp (suite index 3).

// --- descriptor ---
static const clap_plugin_descriptor_t s_desc_m462b = {
    CLAP_VERSION,
    "ca.wbsps.m462b_parametric_eq",
    "Ward-Beck M462B Parametric Equalizer",
    "Ward-Beck Systems Preservation Society",
    "https://wbsps.ca",
    "",
    "https://wbsps.ca/contact",
    "1.0.0",
    "Fully parametric 4-band broadcast console equalizer with rail voltage sag modeling",
    kEqFeatures
};

// --- parameters ---
std::vector<ParameterInfo> buildM462BParams() {
    std::vector<ParameterInfo> p;
    p.push_back({
        0, "LF Frequency", "LF Band", 30.0, 400.0, 80.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setLfFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        1, "LF Gain", "LF Band", -15.0, 15.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setLfGain(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        2, "LF Curve Shape", "LF Band", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) {
            static_cast<m462b::M462BPluginProcessor*>(f)->setLfShape(val >= 0.5 ? m462b::FilterShape::Bell : m462b::FilterShape::Shelf);
        },
        nullptr,
        [](double v) { return v >= 0.5 ? "Bell" : "Shelf"; },
        nullptr
    });
    p.push_back({
        3, "LMF Frequency", "LMF Band", 100.0, 1600.0, 400.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setLmfFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        4, "LMF Gain", "LMF Band", -15.0, 15.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setLmfGain(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        5, "LMF Q", "LMF Band", 0.5, 3.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setLmfQ(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.2f", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        6, "HMF Frequency", "HMF Band", 800.0, 12800.0, 3000.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setHmfFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        7, "HMF Gain", "HMF Band", -15.0, 15.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setHmfGain(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        8, "HMF Q", "HMF Band", 0.5, 3.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setHmfQ(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.2f", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        9, "HF Frequency", "HF Band", 1500.0, 18000.0, 10000.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setHfFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        10, "HF Gain", "HF Band", -15.0, 15.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setHfGain(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%+.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        11, "HF Curve Shape", "HF Band", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) {
            static_cast<m462b::M462BPluginProcessor*>(f)->setHfShape(val >= 0.5 ? m462b::FilterShape::Bell : m462b::FilterShape::Shelf);
        },
        nullptr,
        [](double v) { return v >= 0.5 ? "Bell" : "Shelf"; },
        nullptr
    });
    p.push_back({
        12, "Rail Voltage", "Power", 15.0, 25.0, 22.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setRailVoltage(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "±%.1f V", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        13, "Master Bypass", "Master", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_BYPASS,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m462b::M462BPluginProcessor*>(f)->setMasterBypass(val >= 0.5); },
        nullptr,
        [](double v) { return v >= 0.5 ? "Bypassed" : "Active"; },
        nullptr
    });
    return p;
}

// --- instance ---
        case 3:
            inst = new ClapPluginInstance(
                &s_desc_m462b, host,
                []() { return std::make_unique<m462b::M462BPluginProcessor>(); },
                buildM462BParams(),
                3
            );
            break;

