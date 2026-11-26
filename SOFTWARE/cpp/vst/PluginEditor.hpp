#pragma once
#include "PluginProcessor.hpp"

namespace wbs::m462b {

class M462BPluginEditor {
public:
    explicit M462BPluginEditor(M462BPluginProcessor& processor)
        : processor_(processor) {}

    void paint() const noexcept;
    void resized() noexcept;

    int getWidth() const noexcept { return 620; }
    int getHeight() const noexcept { return 380; }

private:
    M462BPluginProcessor& processor_;
};

} // namespace wbs::m462b
