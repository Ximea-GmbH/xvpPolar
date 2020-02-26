#include "polarplugin.h"
#include "polartohsvchnbl.h"
#include "degreeofpolarizationchnbl.h"
#include <xiCoreVersion.h>
#include <xiRtti.h>

bool CxPolarPlugin::init()
{
    xiRTTI_REGISTER(this, CxPolarToHSVChnbl, "CxChainable");
    xiRTTI_REGISTER(this, CxDegreeOfPolarizationChnbl, "CxChainable")

    return true;
}

QString CxPolarPlugin::copyrightNotice() const
{
   return tr("Copyright (C) 2020, Ximea GmbH");
}

QString CxPolarPlugin::name() const{
    return tr("Polar Plugin");
}

QString CxPolarPlugin::author() const {
   return tr("XIMEA GmbH");
}

QString CxPolarPlugin::description() const {
    return tr("Plugin containing processing chain objects for cameras with Sony Polarsens sensors.");
}

QString CxPolarPlugin::version() const {
    return tr("0.1.0");
}

void CxPolarPlugin::version(int &major, int &minor, int &build) const
{
   major = 0;
   minor = 1;
   build = 0;
}
