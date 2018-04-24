%% Creating a line of best fit for follower time 100

%First, open the workspace called 'TimeCalibration.mat'. This
%contains the truncated followerTime (and also the leaderTime).

Ts = 0.120; %Sampling period

numSamples = size(followerTime,1);
samplesVector = linspace(1, numSamples, numSamples);

nSamplesTruncated = size(followerTimeBegin,1);
samplesTruncated = linspace(1, nSamplesTruncated, nSamplesTruncated)';

linFitTruncated = polyfit(samplesTruncated, double(followerTimeBegin(1:end,2)),1); %first order poly fit for the second column of followerTimeBegin


%% Adjusting the y-intercept of our follower linear fit
clf;
plot(samplesTruncated, linFitTruncated(1).*samplesTruncated+ linFitTruncated(2));
hold on
plot(samplesVector, followerTime);

firstY = 5.924e7; %This value is the first followerTime recorded.
firstSampleRecording = 442; %Sample # where our robot starts recording
linFitTruncated(2) = linFitTruncated(2) - linFitTruncated(1) * firstSampleRecording;

plot(samplesVector, samplesVector.*linFitTruncated(1) + linFitTruncated(2));

%% Creating a linear fit for our leader times

numLeaderSamples = size(leaderTimeFiltered,1);
sLeaderVector = linspace(1, numLeaderSamples, numLeaderSamples);
nLeaderSamplesTruncated = size(leaderTimeBegin,1);
samplesLeaderTruncated = linspace(1, nLeaderSamplesTruncated, nLeaderSamplesTruncated)';

linFitLeaderTruncated = polyfit(samplesLeaderTruncated, double(leaderTimeBegin(1:end,2)),1); %first order poly fit for the second column of followerTimeBegin


%% Adjusting the y-intercept of our leader linear fit
clf;
plot(samplesLeaderTruncated, linFitLeaderTruncated(1).*samplesLeaderTruncated+ linFitLeaderTruncated(2));
hold on
plot(sLeaderVector, leaderTimeFiltered(1:end, 2));
% 
firstLeaderY = 5.078e6; %This value is the first followerTime recorded.
linFitLeaderTruncated(2) = firstLeaderY;

plot(sLeaderVector, (sLeaderVector-442).*linFitLeaderTruncated(1) + linFitLeaderTruncated(2));


%% Plotting relevant info
clf;
plot(sLeaderVector, (sLeaderVector-442).*linFitLeaderTruncated(1) + linFitLeaderTruncated(2));
hold on
plot(samplesVector, samplesVector.*linFitTruncated(1) + linFitTruncated(2));


%% Plotting leader against follower time
clf;
cutoff = min(size(sLeaderVector,2), size(samplesVector,2));
samplesVector = samplesVector(1:cutoff);
sLeaderVector = sLeaderVector(1:cutoff);

LeaderCurve = (sLeaderVector-442).*linFitLeaderTruncated(1) + linFitLeaderTruncated(2);
FollowerCurve = samplesVector.*linFitTruncated(1) + linFitTruncated(2);
plot(LeaderCurve, FollowerCurve);



