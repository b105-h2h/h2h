function bpm_log = bpm_decision_module( signal )
    Fs = 1000;  
    T = 1/Fs;          % Sampling period
    L = max(size(signal));
    t = (0:(L-1))*T;   % Time vector
    
    threshold_high = 1;
    threshold_low = 1;
    threshold_high_log = zeros(1,L);
    threshold_low_log = zeros(1,L);
    bpm_detected = zeros(1,L);
    maxerino = 0;
    sample_counter = 0;
    bpm_log_counter = 1;
    flag_qrs_zone = 0;
    flag_risk = 0 ;
    for i = 1:L
        ecg_lead = signal(i);
        if (ecg_lead > threshold_high)&&(flag_qrs_zone == 0)
            flag_qrs_zone = 1;
        elseif flag_qrs_zone == 1
            if (ecg_lead <= 0)
                bpm =              (60 * Fs) / sample_counter;
                threshold_high =        (threshold_high + maxerino*3) /4;
                threshold_low =        (threshold_low + maxerino*3) *6/40;
                maxerino = 0;
                flag_qrs_zone = 0;
%                 if( sample_counter > 200)
                    bpm_log(bpm_log_counter) = bpm;
                    bpm_log_counter = bpm_log_counter + 1;
                    sample_counter =   0;
                    bpm_detected(i)= max(signal); 
%                 end
            end
        elseif threshold_high > threshold_low
            threshold_high = threshold_high *255/256;
        end
        if ecg_lead > maxerino
            maxerino = ecg_lead;
        end
        sample_counter = sample_counter + 1;
        if (sample_counter > 2*Fs)&&(flag_risk ==0)
            maxerino = 0;
            sample_counter = 0;
            threshold_high = 1;
            threshold_low = 1;
        elseif (flag_risk ==1)
            disp('CUIDAAAAAAAAAAAAO')
        end
        threshold_high_log(i) = threshold_high;
        threshold_low_log(i) = threshold_low;
    end

%     norm = max(threshold_high_log);
%     plot(t, threshold_high_log./norm,'red');
%     plot(t, threshold_low_log./norm,'blue');

    plot(t,signal,'green');
    hold on
    plot(t, bpm_detected,'black');
    plot(t, threshold_high_log,'red');
    plot(t, threshold_low_log,'blue');
%     plot(t, bpm_detected,'green');
    hold off
end

