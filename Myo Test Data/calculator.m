
function c = calculator(measurement)

EMG0 = sqrt(measurement.EMG0 .^ 2);
EMG1 = sqrt(measurement.EMG1 .^ 2);
EMG2 = sqrt(measurement.EMG2 .^ 2);
EMG3 = sqrt(measurement.EMG3 .^ 2);
EMG4 = sqrt(measurement.EMG4 .^ 2);
EMG5 = sqrt(measurement.EMG5 .^ 2);
EMG6 = sqrt(measurement.EMG6 .^ 2);
EMG7 = sqrt(measurement.EMG7 .^ 2);

EMG = EMG0 + EMG1 + EMG2 + EMG3 + EMG4 + EMG5 + EMG6 + EMG7;

%EMG = movmedian(EMG, 50);
EMG = movmean(EMG, 60 );
EMG = movmean(EMG, 60 );

tbl = table(measurement.gyro_z,measurement.EMG0,measurement.EMG1,measurement.EMG2, EMG);
tbl.Properties.VariableNames = {'gyro_z', 'EMG0', 'EMG1','EMG2','tension'};
head(tbl,5);

stackedplot(tbl);
c = EMG;

