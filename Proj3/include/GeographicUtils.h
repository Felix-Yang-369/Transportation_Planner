#ifndef GEOGRAPHICUTILS_H
#define GEOGRAPHICUTILS_H

#include "StreetMap.h"
#include <vector>

struct SGeographicUtils{
    static double DegreesToRadians(double deg);
    static double RadiansToDegrees(double rad);
    static double Normalize360(double deg);
    static double Normalize180180(double deg);
    static double HaversineDistanceInMiles(CStreetMap::SLocation loc1, CStreetMap::SLocation loc2);
    static double CalculateBearing(CStreetMap::SLocation src, CStreetMap::SLocation dest);
    static double CalculateExternalBisector(double bear1, double bear2);
    static bool CalculateExtents(const std::vector<CStreetMap::SLocation> &locations, CStreetMap::SLocation &lowerleft, CStreetMap::SLocation &upperright);
    static std::vector<CStreetMap::SLocation> FilterLocations(const std::vector<CStreetMap::SLocation> &locations, const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright);
    static std::string BearingToDirection(double bearing);
    static std::string CalcualteExternalBisectorDirection(CStreetMap::SLocation src, CStreetMap::SLocation mid, CStreetMap::SLocation dest);
    static std::string ConvertLLToDMS(CStreetMap::SLocation loc);
};

#endif
