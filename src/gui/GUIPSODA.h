/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class gui_PSODA */

#ifndef _Included_gui_PSODA
#define _Included_gui_PSODA
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     gui_PSODA
 * Method:    initInterpreter
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_initInterpreter
  (JNIEnv *, jobject, jlong);

/*
 * Class:     gui_PSODA
 * Method:    interpretFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_interpretFile
  (JNIEnv *, jobject, jstring);

/*
 * Class:     gui_PSODA
 * Method:    deleteInterpreter
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_deleteInterpreter
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    pausePSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_pausePSODA
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    resumePSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_resumePSODA
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    killPSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_killPSODA
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    writeTreeSAAPPSODA
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_writeTreeSAAPPSODA
  (JNIEnv *, jobject, jstring);

/*
 * Class:     gui_PSODA
 * Method:    saveTrees
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_gui_PSODA_saveTrees
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     gui_PSODA
 * Method:    saveAlign
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_gui_PSODA_saveAlign
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     gui_PSODA
 * Method:    getTrees
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getTrees
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    getTaxaNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getTaxaNames
  (JNIEnv *, jobject);

/*
 * Class:     gui_PSODA
 * Method:    getDefinedCommandNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getDefinedCommandNames
  (JNIEnv *, jclass);

/*
 * Class:     gui_PSODA
 * Method:    getPopularCommandNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getPopularCommandNames
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class gui_PSODA_CaretListenerLabel */

