#include "GeographicUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

double SGeographicUtils::DegreesToRadians(double deg){
    return M_PI * (deg) / 180.0;
}

double SGeographicUtils::RadiansToDegrees(double rad){
    return 180.0 * (rad) / M_PI;
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

double SGeographicUtils::HaversineDistanceInMiles(CStreetMap::SLocation loc1, CStreetMap::SLocation loc2){
    double LatRad1 = DegreesToRadians(loc1.DLatitude);
    double LatRad2 = DegreesToRadians(loc2.DLatitude);
    double LonRad1 = DegreesToRadians(loc1.DLongitude);
    double LonRad2 = DegreesToRadians(loc2.DLongitude);
    double DeltaLat = LatRad2 - LatRad1;
    double DeltaLon = LonRad2 - LonRad1;
    double DeltaLatSin = sin(DeltaLat/2);
    double DeltaLonSin = sin(DeltaLon/2);
    double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
    const double EarthRadiusMiles = 3959.88;

    return 2 * EarthRadiusMiles * Computation;
}

double SGeographicUtils::CalculateBearing(CStreetMap::SLocation src, CStreetMap::SLocation dest){
    double LatRad1 = DegreesToRadians(src.DLatitude);
    double LatRad2 = DegreesToRadians(dest.DLatitude);
    double LonRad1 = DegreesToRadians(src.DLongitude);
    double LonRad2 = DegreesToRadians(dest.DLongitude);
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}

#include <iostream>
using std::cout;
using std::endl;

double SGeographicUtils::CalculateExternalBisector(double bear1, double bear2){
    bear1 = Normalize360(bear1);
    bear2 = Normalize360(bear2);

    double Difference = Normalize180180(bear2 - bear1 + 360.0);   // shortest signed angle
    auto NearStraight = (Difference <= -178.5) || (Difference >= 178.5);
    // Internal bisector
    double InternalMid = Normalize360(bear1 + Difference/2 + 360.0);
    // External bisector is opposite direction
    double ExternalMid = Normalize360(InternalMid + 180.0);
    //ExternalMid = bear1 + Difference * 0.5;
    if(NearStraight && ((ExternalMid < 45.0)||(ExternalMid > 225.0))){
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

std::string SGeographicUtils::CalcualteExternalBisectorDirection(CStreetMap::SLocation src, CStreetMap::SLocation mid, CStreetMap::SLocation dest){
    double Bearing1 = CalculateBearing(mid,src);
    double Bearing2 = CalculateBearing(mid,dest);
    double BisectorBearing;
    if(src == dest){
        BisectorBearing = Normalize180180(Bearing2 + 180.0);
    }
    else{
        BisectorBearing = CalculateExternalBisector(Bearing1,Bearing2);
    }
    return BearingToDirection(BisectorBearing);
}

bool SGeographicUtils::CalculateExtents(const std::vector<CStreetMap::SLocation> &locations, CStreetMap::SLocation &lowerleft, CStreetMap::SLocation &upperright){
    lowerleft.DLatitude = 90.0;
    lowerleft.DLongitude = 180.0;
    upperright.DLatitude = -90.0;
    upperright.DLongitude = -180.0;

    for(auto &Location : locations){
        if(lowerleft.DLatitude > Location.DLatitude){
            lowerleft.DLatitude = Location.DLatitude;
        }
        if(upperright.DLatitude < Location.DLatitude){
            upperright.DLatitude = Location.DLatitude;
        }
        if(lowerleft.DLongitude > Location.DLongitude){
            lowerleft.DLongitude = Location.DLongitude;
        }
        if(upperright.DLongitude < Location.DLongitude){
            upperright.DLongitude = Location.DLongitude;
        }
    }
    return !locations.empty();
}

std::vector<CStreetMap::SLocation> SGeographicUtils::FilterLocations(const std::vector<CStreetMap::SLocation> &locations, const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright){
    std::vector<CStreetMap::SLocation> Filtered;
    for(auto &Location : locations){
        if((Location.DLatitude < lowerleft.DLatitude)||(Location.DLongitude < lowerleft.DLongitude)||(Location.DLatitude > upperright.DLatitude)||(Location.DLongitude > upperright.DLongitude)){
            continue;
        }
        Filtered.push_back(Location);
    }
    return Filtered;
}  // LCOV_EXCL_LINE



std::string SGeographicUtils::ConvertLLToDMS(CStreetMap::SLocation loc){
    double Lat = loc.DLatitude;
    double Lon = loc.DLongitude;
    double LatAbs = fabs(Lat);
    double LonAbs = fabs(Lon);
    std::stringstream OutStream;
    
    OutStream<<(int)LatAbs<<"d ";
    double Remainder = LatAbs - (int)LatAbs;
    Remainder *= 60.0;
    OutStream<<(int)Remainder<<"' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream<<std::setprecision(4)<<std::noshowpoint<<Remainder<<(Lat < 0.0 ? "\" S, " : "\" N, ");
    
    OutStream<<(int)LonAbs<<"d ";
    Remainder = LonAbs - (int)LonAbs;
    Remainder *= 60.0;
    OutStream<<(int)Remainder<<"' ";
    Remainder = Remainder - (int)Remainder;
    Remainder *= 60.0;
    Remainder = Remainder < 0.0005 ? 0.0 : Remainder;
    OutStream<<Remainder<<(Lon < 0.0 ? "\" W" : "\" E");
    
    return OutStream.str();
}
