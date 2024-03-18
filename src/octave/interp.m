function fieldout = interp(fieldin, displacement)
    % Get field dimensions
    [dimx, dimy] = size(fieldin);
    
    % Initialize output field
    fieldout = zeros(dimx, dimy);
    
    % Iterate over all voxels
    for i = 1:dimx
        for j = 1:dimy
            % calculate the coordinate to sample from
            px = i + displacement(i,j,1); dx = floor(px); fx = px - dx;
            py = j + displacement(i,j,2); dy = floor(py); fy = py - dy;
            
            % check if within bounds
            if ((dx < 1) || (dx > dimx) || 
                (dy < 1) || (dy > dimy))
                fieldout(i,j) = fieldin(i,j);
                continue;
            endif
            
            % sampler from nearest points
            val = fieldin(dx, dy) * (1 - fx) * (1 - fy);
            weight = (1- fx) * (1 - fy);
            
            if (dx < dimx-1)
                val += fieldin(dx+1,dy) * fx * (1 - fy);
                weight += fx * (1 - fy);
            endif
            if (dy < dimy -1)
                val += fieldin(dx, dy+1) * (1 - fx) * fy;
                weight += (1 - fx) * fy;
            endif
            if ((dx < dimx-1) && (dy < dimy-1))
                val += fieldin(dx+1, dy+1) * fx * fy;
                weight += fx*fy;
            endif
            
            % set output field value
            if (weight ~= 0)
                fieldout(i,j) = val / weight;
            endif
        endfor
    endfor
    
    
endfunction