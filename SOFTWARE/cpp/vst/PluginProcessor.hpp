#pragma once
#include "../core/SharedPluginFrame.hpp"
#include "../core/StateVariableCore.hpp"
#include "../core/NonLinearCore.hpp"

namespace wbs::m462b {

enum class FilterShape : uint8_t {
    Shelf = 0,
    Bell
};

class M462BPluginProcessor : public engine::SharedPluginFrame {
public:
    M462BPluginProcessor();
    ~M462BPluginProcessor() override = default;

    void prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) override;
    void releaseModule() override;
    void updateParametersFromHost() noexcept override;
    void processModuleSlice(engine::AudioBlock& slice) noexcept override;
    void updateMeters(const engine::AudioBlock& audioBlock) noexcept override;

    // Parameter setters
    void setLfParams(float freqHz, float gainDb, FilterShape shape) noexcept;
    void setLfFreq(float freqHz) noexcept { lfFreq_ = std::clamp(freqHz, 30.0f, 400.0f); }
    void setLfGain(float gainDb) noexcept { lfGain_ = std::clamp(gainDb, -15.0f, 15.0f); }
    void setLfShape(FilterShape shape) noexcept { lfShape_ = shape; }
    void setLmfParams(float freqHz, float gainDb, float q) noexcept;
    void setLmfFreq(float freqHz) noexcept { lmfFreq_ = std::clamp(freqHz, 100.0f, 1600.0f); }
    void setLmfGain(float gainDb) noexcept { lmfGain_ = std::clamp(gainDb, -15.0f, 15.0f); }
    void setLmfQ(float q) noexcept { lmfQ_ = std::clamp(q, 0.5f, 3.0f); }
    void setHmfParams(float freqHz, float gainDb, float q) noexcept;
    void setHmfFreq(float freqHz) noexcept { hmfFreq_ = std::clamp(freqHz, 800.0f, 12800.0f); }
    void setHmfGain(float gainDb) noexcept { hmfGain_ = std::clamp(gainDb, -15.0f, 15.0f); }
    void setHmfQ(float q) noexcept { hmfQ_ = std::clamp(q, 0.5f, 3.0f); }
    void setHfParams(float freqHz, float gainDb, FilterShape shape) noexcept;
    void setHfFreq(float freqHz) noexcept { hfFreq_ = std::clamp(freqHz, 1500.0f, 18000.0f); }
    void setHfGain(float gainDb) noexcept { hfGain_ = std::clamp(gainDb, -15.0f, 15.0f); }
    void setHfShape(FilterShape shape) noexcept { hfShape_ = shape; }
    void setRailVoltage(float volts) noexcept; // 15.0, 22.0, 25.0
    void setMasterBypass(bool bypassed) noexcept;

private:
    engine::StateVariableFilterCore lfBand_[engine::kMaxChannels];
    engine::StateVariableFilterCore lmfBand_[engine::kMaxChannels];
    engine::StateVariableFilterCore hmfBand_[engine::kMaxChannels];
    engine::StateVariableFilterCore hfBand_[engine::kMaxChannels];
    engine::NonLinearTransformerCore outputDriver_[engine::kMaxChannels];

    float lfFreq_{ 80.0f };
    float lfGain_{ 0.0f };
    FilterShape lfShape_{ FilterShape::Shelf };

    float lmfFreq_{ 400.0f };
    float lmfGain_{ 0.0f };
    float lmfQ_{ 1.0f };

    float hmfFreq_{ 3000.0f };
    float hmfGain_{ 0.0f };
    float hmfQ_{ 1.0f };

    float hfFreq_{ 10000.0f };
    float hfGain_{ 0.0f };
    FilterShape hfShape_{ FilterShape::Shelf };

    float railVoltage_{ 22.0f };
    bool bypass_{ false };

    float vuMeter_[engine::kMaxChannels]{ 0.0f };
};

} // namespace wbs::m462b
