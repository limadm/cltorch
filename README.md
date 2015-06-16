# cltorch

An OpenCL backend for [torch](http://torch.ch/).

## What's working

### Import

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>

<tr><td><pre>require 'cltorch'</pre> <td> works <td><pre>require 'cltorch'</pre></tr>

</table>

### Device information and control

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>

<tr><td>Device information<td>works<td><pre>
print('num devices: ', cltorch.getDeviceCount())
props = cltorch.getDeviceProperties(1)
for k,v in props do
    print(k, v)
end
cltorch.setDevice(1)
print('current device: ', cltorch.getDevice())

</pre></tr>
</table>

### From [storage.md](https://github.com/torch/torch7/blob/master/doc/storage.md)

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>

<tr><td> torch.ClStorage <td> works <td><pre>
c = torch.ClStorage()
c = torch.ClStorage(3)
c[1] = 5
c = torch.ClStorage{4,9,2}
c:fill(7)
a = torch.Storage{1.5, 2.4, 5.3}
c:copy(a)
c[2] = 21
a:copy(c)
d = torch.ClStorage(3)
d:copy(c)
print('size:', #d)
print('size:', d:size())
c:resize(5)
</pre></tr>

</table>

### From [tensor.md](https://github.com/torch/torch7/blob/master/doc/tensor.md)

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>

<tr><td>torch.ClTensor constructors<td>works<td><pre>
c = torch.ClTensor()
c = torch.ClTensor{7,4,5}
c = torch.ClTensor(3,2)
</pre></tr>

<tr><td>Cloning <td>90%<td><pre>
c = torch.ClTensor{{3,1,6},{2.1,5.2,3.9}}
d = c:clone()
print('isTensor', torch.isTensor(c))
</pre></tr>

<tr><td>Quering size and structure<td>90%<td><pre>
c = torch.ClTensor{3,5,2}
print('torch.isTensor(c)', torch.isTensor(c))
print('c:nDimension()', c:nDimension())
C = torch.ClTensor{{3,2,4},{9,7,5}}
print('C:nDimension()', C:nDimension())
print('c:dim()', C:dim())
print('C:size()', C:size())
print('C:size(1)', C:size(1))
print('C:size(2)', C:size(2))
print('#C', #C)
print('C:stride()', C:stride())
print('C:stride(1)', C:stride(1))
print('C:stride(2)', C:stride(2))
print('C:storage()', C:storage())
print('C:nElement()', C:nElement())
print('C:storageOffset()', C:storageOffset())
</pre></tr>

<tr><td>Copying and initializing<td>Done<td><pre>
c = torch.Tensor{2,6,9}:cl()
b = c:float()
c = torch.ClTensor{{3,1,6},{2.1,5.2,3.9}}
b:copy(c)
c:copy(b)
d = torch.ClTensor(2,3)
d:copy(c)
c:fill(1.345)
c:zero()
</pre></tr>

<tr><td>Resizing<td>Done<td><pre>
c:resize(3,2)
l = torch.LongStorage{3,3}
c:resize(l)
d = torch.ClTensor(2,2)
d:resizeAs(c)
</pre></tr>

<tr><td>Extracting sub-tensors<td>50%<td><pre>
  E = torch.ClTensor{{3,1},{2,9},{3,2},{7,8},{6,4}}
  F = E:narrow(1,2,3)
  F = E:sub(2,3,2,2)
  E:select(1,2):fill(99)
  x = torch.Tensor(5, 6):zero()
  x[{ 1,3 }] = 1
  x[{ 2,{2,4} }] = 2 
  x[{ {},4 }] = -1
  x[{ {},2 }] = torch.range(1,5) 
  x[torch.lt(x,0)] = -2
</pre></tr>


<tr><td>Expanding/Replicating/Squeezing Tensors<td>0%<td><pre>
</pre></tr>

<tr><td>Manipulating the tensor view<td>5%<td><pre>
C = torch.ClTensor{{3,2,4},{9,7,5}}
print(C:t())
</pre></tr>

</table>

### From [random.md](https://github.com/torch/torch7/blob/master/doc/random.md)

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>

<tr><td>uniform, etc <td>0%<td><pre>For now, you would need to do for example:
torch.Tensor(5,3):uniform():cl()
</pre></tr>
</table>

### From [maths.md](https://github.com/torch/torch7/blob/master/doc/maths.md)

<table>

<tr><td>Component<td>Status<td>Examples of what works now</tr>


<tr><td>Construction or extraction functions<td>Started<td><pre>
c:fill(1.345)
c:zero()
print(torch.ClTensor.zeros(torch.ClTensor.new(), 3, 5))
print(torch.ClTensor.ones(torch.ClTensor.new(), 3, 5))
A = torch.ClTensor{{3,5,2},{4,5,6}}
print(A:reshape(3,2))
</tre></tr>

<tr><td>Element-wise operations<td>Done<td><pre>
c:abs()
for _,name in ipairs({'log','exp', 'cos', 'acos', 'sin', 'asin',
   'atan', 'tanh', 'ceil', 'floor', 'abs', 'round'}) do
  loadstring('c:' .. name .. '()')()
end
</pre>
</tr>

<tr><td>basic operations <td>50% done<td><pre>
d = torch.ClTensor{{3,5,-2},{2.1,2.2,3.9}}
c = torch.ClTensor{{4,2,-1},{3.1,1.2,4.9}}
c:add(d)
c:cmul(d)
c:cdiv(d)
c:add(3)
c:mul(3)
c:div(2)
c = torch.add(c,3)
c = torch.mul(c, 4)
c = torch.div(c, 3)
torch.pow(2,c)
c:pow(2)
torch.cpow(c,d)
torch.cdiv(c,d)
torch.pow(c,2)
torch.clamp(c, 50, 100)
c:clamp(50, 100)
-c

A = torch.ClTensor{{1,2,-1},
                   {3,4,0}}
B = torch.ClTensor{{0,1},
                   {1,2},
                   {4,5}}
print(torch.mm(A,B))
C:mm(A,B)

C = torch.ClTensor{{3,2},{9,7}}
D = torch.ClTensor{{3,1,7},{3,2,4}}
E = torch.ClTensor{{3,1},{2,9},{3,2}}
print(torch.addmm(C,D,E))

c = torch.ClTensor{3,2}
D = torch.ClTensor{{3,1,7},{3,2,4}}
e = torch.ClTensor{3,1,2}
print(torch.addmv(c,D,e))

v1 = torch.ClTensor{3,5,1}
v2 = torch.ClTensor{2,4,8}
print(torch.dot(v1, v2))

print(torch.mv(A,v1))

C = torch.ClTensor{{3,1,7},{3,2,4},{8,5,3}}
d = torch.ClTensor{3,2,5}
e = torch.ClTensor{3,1,2}
print(torch.addr(C,d,e))
</pre></tr>

<tr><td>Logical operations <td>Done<td><pre>
d = torch.ClTensor{{3,5,-2},{2.1,2.2,3.9}}
c = torch.ClTensor{{4,2,-1},{3.1,1.2,4.9}}
for _,name in ipairs({'lt','le','gt','ge','ne','eq'}) do
  print(loadstring('return c:' .. name .. '(5)')())
end
for _,name in ipairs({'lt','le','gt','ge','ne','eq'}) do
  print(loadstring('return torch.' .. name .. '(c,d)')())
end
</pre></tr>

<tr><td>Overloaded operators <td>80% done<td><pre>
d = torch.ClTensor{{3,5,-2},{2.1,2.2,3.9}}
c = torch.ClTensor{{4,2,-1},{3.1,1.2,4.9}}
c = c + d
c = c - d
c = c / 2
c = c * 1.5
c = c + 4
c = c - 5

A = torch.ClTensor{{1,2,-1},
                   {3,4,0}}
B = torch.ClTensor{{0,1},
                   {1,2},
                   {4,5}}
print( A * B)

v1 = torch.ClTensor{3,5,1}
v2 = torch.ClTensor{2,4,8}
print(v1 * v2)
</pre></tr>

<tr><td>Column or row-wise operations<td>5% done<td><pre>
A = torch.ClTensor{{3,5,2},{4,5,6}}
A:sum()
A:sum(2)
A:sum(1)
A:max()
A:max(1) -- only returns the result, not the indices
A:max(2) -- only returns the result, not the indices
A:min()
A:min(1) -- only returns the result, not the indices
A:min(2) -- only returns the result, not the indices

</pre></tr>

</table>

# Installation

* First install torch distro, see [https://github.com/torch/distro](https://github.com/torch/distro).
* Now, git clone the cltorch distro, cd into it, and run:
```
luarocks make rocks/cltorch-scm-1.rockspec
```

# Updating

* Sometimes you might want to do `git pull` to pull in new updates
* If you try this, you might see build errors about EasyCL
* In the future, these might be handled automatically by the build script :-)
* For now, note that two possible issues after a `git pull` are
  * the EasyCL submodule may not have been updated
  * the EasyCL subproject may not have been rebuilt
* To solve these issues, after doing `git pull`, you can do the following, which will ensure the EasyCL submodule is up to date, and will be fully rebuilt, and installed:
```
git submodule update
rm -Rf build/EasyCL
```
* Now you can run the luarocks make command, as above, and hopefully it will work this time :-)
  * if it doesnt, please raise an issue.  It might be easy to fix, but I cant help to fix it, if I dont know about it :-)

# Migration status by file

Porting status by file, compared with original cutorch files.  Note that `.cpp` here could have been ported from `.c`, `.cpp`, or `.cu`.

| File | Migration status |
|---|---|
| THClTensorMathCompare.cpp | Done |
| THClTensormathCompareT.cpp | Done |
| THClTensorMathPairwise.cpp | Done |
| THClTensor.h | Done |
| THClTensorCopy.h | Done |
| THClTensorMath.h | Done |
| THClTensor.cpp | 90% |
| THClTensorCopy.cpp | 50% |
| THClTensorMath.cpp | 50% |
| THClTensorIndex.cpp | 0% |
| THClTensorMath2.cpp | 20% |
| THClTensorMathBlas.cpp | 30% |
| THClBlas.cpp | 50% |
| THClReduce.* | 90% |
| THClReduceAll.* | 70% |
| THClGeneral.* | 30% |
| THClTensorMathTransformReduce.* | 0% |

# Dependencies

cltorch has the following build dependencies:
* [lua 5.1](http://www.lua.org/versions.html) libraries - used for runtime Kernel templating
* [clBLAS](https://github.com/clMathLibraries/clBLAS) - provides GPU-based matrix operations, such as multiplication
* [EasyCL](https://github.com/hughperkins/EasyCL) - provides an abstraction layer over the low-level OpenCL API
* [clew](https://github.com/martijnberger/clew) - similar to glew, means that cltorch can be loaded without any OpenCL library/runtime being present

At runtime, if you want to call any of the cltorch methods, you will also need:
* OpenCL-compatible GPU
* OpenCL library/driver (normally provided by the GPU vendor)

# Requests for additional operations etc

* Please raise an issue for any operations etc which you particularly need, or you feel are not working for some reason.
* (Ditto for any build errors)

# Guidelines for contributors

You might or might not find [ContributorGuidelines.md](doc/ContributorGuidelines.md) useful.  Anyway, it is there if you want to see my own thoughts and ideas on how to approach cltorch development, and cutorch-porting.

# Recent changes

* 15th-17th June:
  * pow(x,y) no longer returns undefined values for x containing, or being, negative
  * pow(x,y) now uses `pown` when y is an exact integer scalar (ie where (float)((int)y) == y)
  * when no opencl-enabled devices enabled, now raise a THError, with a clear error message, rather than throwing a C++ exception, with no error message output
  * under the hood: added cltorch.getState()
  * renamed libTHCL.so to libTHCl.so
  * added THCl include files to `install` section
  * masked fill works now
  * torch.addr works now
* 15th June:
  * C:t() working
* 14th June:
  * ReduceAll working :-)  For now means: sometensor:sum() works
  * sometensor:sum(1) and sometensor:sum(2) working too now :-)
  * A:min(), A:max() added
  * created unit tests, in [test](test) directory, [cltorch-unit-tensor.lua](test/cltorch-unit-tensor.lua) which pass
* 13th June:
  * added `cltorch.setDevice`/`cltorch.getDevice`, see [test-device.lua](test/test-device.lua) for an example
  * added EasyCL includes to EasyCL install section, to remove build errors with "EasyCL.h" not found, etc

