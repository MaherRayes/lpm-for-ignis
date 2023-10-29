#pragma once

#include "Technique.h"

namespace IG {
class LightPhotonMappingTechnique : public Technique {
public:
    LightPhotonMappingTechnique(const Parser::Object& obj);
    ~LightPhotonMappingTechnique() = default;

    bool hasDenoiserSupport() const override { return true; }
    
    TechniqueInfo getInfo(const LoaderContext& ctx) const override;
    void generateBody(const SerializationInput& input) const override;

private:
    size_t mPhotonCount;
    size_t mMaxCameraDepth;
    size_t mMaxLightDepth;
    std::string mLightSelector;
    float mMergeRadius;
    float mClamp;
    bool mAOV;
};
} // namespace IG