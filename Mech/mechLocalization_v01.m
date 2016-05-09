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
clear                               % Clear All Variables
clc                                 % Clear Command Window
%**************************************************************************
% Sim Options
%**************************************************************************
optnEST = 2;                        % Estimator Select
                                    %  1 = Dead Reckoning
                                    %  2 = Particle Filter
%**************************************************************************                                    
optnSaveVideo = 0;                  % Save Video Select
                                    %  0 = No Save 
                                    %  1 = Save Video to AVI file
%**************************************************************************
optnNoiseOff  = 0;                  % Sensor Noise Options
                                    %  0 = Use Sensor Noise Sources
                                    %  1 = No Sensor Noise Sources 
%**************************************************************************
optnBiasOff   = 0;                  % Sensor Bias Options
                                    %  0 = Use Sesnor Bias Sources
                                    %  1 = No Sensor Bias Sources
%**************************************************************************
optnStart     = 1;                  % PF Start Location
                                    %  1 = Known Corner
                                    %  2 = Anywhere in Arena
%**************************************************************************
optnShortRun  = 0;                  % Simulation Run Length
                                    %  0 = Full Path Length
                                    %  1 = Shortened Run
%**************************************************************************                                    
optnParticleN = 100;                % Number of Particles      
%**************************************************************************
% Constants
%**************************************************************************
OneThird = 1/3;
TwoPi = 2*pi;
%**************************************************************************
% Construct Arena and Buildings
%**************************************************************************
initArena
%**************************************************************************
% Initialize Path
%**************************************************************************
initPath

if(optnShortRun)
    Nsim = 15;
else
    Nsim = N;
end
%**************************************************************************
% Initialize
%**************************************************************************
% Initial Conditions
x = 18;                             % x-location (from Lower left corner)  (in)
y = 18;                             % y-location (from Lower left corner)  (in)
psi = deg2rad(0);                   % Angle (rad)
R = 10;                             % Mech Barrel Length (in)
                                    % Rotate Mech Barrel (in)
[Xr,Yr,Zr] =  SingleRotation(-psi,[0 R],[0 0],[0 0]);
Xr = Xr+x;                          % Place Mech Barrel at x-location (in)
Yr = Yr+y;                          % Place Mech Barrel at y-location (in)

% Turret
rTurret = 9;                        % Mech Body Radius (in)
[xBody,yBody,zBody]=cylinder(5,25); % Mech Body Circle

% Sonar Parameters
                                    % Sensor Ideal Location (rad)
sonrPsiIdeal = deg2rad([45 135 225 315]);  
sonrN= length(sonrPsiIdeal);        % Number of Sonar Sensors
                                    % Sensor Real Location (rad)
sonrRange = 30*12;                  % Sonar Sensor Range (in)
sonrNormMax = arenaW;               % Sonar max Range (in)
%**************************************************************************
%  Set Noise Sources
%**************************************************************************
if(optnNoiseOff)
    vxNoise     = 0;                % x-Rate Sensor Noise (in/s)        [Integrated Accel] 
    vyNoise     = 0;                % y-Rate Sensor Noise (in/s)        [Integrated Accel] 
    psiNoise    = 0;                % Angular Sensor Noise (rad)        [Magnetometer]
    psidotNoise = 0;                % Angular Rate Sensor Noise (rad/s) [Gyro]           
    xNoisei     = 0;                % Initial y Positon Randomness Std. Dev (in) 
    yNoisei     = 0;                % Initial x Positon Randomness Std. Dev (in)     
    psiNoisei   = deg2rad(5);       % Initial Orienation Randomness Std. Dev (rad)
    sonrPsiRe   = sonrPsiIdeal;     % Sonar Alignment Randomness (in)
    
else
    vxNoise = 1*randn(size(vx))/3;  % X-Rate Sensor Noise (in/s)        [Integrated Accel] 
    vyNoise = 1*randn(size(vy))/3;  % Y-Rate Sensor Noise (in/s)        [Integrated Accel]
                                    % Angular Sensor Noise (rad)        [Magnetometer]
    psiNoise = deg2rad(5)*randn(size(psi))/3;
                                    % Angular Rate Sensor Noise (rad/s) [Gyro]   
    psidotNoise = deg2rad(5)*randn(size(psidot))/3;
    xNoisei = 20;                   % Initial x Positon Randomness Std. Dev (in) 
    yNoisei = 20;                   % Initial x Positon Randomness Std. Dev (in)     
    psiNoisei = deg2rad(5);         % Initial Orienation Randomness Std. Dev (rad)
                                    % Sonar Alignment Randomness (in)
    sonrPsiRe = sonrPsiIdeal+deg2rad(3)*randn(1,sonrN)/3;
