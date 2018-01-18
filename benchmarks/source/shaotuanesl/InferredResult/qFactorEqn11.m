data = xlsread('QfactorCalculation1.xlsx'); % Data from "measurement-3-15.h"
acc = data(:,1); % acceleration data for Q factor calculation

absacc = abs(acc); %take absolute value of acceleration data to identify peaks
[peakValue, index] = findpeaks(absacc);

qPeaks = zeros(length(peakValue)-1,1); % denominator term in Eqn.11

for i = 1 : (length(peakValue)-1)
    qPeaks(i) = abs((peakValue(i)-peakValue(i+1)));
end

qFactor = max(absacc)/mean(qPeaks); % take average of Q based on Eqn.11
