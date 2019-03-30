![GitHub](https://img.shields.io/github/license/BenSokol/EECS675-Project2.svg)
# EECS675-Project2

## Project Specification
The project specification is located [here](https://web.archive.org/web/20190308173240/https://people.eecs.ku.edu/~jrmiller/Courses/675/Spring2019/Projects/Project2.html).

## Requirements
|                 | Required                   | Prefered                      |
| --------------- | -------------------------- | ----------------------------- |
| OS              | Linux<br>macOS             |                               |
| CPU             |                            | Dual core Processor or better |
| Compiler        | gcc >= 5.4<br>clang >= 3.4 | gcc >= 6<br>clang >= 3.7      |
| C Version       | c11                        |                               |
| C++ Version     | c++14                      | c++17                         |
| openMPI Version | 1.10                       |                               |

## To use
### To compile
```bash
make
```
For compilation help, see [this](https://github.com/BenSokol/build-tools) page.

### To run
```bash
mpirun <mpi flags> reportGenerator dataFileName reportYear customerType
```
Where:  
dataFileName is file to read in from.  
reportYear is the year to search for (1997-2018).  
customerType is the customer type to search for (G, I, R).  

### Example
```bash
mpirun -np 20 reportGenerator Data3000.txt 2007 I
```

## License
[MIT](https://github.com/BenSokol/EECS675-Project2/blob/master/LICENSE).
