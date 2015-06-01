#ifdef __ANDROID__
#include <jni.h>
#include <stdlib.h>
#include <android/log.h>
#include <sstream>
#include <stdio.h>
#include <cmath>
#else
#ifdef __APPLE__
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#else
#include "..\..\llge\stdafx.h"
#endif
#endif

#include <new>
#include <map>
#include <string>
#include <vector>
#include <time.h>