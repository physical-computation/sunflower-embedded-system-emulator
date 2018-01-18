data = xlsread('QfactorCalculation1.xlsx'); % Data from "measurement-3-15.h"
gyro = data(:,2); % angular rate data for Q factor calculation

absgyro = abs(gyro); %take absolute value of gyro data to identify peaks
peakValue = findpeaks(absgyro);
qPeaks = zeros(length(peakValue)-1,1); % denominator term in Eqn.3

for i = 1 : (length(peakValue)-1)
    qPeaks(i) = (peakValue(i)^2-peakValue(i+1)^2); % squared value
end

qFactor = (max(peakValue))^2/mean(qPeaks); % take average of Q based on Eqn.3