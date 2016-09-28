fs = 250;
t= 0:1/fs:10;

x_n = zeros(1,size(t,2));
x_n(100) = 1*size(t,2);

L = size(x_n,2);

b0=2;
b1=1;
b2=0;
b3=-1;
b4=-2;

buffer_x = zeros(1,4);
y_n = zeros(1,L);
for i = 1:size(x_n,2)
    y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
        b2 * buffer_x(2) + b3 * buffer_x(3) + ...
            b4 * buffer_x(4))/8;
    buffer_x(4) = buffer_x(3);
    buffer_x(3) = buffer_x(2);
    buffer_x(2) = buffer_x(1);
    buffer_x(1) = x_n(i);
   
end

subplot(2,1,1)
plot(t,y_n)

Y_w = fft(y_n);
K = size(y_n,2);

subplot(2,1,2)

plot(t,abs(Y_w./K))