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

%**************************************************************************
% This Function Simlulates Sonar Sensors on the Mech Detecting Arena and 
% Building Walls
%**************************************************************************
function [CollisioncX,CollisioncY,CollisioncNorm ] =  fSonar(N,Xc,Yc,Xa,Ya,Psi,R)
    for j = 1:N
        Collisionc(j) =  fCollision(Xc,Yc, Xa(j,:),Ya(j,:),Psi,R);    
    end
    [XX,jmin]      = min([Collisionc.Norm]);
    CollisioncX    = [Collisionc(jmin).X];
    CollisioncY    = [Collisionc(jmin).Y];
    CollisioncNorm = [Collisionc(jmin).Norm];
return