end

xNoiseEst = 3;                      % Estimator x Position Randomness Std. Dev (in) 
yNoiseEst = 3;                      % Estimator y Position Randomness Std. Dev (in) 
psiNoiseEst = deg2rad(5);           % Estimator Orientation Randomness Std. Dev (in) 
%**************************************************************************
%  Set Bias Sources
%**************************************************************************
if(optnBiasOff)
    vxBias      = 0;                % X-Rate Sensor Bias (in/s)
    vyBias      = 0;                % Y-Rate Sensor Bias (in/s)
    psiBias     = 0;                % Angular Sensor Bias (in/s)
    psidotBias  = 0;                % Angular Rate Sensor Bias (in/s)
else
    vxBias  = .1*randn(1)/3;        % X-Rate Sensor Bias (in/s)
    vyBias  = .1*randn(1)/3;        % Y-Rate Sensor Bias (in/s)
                                    % Angular Rate Sensor Bias (in/s)
    psiBias = deg2rad(2)*randn(1)/3;
                                    % Angular Rate Sensor Bias (rad/s)
    psidotBias   = deg2rad(.5)*randn(1)/3;      
end
%**************************************************************************
% Simulate Truth Pose
%**************************************************************************
for jsim = 2:Nsim
    % Run EOM using noise free path
    [x(jsim),y(jsim),psi(jsim)] =  fEOM(x(jsim-1),y(jsim-1),vx(jsim),vy(jsim),...
        psi(jsim-1),psidot(jsim),delT);
    
    % Update Turret Location and Orientation
    [XTurret,YTurret,ZTurret] =  SingleRotation(-psi(jsim),[0; R],[0; 0],[0; 0]);
    xTurret(:,jsim) = XTurret'+x(jsim);
    yTurret(:,jsim) = YTurret'+y(jsim);
