function [ w ] = Wviscosity( r, h )
%WVISCOSITY Summary of this function goes here
%   Detailed explanation goes here
    radius = norm(r);

    if radius < h && radius >= 0
        w = (15/(2*pi*h^3)) * ( -radius^3/(2*h^3) + radius^2/(h^2) + h/(2*radius) - 1 );
    else
        w = 0;
        
    end
end