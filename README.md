<div align="justify">

# TOOLBOX

This project is a Git repository that contains several generic libraries to simplify building applications. These libraries can be used to speed up development and improve the quality of the code.

---

## 1.- Getting Started

This libraries can be downloaded with the next command:

```bash
$ git clone git@github.com:JosebaRG/toolbox.git
```

---

## 2.- Libraries

### 2.1.- LibJString

This library written in C was created to simplify working with strings in C. The implemented functions aim to provide similar functionality to that obtained in LabVIEW.

Library can be included in C with:

```c
#include "libjstring.h"
```

### 2.2.- LibJXml

This library written in C defindes a structure tree to store an XML file in memory. This library provides functions to convert the XML file into the tree structure and vice versa.

Library can be included in C with:

```c
#include "libjxml.h"
```

### 2.3.- LibJQueue

This library written in C provides tools to manage queues of different types like: circular, FiFo and LiFo.

Library can be included in C with:

```c
#include "libjqueue.h"
```

### 2.4.- LibJLog

This library written in C gives an easy way to manage system logs.

Library can be included in C with:

```c
#include "libjlog.h"
```

### 2.5.- LibJAssert

This library written in C provides a collection of functions that use the assert() macro to check the integrity of their input parameters. By including this library in one project, it can easily incorporate parameter checking into the code without having to write your specific custom error-checking code.

Library can be included in C with:

```c
#include "libjassert.h"
```

### 2.6.- LibJTypes

This library written in C defines the data types in a more visual format.

Library can be included in C with:

```c
#include "libjtypes.h"
```

### 2.7.- LibJContainer

This library written in C allows accesing the container struct of the provided data.

Library can be included in C with:

```c
#include "libjcontainer.h"
```

---

### 3.- Contributing

If you find a bug or have an enhancement request, please open an issue. If you want to contribute, please fork the repository and make changes as you'd like. Pull requests are warmly welcome.

---

### 4.- License
This Git repository contains my personal libraries and is licensed under the Creative Commons Non-Commercial (CC BY-NC) license. This means that while you are free to use and modify these libraries for non-commercial purposes, you must give me credit as the original author and cannot use them for commercial purposes without my permission.

---

### 5.- Author

- Name: Joseba R.G.
- Email: joseba.rg@protonmail.com
- GitHub: https://github.com/JosebaRG
