%% Load the vector field
close all;
clear all;
clc;

addpath(genpath(pwd));
load('example\motion_coarse');

%% Iterative model for inverse vector field 
% Get parameters and initiate C++ object
niter = 20;
convergence_criterion = 1e-5;
[dimx, dimy, ~] = size(motion);

InvertVectorField2d([dimx, dimy], niter, convergence_criterion);

% Pass motion field to C++ object and calculate inverse
InvertVectorField2d(motion);

% Get the inverse vector field from the C++ object
invmotion = InvertVectorField2d();

% Once finished, the C++ object is deleted and memory is freed with the 
% following command.
InvertVectorField2d();

%% Calculate some properties to show some results
[dudy, dudx] = gradient(squeeze(motion(:,:,1)));
[dvdy, dvdx] = gradient(squeeze(motion(:,:,2)));

divu = dudx + dvdy;

jacu = (1 + dudx) .* (1 + dvdy) - dudy .* dvdx;

[dudyinv, dudxinv] = gradient(squeeze(invmotion(:,:,1)));
[dvdyinv, dvdxinv] = gradient(squeeze(invmotion(:,:,2)));

divuinv = dudxinv + dvdyinv;

jacuinv = (1 + dudxinv) .* (1 + dvdyinv) - dudyinv .* dvdxinv;

%
figure(); 
subplot(2,4,1); quiver(motion(end:-1:1,:,1), motion(end:-1:1,:,2), 0); axis off; title('input motion');
subplot(2,4,2); imagesc(divu); colormap jet; axis off; colorbar; title('divergence');
subplot(2,4,3); imagesc(jacu); colormap jet; axis off; colorbar; title('jacobian');
subplot(2,4,4); imagesc(sqrt(dudx.^2 + dudy.^2 + dvdx.^2 + dvdy.^2)); colormap jet; axis off; colorbar; title('harmonic energy');
subplot(2,4,5); quiver(invmotion(end:-1:1, :, 1), invmotion(end:-1:1, :, 2), 0); axis off; title('inverted motion');
subplot(2,4,6); imagesc(divuinv); colormap jet; axis off; colorbar; title('divergence');
subplot(2,4,7); imagesc(jacuinv); colormap jet; axis off; colorbar; title('jacobian');
subplot(2,4,8); imagesc(sqrt(dudxinv.^2 + dudyinv.^2 + dvdxinv.^2 + dvdyinv.^2)); colormap jet; axis off; colorbar; title('harmonic energy');


% Warp a grid forward and then invert
id_displacement_fb = zeros(dimx, dimy, 2);
id_displacement_fb(:,:,1) = invmotion(:,:,1) + interp(motion(:,:,1), invmotion);
id_displacement_fb(:,:,2) = invmotion(:,:,2) + interp(motion(:,:,2), invmotion);
printf("Apply the inverse transformation on the motion field should yield the\n");
printf("identity transformation. The corresponding displacement field should be\n");
printf("identically zero. Here, it is mean +- std: %.3f +- %.3f\n", ...
    mean(id_displacement_fb(:)), ...
    std(id_displacement_fb(:)));

id_displacement_bf = zeros(dimx, dimy, 2);
id_displacement_bf(:,:,1) = motion(:,:,1) + interp(invmotion(:,:,1), motion);
id_displacement_bf(:,:,2) = motion(:,:,2) + interp(invmotion(:,:,2), motion);
printf("Apply the forward transformation on the inverse motion field should\n");
printf(" yield the identity transformation. The corresponding displacement field\n");
printf("should be identically zero. Here, it is mean +- std: %.3f +- %.3f\n", ...
    mean(id_displacement_bf(:)), ...
    std(id_displacement_bf(:)));
