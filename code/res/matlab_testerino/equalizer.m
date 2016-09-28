clear
load('ECG.mat');

L = max(size(EKG2));
Fs = 1000;         % Sampling frequency
T = 1/Fs;          % Sampling period
t = (0:(L-1))*T;   % Time vector

f = Fs*(0:(L-1))/L;

x = zeros(1,L);
x(1)= 1;
b = [ 1 -1];
a = [ 1 -1018/1024];

order = 4;
F = 1/500:0.001:150/500;
g = grpdelay(b,a,F,2); % We will only equalize the passband.
Gd = max(g)-g;
[num,den]=iirgrpdelay(order, F, [1/500 150/500], g);

% fvtool(b,a);
% fvtool(num,den);

xhp = filter(b,a,x);
Xhp = fft(xhp);

xhps = filter(num,den,xhp);
Xhps = fft(xhps);

% figure
% hold on
% [hAx,hLine1,hLine2] = plotyy(f(1:L/2),10*log(abs(Xhps(1:L/2))),f(1:L/2),unwrap(angle(Xhps(1:L/2))));
% grid
% title('Magnitude Response(dB) and Phase Response')
% xlabel('Frenquency (Hz)')
% 
% ylabel(hAx(1),'Magnitude (dB)') % left y-axis
% ylabel(hAx(2),'Phase (radians)') % right y-axis
% hold off

prev_x = 0;
prev_y = 0;
dife = 0; 
inte = 0;
dc_bloqued = zeros(1,L);

for i = 1:L
   dife = x(i) - prev_x;
   prev_x = x(i);
   inte = (1018/1024)*prev_y + dife;
   prev_y = inte;  
   dc_bloqued(i) = inte;
end


num_sintonizer = [610, -2442, 4038, -3230, 1024];
dem_sintonizer = [1024, -3230, 4038, -2442, 610];

factor = 2^10;
% num_sintonizer =  round(num.*factor);
% dem_sintonizer =  round(den.*factor);
buffer_x = zeros(1,order +1);
buffer_y = zeros(1,order +1);
y_n = 0;
sintonized = zeros(1,L);

for i = 1:L

%    // Shift the register values.
%    for(k=N; k>0; k--)RegX[k] = RegX[k-1];
%    for(k=N; k>0; k--)RegY[k] = RegY[k-1];    
%    RegX[0] = Signal[j];
%    CenterTap = NumCoeff[0] * RegX[0];
%    for(k=1; k<=N; k++)
%     {
%      CenterTap += NumCoeff[k] * RegX[k] - DenomCoeff[k] * RegY[k];
%     }
%    RegY[0] = CenterTap * DenomCoeff[0];  // DenomCoeff[0] should = 1
%    FilteredSignal[j] = RegY[0];

    for j =order + 1:-1:2
        buffer_x(j)= buffer_x(j-1);
        buffer_y(j)= buffer_y(j-1);
    end
    buffer_x(1) = dc_bloqued(i);
    y_n = num_sintonizer(1) * buffer_x(1)/factor;
    for j = 2:order +1
         y_n = y_n + (num_sintonizer(j) * buffer_x(j))/factor - (dem_sintonizer(j) * buffer_y(j))/factor;
    end
    buffer_y(1) = y_n * dem_sintonizer(1)/factor; 
    sintonized(i) = buffer_y(1);
end

SINTONIZED = fft(sintonized);  
modulo_sin = 10*log(abs(SINTONIZED(1:L/2)));
fase_sin = unwrap(angle(SINTONIZED(1:L/2)));
half_frec = f(1:L/2);
retraso = diff(fase_sin);

figure
hold on
[hAx,hLine1,hLine2] = plotyy(half_frec,modulo_sin,half_frec,fase_sin);
grid
title('Magnitude Response(dB) and Phase Response')
xlabel('Frenquency (Hz)')

ylabel(hAx(1),'Magnitude (dB)') % left y-axis
ylabel(hAx(2),'Phase (radians)') % right y-axis
hold off

figure

plot(half_frec(1:30),modulo_sin(1:30))
grid
title('Magnitude Response(dB) and Phase Response')
ylabel('Magnitude (dB)')


low_pass_coef_151hz = [0, 1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, -1, -1, -1, 0,...
    2, 2, 0, -2, -2, -1, 2, 3, 1, -2, -3, -2, 1, 4, 4, 0, -4, -5, -1, 4, 6,...
     3, -3, -7, -5, 2, 8, 8, 0, -8, -10, -3, 8, 13, 7, -7, -16, -12, 4, 19,...
     18, 0, -21, -27, -8, 23, 40, 21, -25, -64, -54, 26, 152, 268, 315, 268,...
     152, 26, -54, -64, -25, 21, 40, 23, -8, -27, -21, 0, 18, 19, 4, -12,...
     -16, -7, 7, 13, 8, -3, -10, -8, 0, 8, 8, 2, -5, -7, -3, 3, 6, 4, -1,...
     -5, -4, 0, 4, 4, 1, -2, -3, -2, 1, 3, 2, -1, -2, -2, 0, 2, 2, 0, -1,...
     -1, -1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, 1, 0];

xhp = filter(low_pass_coef_151hz,1024,x);
Xhp = fft(xhp);

figure
hold on
[hAx,hLine1,hLine2] = plotyy(f(1:L/2),10*log(abs(Xhp(1:L/2))),f(1:L/2),unwrap(angle(Xhp(1:L/2))));
grid
title('Magnitude Response(dB) and Phase Response')
xlabel('Frenquency (Hz)')

ylabel(hAx(1),'Magnitude (dB)') % left y-axis
ylabel(hAx(2),'Phase (radians)') % right y-axis
hold off

all_pass_diff_coef = [20, -40, 56, -148, 1306, -1306, 148, -56, 40, -20];

xhp = filter(all_pass_diff_coef,1024,x);
Xhp = fft(xhp);

figure
hold on
[hAx,hLine1,hLine2] = plotyy(f(1:L/2),10*log(abs(Xhp(1:L/2))),f(1:L/2),unwrap(angle(Xhp(1:L/2))));
grid
title('Magnitude Response(dB) and Phase Response')
xlabel('Frenquency (Hz)')

ylabel(hAx(1),'Magnitude (dB)') % left y-axis
ylabel(hAx(2),'Phase (radians)') % right y-axis
hold off