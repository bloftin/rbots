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
% The Routine Constructs the Mechs Path through the Arena
%**************************************************************************

%**************************************************************************
% Path Sector Lengths
%**************************************************************************
N0 = 30;
N1 = 15;
N2 = 15;
N3 = 15;
N4 = 15;
N5 = 20;
N6 = 5;
N7 = 20;
N8 = 45;
N9 = 5;
N10 = 23;
N11 = 15;
N12 = 45;
N13 = 15;
N14 = 45;
N15 = 15;
N16 = 45;
N17 = 15;
N18 = 45;
N = N0+N1+N2+N3+N4+N5+N6+N7+N8+N9+N10+N11+N12+N13+N14+N15+N16+N17+N18;
delT = 1;
t = [0:delT:N-1];
%**************************************************************************
% X Velocity (Body)
%**************************************************************************
vx0 = 0*ones(1,N0);
vx1 = 3.25*ones(1,N1);
vx2 = 3.25*ones(1,N2);
vx3 = 7.5*ones(1,N3);
vx4 = 0*ones(1,N4);
vx5 = 0*ones(1,N5);
vx6 = 0*ones(1,N6);
vx7 = 0*ones(1,N7);
vx8 = 3.25*ones(1,N8);
vx9 = 0*ones(1,N9);
vx10 = -3.25*ones(1,N10);
vx11 = 0*ones(1,N11);
vx12 = 3.25*ones(1,N12);
vx13 = 0*ones(1,N13);
vx14 = 3.25*ones(1,N14);
vx15 = 0*ones(1,N15);
vx16 = 3.25*ones(1,N16);
vx17 = 0*ones(1,N17);
vx18 = 3.25*ones(1,N18);
vx = [vx0 vx1 vx2 vx3 vx4 vx5 vx6 vx7 vx8 vx9 vx10 vx11 vx12 vx13 vx14 vx15 vx16 vx17 vx18];
%**************************************************************************
% Y Velocity (Body)
%**************************************************************************
vy0 = 0*ones(1,N0);
vy1 = 0*ones(1,N1);
vy2 = 0*ones(1,N2);
vy3 = 0*ones(1,N3);
vy4 = 0*ones(1,N4);
vy5 = 3.25*ones(1,N5);
vy6 = 0*ones(1,N6);
vy7 = 3.25*ones(1,N7);
vy8 = 0*ones(1,N8);
vy9 = 0*ones(1,N9);
vy10 = 0*ones(1,N10);
vy11 = 0*ones(1,N11);
vy12 = 0*ones(1,N12);
vy13 = 0*ones(1,N13);
vy14 = 0*ones(1,N14);
vy15 = 0*ones(1,N15);
vy16 = 0*ones(1,N16);
vy17 = 0*ones(1,N17);
vy18 = 0*ones(1,N18);
vy = [vy0 vy1 vy2 vy3 vy4 vy5 vy6 vy7 vy8 vy9 vy10 vy11 vy12 vy13 vy14 vy15 vy16 vy17 vy18];
%**************************************************************************
% Angular (Yaw) Rate (Body)
%**************************************************************************
psidot0 = ones(1,N0)*2*pi/(N0-1);
psidot1 = ones(1,N1)*0;
psidot2 = ones(1,N2)*pi/2/N2;
psidot3 = ones(1,N3)*0;
psidot4 = ones(1,N4)*pi/N4;
psidot5 = ones(1,N5)*0;
psidot6 = -ones(1,N6)*pi/2/N6;
psidot7 = ones(1,N7)*0;
psidot8 = .05*cos([0:N8-1]/N8*2*pi);
psidot9 = -ones(1,N9)*pi/2/N9;
psidot10 = ones(1,N10)*0;
psidot11 = -ones(1,N11)*pi/2/N11;
psidot12 = 0*ones(1,N12)*pi/2/N12;
psidot13 = ones(1,N13)*pi/2/N13;
psidot14 = ones(1,N14)*0;
psidot15 = ones(1,N15)*pi/2/N15;
psidot16 = 0*ones(1,N16)*pi/2/N16;
psidot17 = ones(1,N17)*pi/2/N17;
psidot18 = 0*ones(1,N18)*pi/2/N18;
psidot = [psidot0 psidot1 psidot2 psidot3 psidot4 psidot5 psidot6 psidot7...
    psidot8 psidot9 psidot10 psidot11 psidot12 psidot13 psidot14 psidot15...
    psidot16 psidot17 psidot18];