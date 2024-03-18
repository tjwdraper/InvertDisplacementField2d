clc;
clear all;
close all;

addpath([pwd, '/src/c++/']);

if isfile('src/c++/InvertVectorField2d.mex');
  delete('src/c++/InvertVectorField2d.mex');
end

printf('Compiling InvertVectorField2d.mex...');

mkoctfile --mex ...
  -o 'src/c++/InvertVectorField2d.mex' ...
  src/c++/Inverter.cpp ...
  src/c++/Motion.cpp ...
  src/c++/WrapperInverseVectorField2d.cpp

printf('succes!\n');
