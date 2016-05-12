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
% The Initializaton Routine Construct the Mech Warfare Arena and Buidlings
%**************************************************************************

%**************************************************************************
% Declare Structures
%**************************************************************************
Model  = struct();                  % Arena (Ideal) Model
Sensor = struct();                  % Sensor
%**************************************************************************
% Construct Arena and Buildings
%**************************************************************************
arenaW  = 15*12;                    % Arena Width     [x-dimension] (in)
arenaH  = 15*12;                    % Arena Height    [y-dimension] (in)
buildW1 = 3*12;                     % Building Width  [x-dimension] (in)
buildH1 = 3*12;                     % Building Height [y-dimension] (in)
arenN = 5;                          % Number of Arena Buildings
                                    % First Building is the Arena Itself
if(optnNoiseOff)
    xNoiseA = 0;
    yNoiseA = 0;
    psiNoiseA = 0;
else
    xNoiseA = 2;
    yNoiseA = 2;
    psiNoiseA = 2;
end

% Setup Arena Dimensions                                    
Arena.Building(1).W = arenaW;
Arena.Building(2).W = buildW1;
Arena.Building(3).W = buildW1;
Arena.Building(4).W = buildW1;
Arena.Building(5).W = buildW1;

Arena.Building(1).H = arenaH;
Arena.Building(2).H = buildH1;
Arena.Building(3).H = buildH1;
Arena.Building(4).H = buildH1;
Arena.Building(5).H = buildH1;

Arena.Building(1).X = arenaW/2;
Arena.Building(2).X = 3*arenaW/10;
Arena.Building(3).X = 7*arenaW/10;
Arena.Building(4).X = 3*arenaW/10;
Arena.Building(5).X = 7*arenaW/10;

Arena.Building(1).Y = arenaW/2;
Arena.Building(2).Y = 3*arenaW/10;
Arena.Building(3).Y = 3*arenaW/10;
Arena.Building(4).Y = 7*arenaW/10;
Arena.Building(5).Y = 7*arenaW/10;

% Construct Arena
Arena.Building(1).XY = fBuilding(Arena.Building(1).W,Arena.Building(1).H ...
    ,Arena.Building(1).X,Arena.Building(1).Y,0);
Model.Building(1).XY = fBuilding(Arena.Building(1).W,Arena.Building(1).H ...
    ,Arena.Building(1).X,Arena.Building(1).Y,0);

for jArena = 2:arenN 
    % Real Placement of Arena Buildings
    Arena.Building(jArena).XY = fBuilding(Arena.Building(jArena).W,Arena.Building(jArena).H ...
        ,Arena.Building(jArena).X+xNoiseA*randn(1)/3,Arena.Building(jArena).Y+yNoiseA*randn(1)/3,...
        psiNoiseA/360*2*pi*randn(1)/3);
    
    % Ideal Placement of Arena Buildings
    Model.Building(jArena).XY = fBuilding(Arena.Building(jArena).W,Arena.Building(jArena).H ...
        ,Arena.Building(jArena).X,Arena.Building(jArena).Y,0);
end

% Compass Heading
Arena.Compass.Angle = 90/360*2*pi;
[xCompass,yCompass,zCompass] =  SingleRotation(-Arena.Compass.Angle,[0 .05],[0 0],[0 0]);
Arena.Compass.X = xCompass+.05;
Arena.Compass.Y = yCompass+.25;

for j = 1:arenN
    arenX(j,:) = Arena.Building(:,j).XY(1,:);
    arenY(j,:) = Arena.Building(:,j).XY(2,:);
    
    modlX(j,:) = Model.Building(:,j).XY(1,:);
    modlY(j,:) = Model.Building(:,j).XY(2,:);
end 