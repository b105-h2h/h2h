function preprocessed = bpm_preprocessing( signal )

    L = max(size(signal));
%     lp_order_41hz = 146;
%     lp_folder_coef_41hz = lp_order_41hz /2;
% 
%     low_pass_coef_41hz = [1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0,...
%         1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0, -1, -2, -3, -4, -4, -5, -4, -4, -3, -2,...
%         -1, 1, 3, 5, 6, 8, 8, 9, 8, 7, 5, 2, -1, -4, -8, -11, -14, -17, -18, -18,...
%         -16, -13, -8, -1, 7, 16, 27, 37, 48, 59, 68, 76, 83, 87, 90, 90, 87, 83, ...
%         76, 68, 59, 48, 37, 27, 16, 7, -1, -8, -13, -16, -18, -18, -17, -14, -11,...
%         -8, -4, -1, 2, 5, 7, 8, 9, 8, 8, 6, 5, 3, 1, -1, -2, -3, -4, -4, -5, -4,...
%         -4, -3, -2, -1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, -1, -1, -1, -1,...
%         -1, -1, -1, -1, 0, 0, 0, 0, 0, 1];
%     lp_filtered = zeros(1,L);
% 
%     lp_buffer = zeros(1,lp_order_41hz);
% 
%     for i = 1:L
%         y_n = 0;
%         lp_buffer(1) = signal(i);
%         for j = 1:lp_folder_coef_41hz
%             y_n = y_n + ((low_pass_coef_41hz(j) * (lp_buffer(j) + lp_buffer(lp_order_41hz + 1 - j)))/1024); 
%         end
%         lp_filtered(i) = y_n;
% 
%         for j = lp_order_41hz:-1:2
%             lp_buffer(j) = lp_buffer(j-1);
%         end  
%     end
    
    % plot(f,20*log(abs(fft(lp_filtered))),'black');
    % norm = max(lp_filtered_151hz);
    % p2 = plot(t, lp_filtered_151hz./norm,'green');

    dif_order = 10;
    dif_folder_coef = dif_order /2;

    all_pass_diff_coef = [20, -40, 56, -148, 1306, -1306, 148, -56, 40, -20];

    dif_filtered = zeros(1,L);

    dif_buffer = zeros(1,dif_order);

    for i = 1:L
        y_n = 0;
%         dif_buffer(1) = lp_filtered(i);
        dif_buffer(1) = signal(i);
        for j = 1:dif_folder_coef
            y_n = y_n + ((all_pass_diff_coef(j) * (dif_buffer(j) - dif_buffer(dif_order + 1 - j)))/1024);
        end
        dif_filtered(i) = 4*y_n;

        for j = dif_order:-1:2
            dif_buffer(j) = dif_buffer(j-1);
        end  
    end
    preprocessed = dif_filtered;
    % plot(f,20*log(abs(fft(dif_filtered))),'green');
    % norm = max(dif_filtered);
    % p3 = plot(t, dif_filtered./norm,'blue');

%     squared = zeros(1,L);
%     max_value = 1;
%     for i = 1:L
%         if( dif_filtered(i) > 0)
%             if( dif_filtered(i) > max_value)
%                 max_value = dif_filtered(i);
%             else
%                 max_value = max_value*255/256;
%             end
% 
%             normalized = dif_filtered(i)/max_value;
%             squared(i) = normalized * normalized;
%         else
%             squared(i) = 0;
%         end
%     end
% 
%     % plot(f,20*log(abs(fft(squared))),'red');
%     % norm = max(absolute_value);
%     % p4 = plot(t, absolute_value./norm,'red');
% 
%     preprocessed = zeros(1,L);
%     integration_length = 16;
% 
%     int_buffer = zeros(1,integration_length);
% 
%     for i = 1:L
%         int_buffer(1) = squared(i);
%         y_n = 0;
%         for j =1:integration_length
%             y_n = y_n + int_buffer(j);
%         end
%         preprocessed(i) = y_n/integration_length;
%         for j =integration_length:-1:2
%             int_buffer(j) = int_buffer(j - 1);
%         end
%     end
% 
%     % plot(f,20*log(abs(fft(integrated))),'blue');
%     % norm = max(integrated);
%     % p5 = plot(t, integrated./norm,'yellow');

end

