# An Octave/MATLAB and C++ implementation to calculate the inverse of displacement vector fields.

On a domain $\Omega\subseteq\mathbb{R}^2$, we define two images as $R,T:\Omega \to [0,1]$, where each output value can be understood as an pixel-intensity intensity. Suppose the two images show the same information, but one is deformed with respect to another. Thus, we can assume there is a transformation $\varphi\in C^1(\Omega;\mathbb{R}^2)$ that accounts the deformation between the two images, in the sense that $T\circ\varphi = R$. The goal in most conventional image registration problems is to find this transformation given the two images $R,T$ as input.  

The transformation $\varphi$ is typically decomposed in two components: the identity operator $\textnormal{Id}:x\mapsto x$ and the displacement vector field $u\in C^1(\Omega;\mathbb{R}^2)$, i.e. $\varphi = \textnormal{Id} + u$. In some applications, the inverse vector field $\psi = \varphi^{-1}$ is required[[1]](#1). This can be done using the simple yet effective fixed point algorithm taken from [[2]](#2). Let us denote the displacement vector field belonging to $\psi$ with $v$. Then:

$\textnormal{Id} = \varphi \circ \psi = (\textnormal{Id} + u) \circ (\textnormal{Id} + v) = \textnormal{Id} + v + u\circ(\textnormal{Id} + v).$

This motivated the following fixed-point algorithm to iteratively approximate the displacement field $v$ belonging to $\psi$:

$v^0 = 0$

$v^j = -u\circ(\textnormal{Id} + v^{j-1}), \quad j \geqslant 1$

Notice that after the first iteration, we have the approximation $v = -u$. This is a popular approximation for the inverse displacement field, and works well whenever the magnitude of the displacement vector field is small. However, the larger the displacements, the poorer this approximation becomes, and a more refined approximation is mandatory.

This repository provides two implementations of this fixed-point algorithm, one in GNU Octave and one in C++. The C++ implementation can be compiled into a MEX-function that in turn can be called from GNU Octave. The C++ implementation is, as expected, significantly faster. Both implementations use linear interpolation to sample the displacement vector fields. The algorithm terminates whenever the user-specified convergence criterion $\epsilon$ is satisfied, $||v^j - v^{j-1}||^2 < \epsilon ||v^0||^2$.

## Compilation

The C++ implementation of this model is wrapped in a MEX function, which can be compiled using the `mex` command from MATLAB's MEX API. Alternatively, The MEX function can be compiled in GNU Octave using the `mkoctfile` command with the `--mex` flag enabled. The latter option is implemented in the `compile_mex_function.m` file. Running this script from the Octave command window builds the MEX application `InvertVectorField2d.mex` in the C++ source folder.

## How to use

The MEX file should be called first to pass the dimensions of the vector field `[dimx, dimy]`, the maximum number of iterations `niter` and the convergence criterion `eps`:

```
InvertVectorField2d([dimx, dimy], niter, eps);
```

This creates a static C++ object which stores the model parameters and allocates auxiliary variables required for the algorithm. Then, the inverse displacement vector field $v$ is calculated when the forward displacement vector field $u$ is passed:

```
InvertVectorField2d(vector_field);
```

The inverted displacement vector field is stored in the C++ object. It can be retrieved in Octave using

```
[inverse_displacement] = InvertVectorField2d();
```

Finally, once the user is done, the C++ library should be closed by the following command:

```
InvertVectorField2d();
```

which deallocates the C++ object from memory.

## Results

This project contains two example displacement vector fields in the `example` folder. One fields is quite coarse, with negative Jacobian determinant in multiple areas, which indicates non-physical behavior in the context of medical image registration. The smooth counterpart has positive Jacobian determinant on the entire domain. Two example scripts are added to this repository to use the GNU Octave and the C++ implementation. Before the first use of the C++ implementation, the `compile_mex_functions.m` script should be ran the build the MEX function.

The figure below shows the resulting inverse displacement vector field, together with the Jacobian determinant.

![Input displacement vector field together with the inverse displacement vector field and their corresponding Jacobian determinants.](https://github.com/tjwdraper/InvertDisplacementField2d/blob/main/img/motionwithinverse.png?raw=true)

We also look at the mean and standard deviation of the displacement vector fields belonging to the compositions $\varphi\circ\psi$ and $\psi\circ\varphi$. Ideally, these vector fields should be zero everywhere. In our casus, these are $0.000\pm0.137$ and $-0.014\pm0.205$ respectively.

## Caveats

This implementation uses linear interpolation to sample the vector fields on warped grids. However, as shown in [[2]](#2), cubic spline interpolation can yield more accurate approximation at the cost of extra computing time.

The iterative algorithm only uses the right-inverse property ($\varphi\circ\psi = \textnormal{Id}$), but one might argue that the resulting approximation satisfies the left-inverse condition ($\psi\circ\varphi = \textnormal{Id}$). Although ideally, we would have "the" inverse satisfying both properties, there is as far as I know no guarantee that this model necessarily satisfies the left-inverse condition. In the example above, we see that the calculated inverse vector field is also a reasonable approximation as a left-inverse.

If the input vector fields has negative Jacobian determinant anywhere, then this provides difficulties in calculating the inverse, since the Jacobian determinant should always have the same sign via the Inverse Function Theorem. Also, even if the input vector field has positive Jacobian determinant everywhere in the domain, the inverse calculated from this algorithm does not necessarily has so. That is why it might be more accurate to say this algorithm approximates a pseudo-inverse of $\varphi$, which satisfies $\varphi\circ\psi=\textnormal{Id}$, but is not necessarily in the same class of functions $C^1(\Omega;\mathbb{R}^2)$.

## References
<a id="1">[1]</a>
Huttinga, N. R. F., van den Berg, C. A. T. et al. (2020)
Physics in Medicine and Biology, 65(1), 015004.
https://doi.org/10.1088/1361-6560/ab554a


<a id="2">[2]</a>
Chen, M., Lu, W. et al (2008)
Medical physics, 35(1), 81–88. 
https://doi.org/10.1118/1.2816107
