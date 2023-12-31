#pragma once

#include "Light.h"

namespace IG {
class SunLight : public Light {
public:
    SunLight(const std::string& name, const std::shared_ptr<Parser::Object>& light);

    virtual bool isInfinite() const override { return true; }
    virtual bool isDelta() const override { return true; }
    virtual std::optional<Vector3f> direction() const override { return mDirection; }
    
    virtual float computeFlux(const ShadingTree&) const override;
    virtual void serialize(const SerializationInput& input) const override;

private:
    Vector3f mDirection;

    std::shared_ptr<Parser::Object> mLight;
};
} // namespace IG