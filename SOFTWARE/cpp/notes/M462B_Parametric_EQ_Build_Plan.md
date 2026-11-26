# Ward-Beck Systems M462B Parametric Equalizer - VST/AU Plugin Build Plan

**Module Name:** WBS M462B 4-Band Mastering & Channel Parametric Equalizer  
**Hardware Reference:** Ward-Beck Systems M462B Parametric Equalizer (Drawing `462-02-003`, ca. 1975–1981)  
**Target Architecture:** VST3 / AU / AAX (JUCE 7 / C++20)  
**Shared Engine Dependency:** [`wbs::SharedPluginFrame`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/SharedPluginFrame.hpp), [`wbs::StateVariableCore`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/StateVariableCore.hpp), [`wbs::NonLinearCore`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/NonLinearCore.hpp), [`wbs::OversamplingEngine`](file:///home/anthony/Documents/GitProjects/WBSps.ca/2030.env/engine/OversamplingEngine.hpp)

---

## 1. Hardware Architecture & Circuit Analysis

The M462B is Ward-Beck's premier 4-band analog parametric equalizer module, widely acclaimed for its clinical surgical ability combined with gentle musicality. It expands the 4-126 topology into dual cascaded state-variable filter boards running on stabilized $\pm22\text{ V}$ rails, affording $+28\text{ dBu}$ internal headroom before clipping.

### Section Breakdown
1. **Low Frequency (LF) Band:**
   - Frequency: Continuous $30\text{ Hz}$ to $400\text{ Hz}$.
   - Mode Switch: **Peak (Bell) / Shelf**.
   - Gain: $\pm15\text{ dB}$ continuous.
2. **Low-Mid Frequency (LMF) Band:**
   - Frequency: Continuous $100\text{ Hz}$ to $1.6\text{ kHz}$ (16:1 sweep ratio).
   - Bandwidth ($Q$): Continuous $0.5$ (broad musical) to $3.0$ (surgical notch).
   - Gain: $\pm15\text{ dB}$.
3. **High-Mid Frequency (HMF) Band:**
   - Frequency: Continuous $800\text{ Hz}$ to $12.8\text{ kHz}$.
   - Bandwidth ($Q$): Continuous $0.5$ to $3.0$.
   - Gain: $\pm15\text{ dB}$.
4. **High Frequency (HF) Band:**
   - Frequency: Continuous $1.5\text{ kHz}$ to $18.0\text{ kHz}$.
   - Mode Switch: **Peak (Bell) / Shelf**.
   - Gain: $\pm15\text{ dB}$.
5. **Output Stage:**
   - Balanced discrete output buffer with custom transformer emulation and $\pm22\text{ V}$ rail clipping dynamics.

---

## 2. DSP Architecture & Mathematical Model

The M462B implements a 4-stage cascaded Zero-Delay Feedback (ZDF) topology. In analog hardware, each stage contributes phase shift; our ZDF formulation models both the instantaneous amplitude shaping and phase rotation accurately.

### 2.1 Dual Continuous State-Variable Cores
For each band $k \in \{\text{LF, LMF, HMF, HF}\}$:
$$\begin{aligned}
u_k[n] &= x_k[n] - R_k \cdot s_{1,k}[n-1] - s_{2,k}[n-1] \\
y_{HP,k}[n] &= \frac{u_k[n]}{1 + R_k g_k + g_k^2} \\
y_{BP,k}[n] &= g_k \cdot y_{HP,k}[n] + s_{1,k}[n-1] \\
y_{LP,k}[n] &= g_k \cdot y_{BP,k}[n] + s_{2,k}[n-1]
\end{aligned}$$

For Bell mode:
$$y_{out,k}[n] = x_k[n] + (G_k - 1.0) \cdot y_{BP,k}[n]$$

For Shelf mode:
$$y_{out,k}[n] = x_k[n] + (G_k - 1.0) \cdot y_{LP,k}[n] \quad (\text{LF Shelf})$$
$$y_{out,k}[n] = x_k[n] + (G_k - 1.0) \cdot y_{HP,k}[n] \quad (\text{HF Shelf})$$

State updates:
$$s_{1,k}[n] = y_{BP,k}[n] + g_k \cdot y_{HP,k}[n]$$
$$s_{2,k}[n] = y_{LP,k}[n] + g_k \cdot y_{BP,k}[n]$$

---

## 3. Parameter Mapping & State Architecture

| ID | Parameter Name | Range | Steps / Type | Default | Unit |
|---|---|---|---|---|---|
| `m462b_lf_freq` | LF Frequency | $30.0$ to $400.0$ | Continuous (Log) | 80.0 | Hz |
| `m462b_lf_gain` | LF Gain | $-15.0$ to $+15.0$ | Continuous | 0.0 | dB |
| `m462b_lf_mode` | LF Mode | Shelf / Bell | 2-way switch | Shelf | Enum |
| `m462b_lmf_freq` | LMF Frequency | $100.0$ to $1600.0$ | Continuous (Log) | 400.0 | Hz |
| `m462b_lmf_gain` | LMF Gain | $-15.0$ to $+15.0$ | Continuous | 0.0 | dB |
| `m462b_lmf_q` | LMF Bandwidth Q | $0.5$ to $3.0$ | Continuous | 1.0 | Q |
| `m462b_hmf_freq` | HMF Frequency | $800.0$ to $12800.0$ | Continuous (Log) | 3000.0 | Hz |
| `m462b_hmf_gain` | HMF Gain | $-15.0$ to $+15.0$ | Continuous | 0.0 | dB |
| `m462b_hmf_q` | HMF Bandwidth Q | $0.5$ to $3.0$ | Continuous | 1.0 | Q |
| `m462b_hf_freq` | HF Frequency | $1500.0$ to $18000.0$ | Continuous (Log) | 10000.0 | Hz |
| `m462b_hf_gain` | HF Gain | $-15.0$ to $+15.0$ | Continuous | 0.0 | dB |
| `m462b_hf_mode` | HF Mode | Shelf / Bell | 2-way switch | Shelf | Enum |
| `m462b_rail_volt` | Power Rail Headroom | $\pm15\text{V}, \pm22\text{V}, \pm25\text{V}$ | 3-way switch | $\pm22\text{V}$ | Enum |
| `m462b_bypass` | Master EQ Bypass | Active / Bypassed | Toggle | Active | Bool |

---

## 4. C++ Plugin Scaffolding

```cpp
// M462BParametricProcessor.hpp
#pragma once
#include "SharedPluginFrame.hpp"
#include "StateVariableCore.hpp"
#include "NonLinearCore.hpp"
#include "OversamplingEngine.hpp"

namespace wbs {

class M462BParametricProcessor : public SharedPluginFrame {
public:
    M462BParametricProcessor();
    ~M462BParametricProcessor() override = default;

    const char* getPluginIdentifier() const noexcept override { return "WBS_M462B_Parametric_EQ"; }
    uint32_t getPluginVersion() const noexcept override { return 0x010000; }

    void prepareHardwareState(double sampleRate, uint32_t maxBlockSize) override;
    void resetHardwareState() override;
    void processAudioCore(AudioBlock<float>& block) override;

private:
    StateVariableCore lfCore_[2];
    StateVariableCore lmfCore_[2];
    StateVariableCore hmfCore_[2];
    StateVariableCore hfCore_[2];

    NonLinearCore outputDriver_;
    OversamplingEngine oversamplingEngine_;
};

} // namespace wbs
```

---

## 5. UI/UX Specification

- **Module Format:** Dual-slot rack chassis or channel bay module.
- **Knob Clusters:** 4 distinct color-coded bands (LF: Red, LMF: Yellow, HMF: Light Blue, HF: Dark Blue) with concentric inner gain / outer frequency rings.
- **Spectrum Display:** Optional switchable vintage green phosphor FFT analyzer overlay matching 1980s Tektronix studio scopes.

---

## 6. Verification & Test Plan

1. **Overlap Response Check:** Set LMF to $1.2\text{ kHz}$ ($+6\text{ dB}$) and HMF to $1.2\text{ kHz}$ ($-6\text{ dB}$) with matching $Q$; verify total phase cancellation and flat sum response $\pm0.1\text{ dB}$.
2. **Headroom Clamping:** Switch between $\pm15\text{V}$ ($+22\text{ dBu}$ clip) and $\pm25\text{V}$ ($+28\text{ dBu}$ clip); verify dynamic clipping thresholds precisely match rail voltage limits.
3. **Sample Rate Invariance:** Compare frequency magnitude response across $44.1\text{ kHz}, 96\text{ kHz}, 192\text{ kHz}$; peak error must be $< 0.05\text{ dB}$.
