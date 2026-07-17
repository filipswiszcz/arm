#ifndef ARM_COMMON_H
#define ARM_COMMON_H

#define ENGINE_LOG(_m, ...) ((void) 0)
#define ENGINE_ASSERT(_e) ((_e) ? 1 : ENGINE_LOG("%s,%d: Assertion '%s' failed: \n", __FILE__, __LINE__, #_e), 0)

#endif // !ARM_COMMON_H