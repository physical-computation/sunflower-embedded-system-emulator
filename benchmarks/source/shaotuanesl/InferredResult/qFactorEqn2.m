data = xlsread('QfactorCalculation1.xlsx'); % Data from "measurement-3-15.h"
acc = data(:,1); % acceleration data for Q factor calculation

absacc = abs(acc); %take absolute value of acceleration data to identify peaks
[peakValue, index] = findpeaks(absacc);
integral = zeros(length(peakValue),1);
doubleIntegral = zeros(length(peakValue),1);

for i = 1 : length(index)-1
    integral(i) = (trapz(absacc((index(i):index(i+1))))); % numerical integration
    doubleIntegral(i) = trapz(integral(i:(i+1)));
end

qPeaks = zeros(length(peakValue)-1,1); % denominator term in Eqn.2

for i = 1 : (length(index)-1)
    qPeaks(i) = abs(doubleIntegral(i)-doubleIntegral(i+1));
end

qFactor = max(doubleIntegral)/mean(qPeaks); % take average of Q based on Eqn.2


