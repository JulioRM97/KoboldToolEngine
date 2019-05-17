#pragma once

#include <cstdio>
#include <minitrace.h>

#define TODO(what) fprintf(stderr, "TODO %s %s at %s:%d\n", what, __func__, __FILE__, __LINE__);
#define FIXME(what) fprintf(stderr, "FIXME %s %s at %s:%d\n", what, __func__, __FILE__, __LINE__);

#define TODO_IMPLEMENT TODO("implement");


#ifdef MTR_ENABLED
#define $ MTR_SCOPE(__FILE__, __FUNCTION__);
#define $N(name) MTR_SCOPE(__FILE__, name);
#define $F MTRScopedFlow  ____mtr_scope_flow(__FILE__, __FUNCTION__);
#else
#define $
#define $N(name) 
#define $F 
#endif




