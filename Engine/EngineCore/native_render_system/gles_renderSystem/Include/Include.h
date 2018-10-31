#pragma once

#if (defined __IOS__)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif (defined _WIN32)
#include "EGL/eglplatform.h"
#include "EGL/egl.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include "../GLES2/E3DES2Loader.h"
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

#include <stdio.h>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <fstream>
