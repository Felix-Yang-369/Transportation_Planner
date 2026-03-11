#include "GeographicUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

double SGeographicUtils::DegreesToRadians(double deg){
    return M_PI * deg / 180.0;
}

double SGeographicUtils::RadiansToDegrees(double rad){
    return 180.0 * rad / M_PI;
}

double SGeographicUtils::Normalize360(double deg){
    deg = fmod(deg, 360.0);
    if(deg < 0){
        deg += 360.0;
    }
    return deg;
}

double SGeographicUtils::Normalize180180(double deg){
    deg = Normalize360(deg);
    if(deg > 180.0){
        deg -= 360.0;
    }
    return deg;
}

double SGeographicUtils::HaversineDistanceInMiles(CStreetMap::TLocation loc1, CStreetMap::TLocation loc2){
    double LatRad1 = DegreesToRadians(loc1.first);
    double LatRad2 = DegreesToRadians(loc2.first);
    double LonRad1 = DegreesToRadians(loc1.second);
    double LonRad2 = DegreesToRadians(loc2.second);
    double DeltaLat = LatRad2 - LatRad1;
    double DeltaLon = LonRad2 - LonRad1;
    double DeltaLatSin = sin(DeltaLat / 2);
    double DeltaLonSin = sin(DeltaLon / 2);
    double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
    const double EarthRadiusMiles = 3959.88;

    return 2 * EarthRadiusMiles * Computation;
}

double SGeographicUtils::CalculateBearing(CStreetMap::TLocation src, CStreetMap::TLocation dest){
    double LatRad1 = DegreesToRadians(src.first);
    double LatRad2 = DegreesToRadians(dest.first);
    double LonRad1 = DegreesToRadians(src.second);
    double LonRad2 = DegreesToRadians(dest.second);
    double X = cos(LatRad2) * sin(LonRad2 - LonRad1);
    double Y = cos(LatRad1) * sin(LatRad2) - sin(LatRad1) * cos(LatRad2) * cos(LonRad2 - LonRad1);
    return RadiansToDegrees(atan2(X, Y));
}

double SGeographicUtils::CalculateExternalBisector(double bear1, double bear2){
    bear1 = Normalize360(bear1);
    bear2 = Normalize360(bear2);

    double Difference = Normalize180180(bear2 - bear1 + 360.0);
    auto NearStraight = (Difference <= -178.5) || (Difference >= 178.5);

    double InternalMid = Normalize360(bear1 + Difference / 2 + 360.0);
    double ExternalMid = Normalize360(InternalMid + 180.0);

    if(NearStraight && ((ExternalMid < 45.0) || (ExternalMid > 225.0))){
        ExternalMid = InternalMid;
    }
    return Normalize180180(ExternalMid);
}

std::string SGeographicUtils::BearingToDirection(double bearing){
    bearing = Normalize180180(bearing);
    if(-22.5 <= bearing){
        if(67.5 > bearing){
            if(22.5 >= bearing){
                return "N";
            }
            return "NE";
        }
        else{
            if(112.5 >= bearing){
                return "E";
            }
            else if(157.5 >= bearing){
                return "SE";
            }
            return "S";
        }
    }
    else{
        if(-112.5 <= bearing){
            if(-67.5 >= bearing){
                return "W";
            }
            return "NW";
        }
        else{
            if(-157.5 >= bearing){
                return "S";
            }
            return "SW";
        }
    }
}

std::string SGeographicUtils::CalcualteExternalBisectorDirection(CStreetMap::TLocation src, CStreetMap::TLocation mid, CStreetMap::TLocation dest){
    double Bearing1 = CalculateBearing(mid, src);
    double Bearing2 = CalculateBearing(mid, dest);
    double BisectorBearing;

    if(src == dest){
        BisectorBearing = Normalize180180(Bearing2 + 180.0);
    }
    else{
        BisectorBearing = CalculateExternalBisector(Bearing1, Bearing2);
    }
    return BearingToDirection(BisectorBearing);
}

bool SGeographicUtils::CalculateExtents(const std::vector<CStreetMap::TLocation> &locations, CStreetMap::TLocation &lowerleft, CStreetMap::TLocation &upperright){
    lowerleft.first = 90.0;
    lowerleft.second = 180.0;
    upperright.first = -90.0;
    upperright.second = -180.0;

    for(const auto &Location : locations){
        if(lowerleft.first > Location.first){
            lowerleft.first = Location.first;
        }
        if(upperright.first < Location.first){
            upperright.first = Location.first;
        }
        if(lowerleft.second > Location.second){
            lowerleft.second = Location.second;
        }
        if(upperright.second < Location.second){
            upperright.second = Location.second;
        }
    }
    return !locations.empty();
}

std::vector<CStreetMap::TLocation> SGeographicUtils::FilterLocations(const std::vector<CStreetMap::TLocation> &locations, const CStreetMap::TLocation &lowerleft, const CStreetMap::TLocation &upperright){
    std::vector<CStreetMap::TLocation> Filtered;
    for(const auto &Location : locations){
        if((Location.first < lowerleft.first) || (Location.second < lowerleft.second) ||
           (Location.first > upperright.first) || (Location.second > upperright.second)){
            continue;
        }
        Filtered.push_back(Location);
    }
    return Filtered;
}

std::string SGeographicUtils::ConvertLLToDMS(CStreetMap::TLocation loc){
    double Lat = loc.first;
    double Lon = loc.second;
    double LatAbs = fabs(Lat);
    double LonAbs = fabs(Lon);
    std::stringstream OutStream;

    OutStream << (int)LatAbs << "d ";
    double Remainder = LatAbs - (int)LatAbs;
    Remainder *= 60.0;
    OutStream << (int)Remainder << "' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream << std::setprecision(4) << std::noshowpoint << Remainder << (Lat < 0.0 ? "\" S, " : "\" N, ");

    OutStream << (int)LonAbs << "d ";
    Remainder = LonAbs - (int)LonAbs;
    Remainder *= 60.0;
    OutStream << (int)Remainder << "' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream << Remainder << (Lon < 0.0 ? "\" W" : "\" E");

    return OutStream.str();
}
