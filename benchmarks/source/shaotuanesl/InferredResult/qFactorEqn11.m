data = xlsread('QfactorCalculation.xlsx');
acc = data(:,1);
absacc = abs(acc);
[peakValue, index] = findpeaks(absacc);

qPeaks = zeros(length(peakValue)-1,1); % denominator term in Eqn.11


for i = 1 : (length(peakValue)-1)
    qPeaks(i) = abs((peakValue(i)-peakValue(i+1)));
end

qFactor = max(absacc)/mean(qPeaks);