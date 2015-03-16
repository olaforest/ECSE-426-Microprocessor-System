% 1D Kalman filtering of data using with initial Kalman state parameters
% p, r and q.
function [filtered] = KalmanFilter1D(data, p, ~, r, q)
    filtered = zeros(length(data), 1);

    % filtering
    x = data(1);
    for i = 1:length(filtered)
        p = p + q;
        k = p / (p + r);
        x = x + k * (data(i) - x);
        p = (1 - k) * p;
        filtered(i) = x;
    end
    
    % plotting result
    plot(1:length(filtered), data, 1:length(filtered), filtered, 'r');
    title(['Raw sensor data and Kalman filtered data (r = ', num2str(r),...
           ', q = ', num2str(q), ')']);
end

