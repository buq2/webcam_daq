function out = MicrosToDatenum(in)
in64 = uint64(in);

%Convert to full days
micros_in_day64 = uint64(1000*1000*60*60*24);
days = in64/micros_in_day64;

if (days*micros_in_day64 > in64)
    %MATLAB rounds integer division upwards?
    days = days -1;
end


micros_rest = in64-days*micros_in_day64;

days = double(days);
days_fract = double(micros_rest)/(1000*1000)/60/60/24;

out = days + days_fract + datenum('1970','yyyy');