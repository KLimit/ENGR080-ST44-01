function outCoords = filterGPSspikes(inCoords, latOrlon)
%FILTERGPSSPIKES filters the gps spikes that the follower recieves over
%bluetooth.
%   inCoords: input leaderLat or leaderLon
%   latOrlon: string to tell what range to use
if latOrlon == 'lat'
    target = 33;
elseif latOrlon == 'lon'
    target = -117;
end

% currently going to do a loop -- probably dealing with stuff that's too
% big for matlab though. Find a better way

for i = 1:numel(inCoords);
    if (inCoords(i) > target+1) || (inCoords(i) < target - 1)
        if i == 0
            outCoords(i) = target;
        else 
            outCoords(i) = outCoords(i-1);
        end
    else
        outCoords(i) = inCoords(i);
    end
end
