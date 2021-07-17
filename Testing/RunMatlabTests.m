%
% This file will run all the matlab tests
% Most of the code was copied over from the main delay file
% This file can be altered to read in audio files

clear;
close all;



% read in the test file. this can be changed to a different file
% name for further testing
% [x, Fs] = audioread("TestFile.wav");
% if length(x) ~= 2
%    x = [x, zeros(size(x))]; % turn to stereo
% end

Fs = 48000;
L = 1;
N = L * Fs;

% testing delta on left channel
x = zeros(N, 2);
x(1, 1) = 1;

% PingPongTest(x, fbL, fbR, wetL, wetR, D, dLevelL, dLevelL, distortion)
y1 = PingPongTest(x, 0.9, 0.5, 0.5, 0.5, 5, 1.5, 1.5, "hard_limit");
% audiowrite("MatlabTest1.wav", y, Fs);

% testing delta on right channel
x = zeros(N, 2);
x(1, 2) = 1;

% PingPongTest(x, fbL, fbR, wetL, wetR, D, dLevelL, dLevelL, distortion)
y2 = PingPongTest(x, 0.9, 0.5, 0.5, 0.5, 5, 1.5, 1.5, "hard_limit");
% audiowrite("MatlabTest1.wav", y, Fs);

[x, Fs] = audioread("TestAudio.wav");
y3 = PingPongTest(x, 0.99, 0.99, 0.5, 0.5, 10000, 15, 15, "hard_limit");
audiowrite("MatlabTest1.wav", y3, Fs);


