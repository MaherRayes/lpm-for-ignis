# Lightweight Photon mapping for Ignis

This is an implementation of Multiple Importance Sampling and the paper [Lightweight Photon Mapping](https://cgg.mff.cuni.cz/~jaroslav/papers/2018-lwpm/index.htm) for [Ignis](https://github.com/PearCoding/Ignis). The implementation uses histogram as data structure for the area lights.

The Ignis verison used is from 11 December 2022. 

# Changes

Following files were changed to accommodate the new algorithm:

modified:   ../../../../src/artic/driver/driver.art

modified:   ../../../../src/artic/driver/light.art

modified:   ../../../../src/artic/light/area.art

modified:   ../../../../src/artic/light/directional.art

modified:   ../../../../src/artic/light/env.art

modified:   ../../../../src/artic/light/light_selector.art

modified:   ../../../../src/artic/light/point.art

modified:   ../../../../src/artic/light/spot.art

modified:   ../../../../src/artic/light/sun.art

modified:   ../../../../src/runtime/device/Device.cpp

modified:   ../../../../src/runtime/light/AreaLight.cpp

modified:   ../../../../src/runtime/loader/LoaderLight.cpp

modified:   ../../../../src/runtime/loader/LoaderLight.h

modified:   ../../../../src/runtime/loader/LoaderTechnique.cpp

modified:   ../../../../src/runtime/loader/LoaderTechnique.h


Followin files were added to the project:

../../../../src/artic/core/histogram.art

../../../../src/artic/technique/lightphotonmapper.art

../../../../src/runtime/technique/LightPhotonMappingTechnique.cpp

../../../../src/runtime/technique/LightPhotonMappingTechnique.h


# Issues and Limitations

Current version of the code requires manual change of the histogram size to fit each scene. This can be done in the file "LoaderLight.cpp" line 158.

There is a bug in scenes with caustics where you can see light artifacts around the scene (it can be recreated using the diamond scene). It seems to be connected to the direction dimensions of the histogram but fixes haven't been found yet.
