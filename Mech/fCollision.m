% Mech Warfare Localization. 
% This program simlulates the localization (using a Particle Filter) of a 
% Mech (Robot) in a square arena with multiple buildings. This program
% runs in the Matlab(R) programming language.
% Copyright (C) 2016  D. Croft
% 
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.
%**************************************************************************
function [Collision] =  fCollision(x,y,xBox,yBox,psi,R)
%**************************************************************************
% This Function is Called by the Sonar Function
%**************************************************************************
Collision = struct();

[Xr,Yr,Zr] =  SingleRotation(-psi,[0 R],[0 0],[0 0]);
Xr = Xr+x;
Yr = Yr+y;

[XCollision,YCollision] = polyxpoly(Xr,Yr,xBox,yBox,'unique');

if(isempty(XCollision))
    xCollision = inf;
    yCollision = inf;
    nCollision = inf;
else    
    for j = 1:length(XCollision)
        normCollision(j)= norm([XCollision(j)'-x; YCollision(j)'-y]);
    end
    [XX,jmin]  = min(normCollision);
    xCollision = XCollision(jmin);
    yCollision = YCollision(jmin);
    nCollision = normCollision(jmin);
end

Collision.X    = xCollision;
Collision.Y    = yCollision;
Collision.Norm = nCollision;

return