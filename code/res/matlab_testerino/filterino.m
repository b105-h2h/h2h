function bp_filtered = filterino(x_n)

    L = size(x_n,1);
    b0=1024;
    b1=-1857;
    b2=1024;
    a1=-1881;
    a2=981;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    x_n = y_n;

    b0=1024;
    b1=-2036;
    b2=1024;
    a1=-2013;
    a2=1007;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    x_n = y_n;

    b0=1024;
    b1=-1463;
    b2=1024;
    a1=-1878;
    a2=928;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    x_n = y_n;

    b0=1024;
    b1=-2044;
    b2=1024;
    a1=-1958;
    a2=963;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    x_n = y_n;

    b0=1024;
    b1=-1888;
    b2=1024;
    a1=-1896;
    a2=1015;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    x_n = y_n;

    b0=1024;
    b1=-2034;
    b2=1024;
    a1=-2029;
    a2=1021;

    buffer_x = zeros(1,2);
    buffer_y = zeros(1,2);
    y_n = zeros(1,L);

    for i = 1:size(x_n,2)
        y_n(i) = (b0 * x_n(i) + b1 * buffer_x(1) + ...
            b2 * buffer_x(2) - a1 * buffer_y(1) - ...
                a2 * buffer_y(2))/1024;
        buffer_y(2) = buffer_y(1);
        buffer_y(1) = y_n(i);
        buffer_x(2) = buffer_x(1);
        buffer_x(1) = x_n(i);    
    end

    bp_filtered = y_n./64;
end



