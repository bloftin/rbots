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
h = figure(1);
scrsz = get(0,'ScreenSize');
% set(h,'Position',[1 scrsz(4)/2 scrsz(3)*.75 scrsz(4)/2])

if(optnSaveVideo)
    filename = ['PF',datestr(now, 'yymmddHHMMSS'),'.avi'];
    vidObj = VideoWriter(filename);
    open(vidObj);
end

for jsim = 1:Nsim;
    clf
    %**********************************************************************
    % Plot Truth
    %**********************************************************************
    subplot(1,2,1)
    hold on
    annotation('textarrow',Arena.Compass.X,Arena.Compass.Y,'String','N','FontSize',14)
    plot(Arena.Building(1).XY(1,:),Arena.Building(1).XY(2,:),'k','LineWidth',2)
        for jarena = 2:arenN 
            patch(Arena.Building(jarena).XY(1,:),Arena.Building(jarena).XY(2,:),[155 155 155]/255)
        end
        for jsonar = 1:sonrN
            plot(CollissionX(jsonar,1:jsim),CollissionY(jsonar,1:jsim),'b.','MarkerSize',8,'LineWidth',2)
            plot(CollissionX(jsonar,jsim),CollissionY(jsonar,jsim),'rx','MarkerSize',8,'LineWidth',2)
            plot([x(jsim) CollissionX(jsonar,jsim)],[y(jsim) CollissionY(jsonar,jsim)],':','color',[245 245 204]/255,'LineWidth',3)
        end
    plot(x(1:jsim),y(1:jsim),'.','color',[204 204 204]/255,'LineWidth',2)
    plot(x(jsim)+xBody(1,:),y(jsim)+yBody(1,:),'r')
    plot(xTurret(:,jsim),yTurret(:,jsim),'r','LineWidth',2)

    xlabel('x (in)')
    ylabel('y (in)')
    title('Truth')
    xlim([0-24 arenaW+24])
    ylim([0-24 arenaH+24])
    set(gca,'XTick',[0:18:arenaW])
    set(gca,'YTick',[0:18:arenaH])
    axis square
    grid on
    %**********************************************************************
    % Plot Estimates
    %**********************************************************************
    subplot(1,2,2)
    hold on
    plot(Model.Building(1).XY(1,:),Model.Building(1).XY(2,:),'k','LineWidth',2)
        for jarena = 2:arenN 
            patch(Model.Building(jarena).XY(1,:),Model.Building(jarena).XY(2,:),[155 155 155]/255)
        end
        plot(x_est(1:jsim),y_est(1:jsim),'k.','LineWidth',2)
        if(optnEST == 2)
        plot(x_estm(:,jsim),y_estm(:,jsim),'.','color',[155 155 155]/255,'LineWidth',2)
        end
        plot(x_est(jsim)+xBody(1,:),y_est(jsim)+yBody(1,:),'r')
        plot([x_est(jsim) xTurret_est(jsim)],[y_est(jsim) yTurret_est(jsim)],'r','LineWidth',2)
        for jsonar = 1:sonrN
            plot([x_est(1:jsim) + CollissionNorm(jsonar,1:jsim).*cos(sonrPsiIdeal(jsonar) + psi_est(1:jsim))],...
                 [y_est(1:jsim) + CollissionNorm(jsonar,1:jsim).*sin(sonrPsiIdeal(jsonar) + psi_est(1:jsim))],...
                 'b.')
            plot([x_est(jsim) x_est(jsim) + CollissionNorm(jsonar,jsim)*cos(sonrPsiIdeal(jsonar) + psi_est(jsim))],...
                 [y_est(jsim) y_est(jsim) + CollissionNorm(jsonar,jsim)*sin(sonrPsiIdeal(jsonar) + psi_est(jsim))],...
                 'y','LineWidth',1)
        end
        xlabel('x (in)')
        ylabel('y (in)')
        title('Estimate')
        xlim([0-24 arenaW+24])
        ylim([0-24 arenaH+24])
        set(gca,'XTick',[0:18:arenaW])
        set(gca,'YTick',[0:18:arenaH])
        axis square
        grid on
        if(optnSaveVideo)
            currFrame = getframe(h);
            writeVideo(vidObj,currFrame);
            writeVideo(vidObj,currFrame);
        end
    pause(.1);
end
%**************************************************************************
% Save to Video File
%**************************************************************************
if(optnSaveVideo)
    close(vidObj);
    clear vidObj;
end