end
%**************************************************************************
% Add Measurement Noise and Biases to Truth Pose
%**************************************************************************
vxMeas     = vx + vxNoise + vxBias;
vyMeas     = vy + vyNoise + vyBias;
psiMeas    = psi + psiNoise + psiBias;
psidotMeas = psidot + psidotNoise + psidotBias;
%**************************************************************************
% Estimate Pose - Dead Reckoning
% (Works fairly well since model and estimator are exact)
%**************************************************************************
if(optnEST == 1)
    % Set loop variables to single run only
    m = 1;
    optnParticleN = 1;
    
    % Set Estimator Initial Conditions
    % (I've set truth location to exact position and initalize estimator
    % to random starting location to simulate uncertaintity in starting 
    % positon - easier to simulate as it matches approach taken by aprticle 
    % filter
    psi_est(m,1) = psiMeas(1);
    x_est(m,1)   = x(1) + xNoisei*randn(1)*OneThird;
    y_est(m,1)   = y(1) + yNoisei*randn(1)*OneThird;

    % Set Turret Intial Conditions
    [XTurret_est,YTurret_est,ZTurret_est] =  SingleRotation(-psi_est(1,1),...
        [0; R],[0; 0],[0; 0]);
    xTurret_est(m,1) = XTurret_est(2)'+ x_est(m,1);
    yTurret_est(m,1) = YTurret_est(2)'+ y_est(m,1);

    % Run Simlation through all path timesteps
    for jsim = 2:Nsim
        % Update Estimate EOM
        [x_est(m,jsim),y_est(m,jsim),psi_est(m,jsim)] =  fEOM(x_est(m,jsim-1),...
            y_est(m,jsim-1),vxMeas(jsim),vyMeas(jsim),psiMeas(jsim),0,delT);

        % Update Turret Location and Orientation (for Plotting)
        [XTurret_est,YTurret_est,ZTurret_est] =  SingleRotation(-psi_est(m,jsim),...
            [0; R],[0; 0],[0; 0]);
        xTurret_est(m,jsim) = XTurret_est(2)'+ x_est(m,jsim);
        yTurret_est(m,jsim) = YTurret_est(2)'+ y_est(m,jsim);
        
        % Compute sonar readings off buildings/Arena
        for jsonar = 1:sonrN
            [CollissionX(jsonar,jsim),CollissionY(jsonar,jsim),...
                CollissionNorm(jsonar,jsim)] =  ...
                fSonar(arenN,x(jsim),y(jsim),arenX,arenY,...
                psi(jsim)+sonrPsiRe(jsonar),sonrRange);
        end      
    end
%**************************************************************************
% Estimate Pose - Particle Filter    
%**************************************************************************
elseif(optnEST == 2)
    fprintf(1,'\n')  
    tic;
    for jsim = 1:Nsim % Path Loop
        fprintf(1,'Sim Step %i, Time %f\n',jsim,toc)
        for m = 1:optnParticleN         % Particle Filter Loop
            if(jsim == 1)               % Initialize if first time thorugh loop
                % Initialize Particle Locations 
                if(optnStart == 1)      % Known Start Location
                    psi_estm(m,jsim) = psi(1) + psiNoisei*randn(1)*OneThird;
                    x_estm(m,jsim)   = x(1)   + xNoisei*randn(1)*OneThird;
                    y_estm(m,jsim)   = y(1)   + yNoisei*randn(1)*OneThird;  
                elseif(optnStart == 2)  % Anywhere in Arena    
                    psi_estm(m,jsim) = TwoPi*rand(1)*OneThird;
                    x_estm(m,jsim)   = arenaW*rand(1);
                    y_estm(m,jsim)   = arenaH*rand(1);   
                end
            else   
                % Update Particle Estimate Location
                [x_estm(m,jsim),y_estm(m,jsim),psi_estm(m,jsim)] =  fEOM(x_estm(m,jsim-1),...
                    y_estm(m,jsim-1),vxMeas(jsim),vyMeas(jsim),psiMeas(jsim),0,delT); 
            end
            % Add Radomness to Particle Estimate (Could be almost anything)
            x_estm(m,jsim)   = x_estm(m,jsim)   + xNoiseEst  *randn(1)*OneThird;
            y_estm(m,jsim)   = y_estm(m,jsim)   + yNoiseEst  *randn(1)*OneThird;
            psi_estm(m,jsim) = psi_estm(m,jsim) + psiNoiseEst*randn(1)*OneThird;

            % Compute Simulation Measurements (Real Location wrt Real Arena)
            % Actual Observations
            for jsonar = 1:sonrN
                [CollissionX(jsonar,jsim),CollissionY(jsonar,jsim),...
                    CollissionNorm(jsonar,jsim)] =  ...
                    fSonar(arenN,x(jsim),y(jsim),arenX,arenY,...
                    psi(jsim)+sonrPsiRe(jsonar),sonrRange);
            end

            % Compute Innovation (Estimate location wrt Model Map)
            % Predicted Observations
            for jsonar = 1:sonrN
                [CollissionXi(jsonar,jsim),CollissionYi(jsonar,jsim),...
                    CollissionNormi(jsonar,jsim)] =  ...
                    fSonar(arenN,x_estm(m,jsim),y_estm(m,jsim),modlX,modlY,...
                    psi_estm(m,jsim)+sonrPsiIdeal(jsonar),sonrRange);
                
                    if(CollissionNormi(jsonar,jsim)  > sonrNormMax)
                        CollissionNormi(jsonar,jsim) = sonrNormMax;
                    end
            end 

            % Compute error between predicted and actual observations
            v(:,m) = abs(CollissionNormi(:,jsim)-CollissionNorm(:,jsim));
                        
            % Covariance Matrix
            L = eye(4)*1e-3;
            
            % Compute Particle Weight
            vi(m) = -v(:,m)'*L*v(:,m);
            
            % wi(m) = exp(vi)+.01;
            wi(m) = -1./vi(m)+.01;;
        end
            
        % Cummulative Histogram
        ci = cumsum(wi);
        ci = ci/ci(optnParticleN);
        
        % Resample
        for m = 1:optnParticleN
                [XX,jmin] = min(abs(ci-rand(1)));      
                x_estm(m,jsim)   = x_estm(jmin,jsim);
                y_estm(m,jsim)   = y_estm(jmin,jsim);
                psi_estm(m,jsim) = psi_estm(jmin,jsim);
        end
        
        % Compute mean of all partilce for final estimate    
        x_est(jsim)   = mean(x_estm(:,jsim));
        y_est(jsim)   = mean(y_estm(:,jsim));
        psi_est(jsim) = mean(psi_estm(:,jsim));
       
        % Update Turret Location and Orientation (for Plotting)
        [XTurret_est,YTurret_est,ZTurret_est] =  SingleRotation(-mean(psi_est(jsim)),[0; R],[0; 0],[0; 0]);
        xTurret_est(jsim) = XTurret_est(2)'+ mean(x_est(jsim));
        yTurret_est(jsim) = YTurret_est(2)'+ mean(y_est(jsim));
    end
end
%**************************************************************************
% Plot
%**************************************************************************
fprintf(1,'\nReady to plot\n')
plotPF
