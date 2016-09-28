
load('ECG.mat');

% short x[], y[];
% long acc, A, prev_x, prev_y;
% double pole;
% unsigned long n, num_samples;
% pole = 0.9999;
% A = (long)(32768.0*(1.0 - pole));
% acc = 0;
% prev_x = 0;
% prev_y = 0;
% for (n=0; n<num_samples; n++)
%     {
%     acc   -= prev_x;
%     prev_x = (long)x[n]<<15;
%     acc   += prev_x;
%     acc   -= A*prev_y;
%     prev_y = acc>>15;               // quantization happens here
%     y[n]   = (short)prev_y;
%     // acc has y[n] in upper 17 bits and -e[n] in lower 15 bits
% }

% L = 1000;
% x = zeros(1,L);
% x(1) = 1;
x = EKG4;
L = size(x,1);
y = zeros(1,L);

pole = 0.999;
A = (1024.0*(1.0 - pole));
acc = 0;
prev_x = 0;
prev_y = 0;

for i  = 1:L
    acc = acc - prev_x;
    prev_x = x(i) * 1024;
    acc = acc + prev_x;
    acc = acc - A*prev_y;
    prev_y = acc/1024;
    y(i) = prev_y;
end

hold on

% Xw = fft(y);
% plot(abs(Xw),'green')
% plot(unwrap(angle(Xw)),'blue')

plot(detrend(x),'green')
plot(y,'blue')

hold off