#ifndef _Included_gui_PSODA_CaretListenerLabel
#define _Included_gui_PSODA_CaretListenerLabel
#ifdef __cplusplus
extern "C" {
#endif
#undef gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_UNKNOWN
#define gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_UNKNOWN 0L
#undef gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_DEFAULT
#define gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_DEFAULT 1L
#undef gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_SET
#define gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSABLE_SET 2L
#undef gui_PSODA_CaretListenerLabel_TOP_ALIGNMENT
#define gui_PSODA_CaretListenerLabel_TOP_ALIGNMENT 0.0f
#undef gui_PSODA_CaretListenerLabel_CENTER_ALIGNMENT
#define gui_PSODA_CaretListenerLabel_CENTER_ALIGNMENT 0.5f
#undef gui_PSODA_CaretListenerLabel_BOTTOM_ALIGNMENT
#define gui_PSODA_CaretListenerLabel_BOTTOM_ALIGNMENT 1.0f
#undef gui_PSODA_CaretListenerLabel_LEFT_ALIGNMENT
#define gui_PSODA_CaretListenerLabel_LEFT_ALIGNMENT 0.0f
#undef gui_PSODA_CaretListenerLabel_RIGHT_ALIGNMENT
#define gui_PSODA_CaretListenerLabel_RIGHT_ALIGNMENT 1.0f
#undef gui_PSODA_CaretListenerLabel_serialVersionUID
#define gui_PSODA_CaretListenerLabel_serialVersionUID -7644114512714619750LL
#undef gui_PSODA_CaretListenerLabel_serialVersionUID
#define gui_PSODA_CaretListenerLabel_serialVersionUID 4613797578919906343LL
#undef gui_PSODA_CaretListenerLabel_INCLUDE_SELF
#define gui_PSODA_CaretListenerLabel_INCLUDE_SELF 1L
#undef gui_PSODA_CaretListenerLabel_SEARCH_HEAVYWEIGHTS
#define gui_PSODA_CaretListenerLabel_SEARCH_HEAVYWEIGHTS 1L
#undef gui_PSODA_CaretListenerLabel_NOT_OBSCURED
#define gui_PSODA_CaretListenerLabel_NOT_OBSCURED 0L
#undef gui_PSODA_CaretListenerLabel_PARTIALLY_OBSCURED
#define gui_PSODA_CaretListenerLabel_PARTIALLY_OBSCURED 1L
#undef gui_PSODA_CaretListenerLabel_COMPLETELY_OBSCURED
#define gui_PSODA_CaretListenerLabel_COMPLETELY_OBSCURED 2L
#undef gui_PSODA_CaretListenerLabel_WHEN_FOCUSED
#define gui_PSODA_CaretListenerLabel_WHEN_FOCUSED 0L
#undef gui_PSODA_CaretListenerLabel_WHEN_ANCESTOR_OF_FOCUSED_COMPONENT
#define gui_PSODA_CaretListenerLabel_WHEN_ANCESTOR_OF_FOCUSED_COMPONENT 1L
#undef gui_PSODA_CaretListenerLabel_WHEN_IN_FOCUSED_WINDOW
#define gui_PSODA_CaretListenerLabel_WHEN_IN_FOCUSED_WINDOW 2L
#undef gui_PSODA_CaretListenerLabel_UNDEFINED_CONDITION
#define gui_PSODA_CaretListenerLabel_UNDEFINED_CONDITION -1L
#undef gui_PSODA_CaretListenerLabel_IS_DOUBLE_BUFFERED
#define gui_PSODA_CaretListenerLabel_IS_DOUBLE_BUFFERED 0L
#undef gui_PSODA_CaretListenerLabel_ANCESTOR_USING_BUFFER
#define gui_PSODA_CaretListenerLabel_ANCESTOR_USING_BUFFER 1L
#undef gui_PSODA_CaretListenerLabel_IS_PAINTING_TILE
#define gui_PSODA_CaretListenerLabel_IS_PAINTING_TILE 2L
#undef gui_PSODA_CaretListenerLabel_IS_OPAQUE
#define gui_PSODA_CaretListenerLabel_IS_OPAQUE 3L
#undef gui_PSODA_CaretListenerLabel_KEY_EVENTS_ENABLED
#define gui_PSODA_CaretListenerLabel_KEY_EVENTS_ENABLED 4L
#undef gui_PSODA_CaretListenerLabel_FOCUS_INPUTMAP_CREATED
#define gui_PSODA_CaretListenerLabel_FOCUS_INPUTMAP_CREATED 5L
#undef gui_PSODA_CaretListenerLabel_ANCESTOR_INPUTMAP_CREATED
#define gui_PSODA_CaretListenerLabel_ANCESTOR_INPUTMAP_CREATED 6L
#undef gui_PSODA_CaretListenerLabel_WIF_INPUTMAP_CREATED
#define gui_PSODA_CaretListenerLabel_WIF_INPUTMAP_CREATED 7L
#undef gui_PSODA_CaretListenerLabel_ACTIONMAP_CREATED
#define gui_PSODA_CaretListenerLabel_ACTIONMAP_CREATED 8L
#undef gui_PSODA_CaretListenerLabel_CREATED_DOUBLE_BUFFER
#define gui_PSODA_CaretListenerLabel_CREATED_DOUBLE_BUFFER 9L
#undef gui_PSODA_CaretListenerLabel_IS_PRINTING
#define gui_PSODA_CaretListenerLabel_IS_PRINTING 11L
#undef gui_PSODA_CaretListenerLabel_IS_PRINTING_ALL
#define gui_PSODA_CaretListenerLabel_IS_PRINTING_ALL 12L
#undef gui_PSODA_CaretListenerLabel_IS_REPAINTING
#define gui_PSODA_CaretListenerLabel_IS_REPAINTING 13L
#undef gui_PSODA_CaretListenerLabel_WRITE_OBJ_COUNTER_FIRST
#define gui_PSODA_CaretListenerLabel_WRITE_OBJ_COUNTER_FIRST 14L
#undef gui_PSODA_CaretListenerLabel_RESERVED_1
#define gui_PSODA_CaretListenerLabel_RESERVED_1 15L
#undef gui_PSODA_CaretListenerLabel_RESERVED_2
#define gui_PSODA_CaretListenerLabel_RESERVED_2 16L
#undef gui_PSODA_CaretListenerLabel_RESERVED_3
#define gui_PSODA_CaretListenerLabel_RESERVED_3 17L
#undef gui_PSODA_CaretListenerLabel_RESERVED_4
#define gui_PSODA_CaretListenerLabel_RESERVED_4 18L
#undef gui_PSODA_CaretListenerLabel_RESERVED_5
#define gui_PSODA_CaretListenerLabel_RESERVED_5 19L
#undef gui_PSODA_CaretListenerLabel_RESERVED_6
#define gui_PSODA_CaretListenerLabel_RESERVED_6 20L
#undef gui_PSODA_CaretListenerLabel_WRITE_OBJ_COUNTER_LAST
#define gui_PSODA_CaretListenerLabel_WRITE_OBJ_COUNTER_LAST 21L
#undef gui_PSODA_CaretListenerLabel_REQUEST_FOCUS_DISABLED
#define gui_PSODA_CaretListenerLabel_REQUEST_FOCUS_DISABLED 22L
#undef gui_PSODA_CaretListenerLabel_INHERITS_POPUP_MENU
#define gui_PSODA_CaretListenerLabel_INHERITS_POPUP_MENU 23L
#undef gui_PSODA_CaretListenerLabel_OPAQUE_SET
#define gui_PSODA_CaretListenerLabel_OPAQUE_SET 24L
#undef gui_PSODA_CaretListenerLabel_AUTOSCROLLS_SET
#define gui_PSODA_CaretListenerLabel_AUTOSCROLLS_SET 25L
#undef gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSAL_KEYS_FORWARD_SET
#define gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSAL_KEYS_FORWARD_SET 26L
#undef gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSAL_KEYS_BACKWARD_SET
#define gui_PSODA_CaretListenerLabel_FOCUS_TRAVERSAL_KEYS_BACKWARD_SET 27L
#undef gui_PSODA_CaretListenerLabel_serialVersionUID
#define gui_PSODA_CaretListenerLabel_serialVersionUID -7010655128826618290LL
#ifdef __cplusplus
}
#endif
#endif
