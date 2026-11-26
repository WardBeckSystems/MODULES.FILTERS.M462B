#include "PluginProcessor.hpp"
#include <cmath>

namespace wbs::m462b {

M462BPluginProcessor::M462BPluginProcessor()
    : engine::SharedPluginFrame("Ward-Beck M462B Parametric Equalizer", "M462B") {
    defaultOversampling_ = engine::OversamplingRate::TwoX;
}

void M462BPluginProcessor::prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) {
    for (size_t ch = 0; ch < numChannels; ++ch) {
        outputDriver_[ch].prepare(hostRate);
        lfBand_[ch].prepare(oversampledRate);
        lmfBand_[ch].prepare(oversampledRate);
        hmfBand_[ch].prepare(oversampledRate);
        hfBand_[ch].prepare(oversampledRate);
    }
    updateParametersFromHost();
}

void M462BPluginProcessor::releaseModule() {
    for (size_t ch = 0; ch < numChannels_; ++ch) {
        outputDriver_[ch].reset();
        lfBand_[ch].reset();
        lmfBand_[ch].reset();
        hmfBand_[ch].reset();
        hfBand_[ch].reset();
    }
}

void M462BPluginProcessor::updateParametersFromHost() noexcept {
    for (size_t ch = 0; ch < numChannels_; ++ch) {
        const auto lfType = (lfShape_ == FilterShape::Shelf) ? engine::FilterType::LowShelf : engine::FilterType::PeakingBell;
        lfBand_[ch].update(lfFreq_, 0.707, lfGain_, lfType);

        lmfBand_[ch].update(lmfFreq_, lmfQ_, lmfGain_, engine::FilterType::PeakingBell);
        hmfBand_[ch].update(hmfFreq_, hmfQ_, hmfGain_, engine::FilterType::PeakingBell);

        const auto hfType = (hfShape_ == FilterShape::Shelf) ? engine::FilterType::HighShelf : engine::FilterType::PeakingBell;
        hfBand_[ch].update(hfFreq_, 0.707, hfGain_, hfType);
    }
}

void M462BPluginProcessor::processModuleSlice(engine::AudioBlock& slice) noexcept {
    if (bypass_) return;

    for (size_t ch = 0; ch < slice.numChannels; ++ch) {
        float* channelData = slice.getChannelPointer(ch);

        for (size_t i = 0; i < slice.numSamples; ++i) {
            float s = channelData[i];

            // 4 cascaded ZDF state variable parametric stages
            s = lfBand_[ch].processSample(s);
            s = lmfBand_[ch].processSample(s);
            s = hmfBand_[ch].processSample(s);
            s = hfBand_[ch].processSample(s);

            // Output Class-A discrete driver with rail voltage limiting
            s = outputDriver_[ch].processSample(s, 1.0f, 0.20f);

            channelData[i] = s;
        }
    }
}

void M462BPluginProcessor::updateMeters(const engine::AudioBlock& audioBlock) noexcept {
    for (size_t ch = 0; ch < audioBlock.numChannels; ++ch) {
        const float* data = audioBlock.getChannelPointer(ch);
        float peak = 0.0f;
        for (size_t i = 0; i < audioBlock.numSamples; ++i) {
            peak = std::max(peak, std::abs(data[i]));
        }
        vuMeter_[ch] = (vuMeter_[ch] * 0.92f) + (peak * 0.08f);
    }
}

void M462BPluginProcessor::setLfParams(float freqHz, float gainDb, FilterShape shape) noexcept {
    lfFreq_ = std::clamp(freqHz, 30.0f, 400.0f);
    lfGain_ = std::clamp(gainDb, -15.0f, 15.0f);
    lfShape_ = shape;
}

void M462BPluginProcessor::setLmfParams(float freqHz, float gainDb, float q) noexcept {
    lmfFreq_ = std::clamp(freqHz, 100.0f, 1600.0f);
    lmfGain_ = std::clamp(gainDb, -15.0f, 15.0f);
    lmfQ_ = std::clamp(q, 0.5f, 3.0f);
}

void M462BPluginProcessor::setHmfParams(float freqHz, float gainDb, float q) noexcept {
    hmfFreq_ = std::clamp(freqHz, 800.0f, 12800.0f);
    hmfGain_ = std::clamp(gainDb, -15.0f, 15.0f);
    hmfQ_ = std::clamp(q, 0.5f, 3.0f);
}

void M462BPluginProcessor::setHfParams(float freqHz, float gainDb, FilterShape shape) noexcept {
    hfFreq_ = std::clamp(freqHz, 1500.0f, 18000.0f);
    hfGain_ = std::clamp(gainDb, -15.0f, 15.0f);
    hfShape_ = shape;
}

void M462BPluginProcessor::setRailVoltage(float volts) noexcept {
    railVoltage_ = std::clamp(volts, 15.0f, 25.0f);
}

void M462BPluginProcessor::setMasterBypass(bool bypassed) noexcept {
    bypass_ = bypassed;
}

} // namespace wbs::m462b
