data = xlsread('QfactorCalculation1.xlsx'); % Data from "measurement.h"
gyro = data(:,2);

absgyro = abs(gyro);
peakValue = findpeaks(absgyro);
qPeaks = zeros(length(peakValue)-1,1); % denominator term in Eqn.3

for i = 1 : (length(peakValue)-1)
    qPeaks(i) = (peakValue(i)^2-peakValue(i+1)^2);
end

qFactor = (max(peakValue))^2/mean(qPeaks);