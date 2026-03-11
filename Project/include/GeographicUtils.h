#ifndef GEOGRAPHICUTILS_H
#define GEOGRAPHICUTILS_H

#include "StreetMap.h"
#include <vector>
#include <string>

struct SGeographicUtils{
    static double DegreesToRadians(double deg);
    static double RadiansToDegrees(double rad);
    static double Normalize360(double deg);
    static double Normalize180180(double deg);
    static double HaversineDistanceInMiles(CStreetMap::TLocation loc1, CStreetMap::TLocation loc2);
    static double CalculateBearing(CStreetMap::TLocation src, CStreetMap::TLocation dest);
    static double CalculateExternalBisector(double bear1, double bear2);
    static bool CalculateExtents(const std::vector<CStreetMap::TLocation> &locations, CStreetMap::TLocation &lowerleft, CStreetMap::TLocation &upperright);
    static std::vector<CStreetMap::TLocation> FilterLocations(const std::vector<CStreetMap::TLocation> &locations, const CStreetMap::TLocation &lowerleft, const CStreetMap::TLocation &upperright);
    static std::string BearingToDirection(double bearing);
    static std::string CalcualteExternalBisectorDirection(CStreetMap::TLocation src, CStreetMap::TLocation mid, CStreetMap::TLocation dest);
    static std::string ConvertLLToDMS(CStreetMap::TLocation loc);
};

#endif
