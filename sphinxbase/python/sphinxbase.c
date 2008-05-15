/* Generated by Pyrex 0.9.6.4 on Wed May 14 21:55:48 2008 */

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "structmember.h"
#ifndef PY_LONG_LONG
  #define PY_LONG_LONG LONG_LONG
#endif
#if PY_VERSION_HEX < 0x02050000
  typedef int Py_ssize_t;
  #define PY_SSIZE_T_MAX INT_MAX
  #define PY_SSIZE_T_MIN INT_MIN
  #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
  #define PyInt_AsSsize_t(o)	PyInt_AsLong(o)
#endif
#ifndef WIN32
  #ifndef __stdcall
    #define __stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
#endif
#ifdef __cplusplus
#define __PYX_EXTERN_C extern "C"
#else
#define __PYX_EXTERN_C extern
#endif
#include <math.h>
#include "logmath.h"
#include "cmd_ln.h"
#include "ckd_alloc.h"
#include "ngram_model.h"


typedef struct {PyObject **p; char *s;} __Pyx_InternTabEntry; /*proto*/
typedef struct {PyObject **p; char *s; long n;} __Pyx_StringTabEntry; /*proto*/

static PyObject *__pyx_m;
static PyObject *__pyx_b;
static int __pyx_lineno;
static char *__pyx_filename;
static char **__pyx_f;

static int __Pyx_GetStarArgs(PyObject **args, PyObject **kwds, char *kwd_list[],     Py_ssize_t nargs, PyObject **args2, PyObject **kwds2, char rqd_kwds[]); /*proto*/

static void __Pyx_AddTraceback(char *funcname); /*proto*/

/* Declarations from sphinxbase */

struct __pyx_obj_10sphinxbase_NGramModel {
  PyObject_HEAD
  ngram_model_t *lm;
  logmath_t *lmath;
};


static PyTypeObject *__pyx_ptype_10sphinxbase_NGramModel = 0;
static PyObject *__pyx_k1;
static PyObject *__pyx_k2;
static PyObject *__pyx_k3;
static PyObject *__pyx_k4;
static PyObject *__pyx_k5;
static PyObject *__pyx_k6;


/* Implementation of sphinxbase */

static int __pyx_f_10sphinxbase_10NGramModel___cinit__(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static int __pyx_f_10sphinxbase_10NGramModel___cinit__(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_file = 0;
  PyObject *__pyx_v_lw = 0;
  PyObject *__pyx_v_wip = 0;
  PyObject *__pyx_v_uw = 0;
  int __pyx_r;
  logmath_t *__pyx_1;
  char *__pyx_2;
  ngram_model_t *__pyx_3;
  float32 __pyx_4;
  float32 __pyx_5;
  float32 __pyx_6;
  int __pyx_7;
  static char *__pyx_argnames[] = {"file","lw","wip","uw",0};
  __pyx_v_lw = __pyx_k1;
  __pyx_v_wip = __pyx_k2;
  __pyx_v_uw = __pyx_k3;
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O|OOO", __pyx_argnames, &__pyx_v_file, &__pyx_v_lw, &__pyx_v_wip, &__pyx_v_uw)) return -1;
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_file);
  Py_INCREF(__pyx_v_lw);
  Py_INCREF(__pyx_v_wip);
  Py_INCREF(__pyx_v_uw);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":89 */
  __pyx_1 = logmath_init(1.0001,0,0); if (__pyx_1 == NULL) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 89; goto __pyx_L1;}
  ((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lmath = __pyx_1;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":90 */
  __pyx_2 = PyString_AsString(__pyx_v_file); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 90; goto __pyx_L1;}
  __pyx_3 = ngram_model_read(NULL,__pyx_2,NGRAM_AUTO,((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lmath); if (__pyx_3 == NULL) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 90; goto __pyx_L1;}
  ((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm = __pyx_3;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":91 */
  __pyx_4 = PyFloat_AsDouble(__pyx_v_lw); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 91; goto __pyx_L1;}
  __pyx_5 = PyFloat_AsDouble(__pyx_v_wip); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 91; goto __pyx_L1;}
  __pyx_6 = PyFloat_AsDouble(__pyx_v_uw); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 91; goto __pyx_L1;}
  __pyx_7 = ngram_model_apply_weights(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_4,__pyx_5,__pyx_6); if (__pyx_7 == (-1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 91; goto __pyx_L1;}

  __pyx_r = 0;
  goto __pyx_L0;
  __pyx_L1:;
  __Pyx_AddTraceback("sphinxbase.NGramModel.__cinit__");
  __pyx_r = -1;
  __pyx_L0:;
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_file);
  Py_DECREF(__pyx_v_lw);
  Py_DECREF(__pyx_v_wip);
  Py_DECREF(__pyx_v_uw);
  return __pyx_r;
}

static void __pyx_f_10sphinxbase_10NGramModel___dealloc__(PyObject *__pyx_v_self); /*proto*/
static void __pyx_f_10sphinxbase_10NGramModel___dealloc__(PyObject *__pyx_v_self) {
  Py_INCREF(__pyx_v_self);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":97 */
  logmath_free(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lmath);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":98 */
  ngram_model_free(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm);

  Py_DECREF(__pyx_v_self);
}

static PyObject *__pyx_f_10sphinxbase_10NGramModel_apply_weights(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_10sphinxbase_10NGramModel_apply_weights[] = "\n        Change the language model weights applied in L{score}.\n        \n        @param lw: Language weight to apply to model probabilities.\n        @type lw: float\n        @param wip: Word insertion penalty to add to model probabilities\n        @type wip: float\n        @param uw: Weight to give unigrams when interpolating with uniform distribution.\n        @type uw: float\n        ";
static PyObject *__pyx_f_10sphinxbase_10NGramModel_apply_weights(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_lw = 0;
  PyObject *__pyx_v_wip = 0;
  PyObject *__pyx_v_uw = 0;
  PyObject *__pyx_r;
  float32 __pyx_1;
  float32 __pyx_2;
  float32 __pyx_3;
  int __pyx_4;
  static char *__pyx_argnames[] = {"lw","wip","uw",0};
  __pyx_v_lw = __pyx_k4;
  __pyx_v_wip = __pyx_k5;
  __pyx_v_uw = __pyx_k6;
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "|OOO", __pyx_argnames, &__pyx_v_lw, &__pyx_v_wip, &__pyx_v_uw)) return 0;
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_lw);
  Py_INCREF(__pyx_v_wip);
  Py_INCREF(__pyx_v_uw);
  __pyx_1 = PyFloat_AsDouble(__pyx_v_lw); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 111; goto __pyx_L1;}
  __pyx_2 = PyFloat_AsDouble(__pyx_v_wip); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 111; goto __pyx_L1;}
  __pyx_3 = PyFloat_AsDouble(__pyx_v_uw); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 111; goto __pyx_L1;}
  __pyx_4 = ngram_model_apply_weights(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1,__pyx_2,__pyx_3); if (__pyx_4 == (-1)) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 111; goto __pyx_L1;}

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  __Pyx_AddTraceback("sphinxbase.NGramModel.apply_weights");
  __pyx_r = 0;
  __pyx_L0:;
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_lw);
  Py_DECREF(__pyx_v_wip);
  Py_DECREF(__pyx_v_uw);
  return __pyx_r;
}

static PyObject *__pyx_f_10sphinxbase_10NGramModel_wid(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_10sphinxbase_10NGramModel_wid[] = "\n        Get the internal ID for a word.\n        \n        @param word: Word in question\n        @type word: string\n        @return: Internal ID for word\n        @rtype: int\n        ";
static PyObject *__pyx_f_10sphinxbase_10NGramModel_wid(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_word = 0;
  PyObject *__pyx_r;
  char *__pyx_1;
  PyObject *__pyx_2 = 0;
  static char *__pyx_argnames[] = {"word",0};
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O", __pyx_argnames, &__pyx_v_word)) return 0;
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_word);
  __pyx_1 = PyString_AsString(__pyx_v_word); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 122; goto __pyx_L1;}
  __pyx_2 = PyInt_FromLong(ngram_wid(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1)); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 122; goto __pyx_L1;}
  __pyx_r = __pyx_2;
  __pyx_2 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  Py_XDECREF(__pyx_2);
  __Pyx_AddTraceback("sphinxbase.NGramModel.wid");
  __pyx_r = 0;
  __pyx_L0:;
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_word);
  return __pyx_r;
}

static PyObject *__pyx_f_10sphinxbase_10NGramModel_word(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_10sphinxbase_10NGramModel_word[] = "\n        Get the string corresponding to an internal word ID.\n        \n        @param word: Word ID in question\n        @type word: int\n        @return: String for word\n        @rtype: string\n        ";
static PyObject *__pyx_f_10sphinxbase_10NGramModel_word(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_wid = 0;
  PyObject *__pyx_r;
  int32 __pyx_1;
  char *__pyx_2;
  PyObject *__pyx_3 = 0;
  static char *__pyx_argnames[] = {"wid",0};
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O", __pyx_argnames, &__pyx_v_wid)) return 0;
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_wid);
  __pyx_1 = PyInt_AsLong(__pyx_v_wid); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; goto __pyx_L1;}
  __pyx_2 = ngram_word(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1); if (__pyx_2 == NULL) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; goto __pyx_L1;}
  __pyx_3 = PyString_FromString(__pyx_2); if (!__pyx_3) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 133; goto __pyx_L1;}
  __pyx_r = __pyx_3;
  __pyx_3 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  Py_XDECREF(__pyx_3);
  __Pyx_AddTraceback("sphinxbase.NGramModel.word");
  __pyx_r = 0;
  __pyx_L0:;
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_wid);
  return __pyx_r;
}

static PyObject *__pyx_f_10sphinxbase_10NGramModel_score(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_10sphinxbase_10NGramModel_score[] = "\n        Get the score for an N-Gram.\n\n        The argument list consists of the history words (as\n        null-terminated strings) of the N-Gram, in reverse order.\n        Therefore, if you wanted to get the N-Gram score for \"a whole\n        joy\", you would call::\n\n         score, n_used = model.score(\"joy\", \"whole\", \"a\")\n\n        This function returns a tuple, consisting of the score and the\n        number of words used in computing it (i.e. the effective size\n        of the N-Gram).  The score is returned in logarithmic form,\n        using base e.\n\n        If one of the words is not in the LM\'s vocabulary, the result\n        will depend on whether this is an open or closed vocabulary\n        language model.  For an open-vocabulary model, unknown words\n        are all mapped to the unigram <UNK> which has a non-zero\n        probability and also participates in higher-order N-Grams.\n        Therefore, you will get a score of some sort in this case.\n\n        For a closed-vocabulary model, unknown words are impossible\n        and thus have zero probability.  Therefore, if C{word} is\n        unknown, this function will return a \"zero\" log-probability,\n        i.e. a large negative number.  To obtain this number for\n        comparison, call L{the C{zero} method<zero>}.\n        ";
static PyObject *__pyx_f_10sphinxbase_10NGramModel_score(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_word = 0;
  PyObject *__pyx_v_args = 0;
  int32 __pyx_v_wid;
  int32 *__pyx_v_hist;
  int32 __pyx_v_n_hist;
  int32 __pyx_v_n_used;
  int32 __pyx_v_score;
  PyObject *__pyx_v_i;
  PyObject *__pyx_v_spam;
  PyObject *__pyx_r;
  char *__pyx_1;
  Py_ssize_t __pyx_2;
  long __pyx_3;
  PyObject *__pyx_4 = 0;
  PyObject *__pyx_5 = 0;
  PyObject *__pyx_6 = 0;
  static char *__pyx_argnames[] = {"word",0};
  if (__Pyx_GetStarArgs(&__pyx_args, &__pyx_kwds, __pyx_argnames, 1, &__pyx_v_args, 0, 0) < 0) return 0;
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O", __pyx_argnames, &__pyx_v_word)) {
    Py_XDECREF(__pyx_args);
    Py_XDECREF(__pyx_kwds);
    Py_XDECREF(__pyx_v_args);
    return 0;
  }
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_word);
  __pyx_v_i = Py_None; Py_INCREF(Py_None);
  __pyx_v_spam = Py_None; Py_INCREF(Py_None);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":170 */
  __pyx_1 = PyString_AsString(__pyx_v_word); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 170; goto __pyx_L1;}
  __pyx_v_wid = ngram_wid(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":171 */
  __pyx_2 = PyObject_Length(__pyx_v_args); if (__pyx_2 == -1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 171; goto __pyx_L1;}
  __pyx_v_n_hist = __pyx_2;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":172 */
  __pyx_v_hist = ((int32 *)ckd_calloc(__pyx_v_n_hist,(sizeof(int32))));

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":173 */
  for (__pyx_3 = 0; __pyx_3 < __pyx_v_n_hist; ++__pyx_3) {
    __pyx_4 = PyInt_FromLong(__pyx_3); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 173; goto __pyx_L1;}
    Py_DECREF(__pyx_v_i);
    __pyx_v_i = __pyx_4;
    __pyx_4 = 0;

    /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":174 */
    __pyx_4 = PyObject_GetItem(__pyx_v_args, __pyx_v_i); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 174; goto __pyx_L1;}
    Py_DECREF(__pyx_v_spam);
    __pyx_v_spam = __pyx_4;
    __pyx_4 = 0;

    /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":175 */
    __pyx_1 = PyString_AsString(__pyx_v_spam); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 175; goto __pyx_L1;}
    __pyx_2 = PyInt_AsSsize_t(__pyx_v_i); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 175; goto __pyx_L1;}
    (__pyx_v_hist[__pyx_2]) = ngram_wid(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1);
  }

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":176 */
  __pyx_v_score = ngram_ng_score(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_v_wid,__pyx_v_hist,__pyx_v_n_hist,(&__pyx_v_n_used));

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":177 */
  ckd_free(__pyx_v_hist);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":178 */
  __pyx_4 = PyFloat_FromDouble(logmath_exp(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lmath,__pyx_v_score)); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 178; goto __pyx_L1;}
  __pyx_5 = PyInt_FromLong(__pyx_v_n_used); if (!__pyx_5) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 178; goto __pyx_L1;}
  __pyx_6 = PyTuple_New(2); if (!__pyx_6) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 178; goto __pyx_L1;}
  PyTuple_SET_ITEM(__pyx_6, 0, __pyx_4);
  PyTuple_SET_ITEM(__pyx_6, 1, __pyx_5);
  __pyx_4 = 0;
  __pyx_5 = 0;
  __pyx_r = __pyx_6;
  __pyx_6 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  Py_XDECREF(__pyx_4);
  Py_XDECREF(__pyx_5);
  Py_XDECREF(__pyx_6);
  __Pyx_AddTraceback("sphinxbase.NGramModel.score");
  __pyx_r = 0;
  __pyx_L0:;
  Py_XDECREF(__pyx_v_args);
  Py_DECREF(__pyx_v_i);
  Py_DECREF(__pyx_v_spam);
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_word);
  Py_XDECREF(__pyx_args);
  Py_XDECREF(__pyx_kwds);
  return __pyx_r;
}

static PyObject *__pyx_f_10sphinxbase_10NGramModel_prob(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds); /*proto*/
static char __pyx_doc_10sphinxbase_10NGramModel_prob[] = "\n        Get the log-probability for an N-Gram.\n\n        This works effectively the same way as L{score}, except that\n        any weights (language weight, insertion penalty) applied to\n        the language model are ignored and the \"raw\" probability value\n        is returned.\n        ";
static PyObject *__pyx_f_10sphinxbase_10NGramModel_prob(PyObject *__pyx_v_self, PyObject *__pyx_args, PyObject *__pyx_kwds) {
  PyObject *__pyx_v_word = 0;
  PyObject *__pyx_v_args = 0;
  int32 __pyx_v_wid;
  int32 *__pyx_v_hist;
  int32 __pyx_v_n_hist;
  int32 __pyx_v_n_used;
  int32 __pyx_v_score;
  PyObject *__pyx_v_i;
  PyObject *__pyx_v_spam;
  PyObject *__pyx_r;
  char *__pyx_1;
  Py_ssize_t __pyx_2;
  long __pyx_3;
  PyObject *__pyx_4 = 0;
  PyObject *__pyx_5 = 0;
  PyObject *__pyx_6 = 0;
  static char *__pyx_argnames[] = {"word",0};
  if (__Pyx_GetStarArgs(&__pyx_args, &__pyx_kwds, __pyx_argnames, 1, &__pyx_v_args, 0, 0) < 0) return 0;
  if (!PyArg_ParseTupleAndKeywords(__pyx_args, __pyx_kwds, "O", __pyx_argnames, &__pyx_v_word)) {
    Py_XDECREF(__pyx_args);
    Py_XDECREF(__pyx_kwds);
    Py_XDECREF(__pyx_v_args);
    return 0;
  }
  Py_INCREF(__pyx_v_self);
  Py_INCREF(__pyx_v_word);
  __pyx_v_i = Py_None; Py_INCREF(Py_None);
  __pyx_v_spam = Py_None; Py_INCREF(Py_None);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":194 */
  __pyx_1 = PyString_AsString(__pyx_v_word); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 194; goto __pyx_L1;}
  __pyx_v_wid = ngram_wid(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":195 */
  __pyx_2 = PyObject_Length(__pyx_v_args); if (__pyx_2 == -1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 195; goto __pyx_L1;}
  __pyx_v_n_hist = __pyx_2;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":196 */
  __pyx_v_hist = ((int32 *)ckd_calloc(__pyx_v_n_hist,(sizeof(int32))));

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":197 */
  for (__pyx_3 = 0; __pyx_3 < __pyx_v_n_hist; ++__pyx_3) {
    __pyx_4 = PyInt_FromLong(__pyx_3); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 197; goto __pyx_L1;}
    Py_DECREF(__pyx_v_i);
    __pyx_v_i = __pyx_4;
    __pyx_4 = 0;

    /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":198 */
    __pyx_4 = PyObject_GetItem(__pyx_v_args, __pyx_v_i); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 198; goto __pyx_L1;}
    Py_DECREF(__pyx_v_spam);
    __pyx_v_spam = __pyx_4;
    __pyx_4 = 0;

    /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":199 */
    __pyx_1 = PyString_AsString(__pyx_v_spam); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 199; goto __pyx_L1;}
    __pyx_2 = PyInt_AsSsize_t(__pyx_v_i); if (PyErr_Occurred()) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 199; goto __pyx_L1;}
    (__pyx_v_hist[__pyx_2]) = ngram_wid(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_1);
  }

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":200 */
  __pyx_v_score = ngram_ng_prob(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lm,__pyx_v_wid,__pyx_v_hist,__pyx_v_n_hist,(&__pyx_v_n_used));

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":201 */
  ckd_free(__pyx_v_hist);

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":202 */
  __pyx_4 = PyFloat_FromDouble(logmath_exp(((struct __pyx_obj_10sphinxbase_NGramModel *)__pyx_v_self)->lmath,__pyx_v_score)); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 202; goto __pyx_L1;}
  __pyx_5 = PyInt_FromLong(__pyx_v_n_used); if (!__pyx_5) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 202; goto __pyx_L1;}
  __pyx_6 = PyTuple_New(2); if (!__pyx_6) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 202; goto __pyx_L1;}
  PyTuple_SET_ITEM(__pyx_6, 0, __pyx_4);
  PyTuple_SET_ITEM(__pyx_6, 1, __pyx_5);
  __pyx_4 = 0;
  __pyx_5 = 0;
  __pyx_r = __pyx_6;
  __pyx_6 = 0;
  goto __pyx_L0;

  __pyx_r = Py_None; Py_INCREF(Py_None);
  goto __pyx_L0;
  __pyx_L1:;
  Py_XDECREF(__pyx_4);
  Py_XDECREF(__pyx_5);
  Py_XDECREF(__pyx_6);
  __Pyx_AddTraceback("sphinxbase.NGramModel.prob");
  __pyx_r = 0;
  __pyx_L0:;
  Py_XDECREF(__pyx_v_args);
  Py_DECREF(__pyx_v_i);
  Py_DECREF(__pyx_v_spam);
  Py_DECREF(__pyx_v_self);
  Py_DECREF(__pyx_v_word);
  Py_XDECREF(__pyx_args);
  Py_XDECREF(__pyx_kwds);
  return __pyx_r;
}

static PyObject *__pyx_tp_new_10sphinxbase_NGramModel(PyTypeObject *t, PyObject *a, PyObject *k) {
  PyObject *o = (*t->tp_alloc)(t, 0);
  if (!o) return 0;
  if (__pyx_f_10sphinxbase_10NGramModel___cinit__(o, a, k) < 0) {
    Py_DECREF(o); o = 0;
  }
  return o;
}

static void __pyx_tp_dealloc_10sphinxbase_NGramModel(PyObject *o) {
  {
    PyObject *etype, *eval, *etb;
    PyErr_Fetch(&etype, &eval, &etb);
    ++o->ob_refcnt;
    __pyx_f_10sphinxbase_10NGramModel___dealloc__(o);
    if (PyErr_Occurred()) PyErr_WriteUnraisable(o);
    --o->ob_refcnt;
    PyErr_Restore(etype, eval, etb);
  }
  (*o->ob_type->tp_free)(o);
}

static int __pyx_tp_traverse_10sphinxbase_NGramModel(PyObject *o, visitproc v, void *a) {
  return 0;
}

static int __pyx_tp_clear_10sphinxbase_NGramModel(PyObject *o) {
  return 0;
}

static struct PyMethodDef __pyx_methods_10sphinxbase_NGramModel[] = {
  {"apply_weights", (PyCFunction)__pyx_f_10sphinxbase_10NGramModel_apply_weights, METH_VARARGS|METH_KEYWORDS, __pyx_doc_10sphinxbase_10NGramModel_apply_weights},
  {"wid", (PyCFunction)__pyx_f_10sphinxbase_10NGramModel_wid, METH_VARARGS|METH_KEYWORDS, __pyx_doc_10sphinxbase_10NGramModel_wid},
  {"word", (PyCFunction)__pyx_f_10sphinxbase_10NGramModel_word, METH_VARARGS|METH_KEYWORDS, __pyx_doc_10sphinxbase_10NGramModel_word},
  {"score", (PyCFunction)__pyx_f_10sphinxbase_10NGramModel_score, METH_VARARGS|METH_KEYWORDS, __pyx_doc_10sphinxbase_10NGramModel_score},
  {"prob", (PyCFunction)__pyx_f_10sphinxbase_10NGramModel_prob, METH_VARARGS|METH_KEYWORDS, __pyx_doc_10sphinxbase_10NGramModel_prob},
  {0, 0, 0, 0}
};

static PyNumberMethods __pyx_tp_as_number_NGramModel = {
  0, /*nb_add*/
  0, /*nb_subtract*/
  0, /*nb_multiply*/
  0, /*nb_divide*/
  0, /*nb_remainder*/
  0, /*nb_divmod*/
  0, /*nb_power*/
  0, /*nb_negative*/
  0, /*nb_positive*/
  0, /*nb_absolute*/
  0, /*nb_nonzero*/
  0, /*nb_invert*/
  0, /*nb_lshift*/
  0, /*nb_rshift*/
  0, /*nb_and*/
  0, /*nb_xor*/
  0, /*nb_or*/
  0, /*nb_coerce*/
  0, /*nb_int*/
  0, /*nb_long*/
  0, /*nb_float*/
  0, /*nb_oct*/
  0, /*nb_hex*/
  0, /*nb_inplace_add*/
  0, /*nb_inplace_subtract*/
  0, /*nb_inplace_multiply*/
  0, /*nb_inplace_divide*/
  0, /*nb_inplace_remainder*/
  0, /*nb_inplace_power*/
  0, /*nb_inplace_lshift*/
  0, /*nb_inplace_rshift*/
  0, /*nb_inplace_and*/
  0, /*nb_inplace_xor*/
  0, /*nb_inplace_or*/
  0, /*nb_floor_divide*/
  0, /*nb_true_divide*/
  0, /*nb_inplace_floor_divide*/
  0, /*nb_inplace_true_divide*/
  #if Py_TPFLAGS_DEFAULT & Py_TPFLAGS_HAVE_INDEX
  0, /*nb_index*/
  #endif
};

static PySequenceMethods __pyx_tp_as_sequence_NGramModel = {
  0, /*sq_length*/
  0, /*sq_concat*/
  0, /*sq_repeat*/
  0, /*sq_item*/
  0, /*sq_slice*/
  0, /*sq_ass_item*/
  0, /*sq_ass_slice*/
  0, /*sq_contains*/
  0, /*sq_inplace_concat*/
  0, /*sq_inplace_repeat*/
};

static PyMappingMethods __pyx_tp_as_mapping_NGramModel = {
  0, /*mp_length*/
  0, /*mp_subscript*/
  0, /*mp_ass_subscript*/
};

static PyBufferProcs __pyx_tp_as_buffer_NGramModel = {
  0, /*bf_getreadbuffer*/
  0, /*bf_getwritebuffer*/
  0, /*bf_getsegcount*/
  0, /*bf_getcharbuffer*/
};

PyTypeObject __pyx_type_10sphinxbase_NGramModel = {
  PyObject_HEAD_INIT(0)
  0, /*ob_size*/
  "sphinxbase.NGramModel", /*tp_name*/
  sizeof(struct __pyx_obj_10sphinxbase_NGramModel), /*tp_basicsize*/
  0, /*tp_itemsize*/
  __pyx_tp_dealloc_10sphinxbase_NGramModel, /*tp_dealloc*/
  0, /*tp_print*/
  0, /*tp_getattr*/
  0, /*tp_setattr*/
  0, /*tp_compare*/
  0, /*tp_repr*/
  &__pyx_tp_as_number_NGramModel, /*tp_as_number*/
  &__pyx_tp_as_sequence_NGramModel, /*tp_as_sequence*/
  &__pyx_tp_as_mapping_NGramModel, /*tp_as_mapping*/
  0, /*tp_hash*/
  0, /*tp_call*/
  0, /*tp_str*/
  0, /*tp_getattro*/
  0, /*tp_setattro*/
  &__pyx_tp_as_buffer_NGramModel, /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_HAVE_GC, /*tp_flags*/
  "\n    N-Gram language model class.\n\n    This class provides access to N-Gram language models stored on\n    disk.  These can be in ARPABO text format or Sphinx DMP format.\n    Methods are provided for scoring N-Grams based on the model,\n    looking up words in the model, and adding words to the model.\n    ", /*tp_doc*/
  __pyx_tp_traverse_10sphinxbase_NGramModel, /*tp_traverse*/
  __pyx_tp_clear_10sphinxbase_NGramModel, /*tp_clear*/
  0, /*tp_richcompare*/
  0, /*tp_weaklistoffset*/
  0, /*tp_iter*/
  0, /*tp_iternext*/
  __pyx_methods_10sphinxbase_NGramModel, /*tp_methods*/
  0, /*tp_members*/
  0, /*tp_getset*/
  0, /*tp_base*/
  0, /*tp_dict*/
  0, /*tp_descr_get*/
  0, /*tp_descr_set*/
  0, /*tp_dictoffset*/
  0, /*tp_init*/
  0, /*tp_alloc*/
  __pyx_tp_new_10sphinxbase_NGramModel, /*tp_new*/
  0, /*tp_free*/
  0, /*tp_is_gc*/
  0, /*tp_bases*/
  0, /*tp_mro*/
  0, /*tp_cache*/
  0, /*tp_subclasses*/
  0, /*tp_weaklist*/
};

static struct PyMethodDef __pyx_methods[] = {
  {0, 0, 0, 0}
};

static void __pyx_init_filenames(void); /*proto*/

PyMODINIT_FUNC initsphinxbase(void); /*proto*/
PyMODINIT_FUNC initsphinxbase(void) {
  PyObject *__pyx_1 = 0;
  PyObject *__pyx_2 = 0;
  PyObject *__pyx_3 = 0;
  PyObject *__pyx_4 = 0;
  PyObject *__pyx_5 = 0;
  PyObject *__pyx_6 = 0;
  __pyx_init_filenames();
  __pyx_m = Py_InitModule4("sphinxbase", __pyx_methods, 0, 0, PYTHON_API_VERSION);
  if (!__pyx_m) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 10; goto __pyx_L1;};
  Py_INCREF(__pyx_m);
  __pyx_b = PyImport_AddModule("__builtin__");
  if (!__pyx_b) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 10; goto __pyx_L1;};
  if (PyObject_SetAttrString(__pyx_m, "__builtins__", __pyx_b) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 10; goto __pyx_L1;};
  if (PyType_Ready(&__pyx_type_10sphinxbase_NGramModel) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 64; goto __pyx_L1;}
  if (PyObject_SetAttrString(__pyx_m, "NGramModel", (PyObject *)&__pyx_type_10sphinxbase_NGramModel) < 0) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 64; goto __pyx_L1;}
  __pyx_ptype_10sphinxbase_NGramModel = &__pyx_type_10sphinxbase_NGramModel;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":76 */
  __pyx_1 = PyFloat_FromDouble(1.0); if (!__pyx_1) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 76; goto __pyx_L1;}
  __pyx_k1 = __pyx_1;
  __pyx_1 = 0;
  __pyx_2 = PyFloat_FromDouble(1.0); if (!__pyx_2) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 76; goto __pyx_L1;}
  __pyx_k2 = __pyx_2;
  __pyx_2 = 0;
  __pyx_3 = PyFloat_FromDouble(1.0); if (!__pyx_3) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 76; goto __pyx_L1;}
  __pyx_k3 = __pyx_3;
  __pyx_3 = 0;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":100 */
  __pyx_4 = PyFloat_FromDouble(1.0); if (!__pyx_4) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; goto __pyx_L1;}
  __pyx_k4 = __pyx_4;
  __pyx_4 = 0;
  __pyx_5 = PyFloat_FromDouble(1.0); if (!__pyx_5) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; goto __pyx_L1;}
  __pyx_k5 = __pyx_5;
  __pyx_5 = 0;
  __pyx_6 = PyFloat_FromDouble(1.0); if (!__pyx_6) {__pyx_filename = __pyx_f[0]; __pyx_lineno = 100; goto __pyx_L1;}
  __pyx_k6 = __pyx_6;
  __pyx_6 = 0;

  /* "/home/dhuggins/Projects/Sphinx/sphinxbase/amd64-linux/python/../../python/sphinxbase.pyx":180 */
  return;
  __pyx_L1:;
  Py_XDECREF(__pyx_1);
  Py_XDECREF(__pyx_2);
  Py_XDECREF(__pyx_3);
  Py_XDECREF(__pyx_4);
  Py_XDECREF(__pyx_5);
  Py_XDECREF(__pyx_6);
  __Pyx_AddTraceback("sphinxbase");
}

static char *__pyx_filenames[] = {
  "sphinxbase.pyx",
};

/* Runtime support code */

static void __pyx_init_filenames(void) {
  __pyx_f = __pyx_filenames;
}

static int __Pyx_GetStarArgs(
    PyObject **args, 
    PyObject **kwds,
    char *kwd_list[], 
    Py_ssize_t nargs,
    PyObject **args2, 
    PyObject **kwds2,
    char rqd_kwds[])
{
    PyObject *x = 0, *args1 = 0, *kwds1 = 0;
    int i;
    char **p;
    
    if (args2)
        *args2 = 0;
    if (kwds2)
        *kwds2 = 0;
    
    if (args2) {
        args1 = PyTuple_GetSlice(*args, 0, nargs);
        if (!args1)
            goto bad;
        *args2 = PyTuple_GetSlice(*args, nargs, PyTuple_GET_SIZE(*args));
        if (!*args2)
            goto bad;
    }
    else if (PyTuple_GET_SIZE(*args) > nargs) {
        int m = nargs;
        int n = PyTuple_GET_SIZE(*args);
        PyErr_Format(PyExc_TypeError,
            "function takes at most %d positional arguments (%d given)",
                m, n);
        goto bad;
    }
    else {
        args1 = *args;
        Py_INCREF(args1);
    }
    
    if (rqd_kwds && !*kwds)
            for (i = 0, p = kwd_list; *p; i++, p++)
                if (rqd_kwds[i])
                    goto missing_kwarg;
    
    if (kwds2) {
        if (*kwds) {
            kwds1 = PyDict_New();
            if (!kwds1)
                goto bad;
            *kwds2 = PyDict_Copy(*kwds);
            if (!*kwds2)
                goto bad;
            for (i = 0, p = kwd_list; *p; i++, p++) {
                x = PyDict_GetItemString(*kwds, *p);
                if (x) {
                    if (PyDict_SetItemString(kwds1, *p, x) < 0)
                        goto bad;
                    if (PyDict_DelItemString(*kwds2, *p) < 0)
                        goto bad;
                }
                else if (rqd_kwds && rqd_kwds[i])
                    goto missing_kwarg;
            }
        }
        else {
            *kwds2 = PyDict_New();
            if (!*kwds2)
                goto bad;
        }
    }
    else {
        kwds1 = *kwds;
        Py_XINCREF(kwds1);
        if (rqd_kwds && *kwds)
            for (i = 0, p = kwd_list; *p; i++, p++)
                if (rqd_kwds[i] && !PyDict_GetItemString(*kwds, *p))
                        goto missing_kwarg;
    }
    
    *args = args1;
    *kwds = kwds1;
    return 0;
missing_kwarg:
    PyErr_Format(PyExc_TypeError,
        "required keyword argument '%s' is missing", *p);
bad:
    Py_XDECREF(args1);
    Py_XDECREF(kwds1);
    if (args2) {
        Py_XDECREF(*args2);
    }
    if (kwds2) {
        Py_XDECREF(*kwds2);
    }
    return -1;
}

#include "compile.h"
#include "frameobject.h"
#include "traceback.h"

static void __Pyx_AddTraceback(char *funcname) {
    PyObject *py_srcfile = 0;
    PyObject *py_funcname = 0;
    PyObject *py_globals = 0;
    PyObject *empty_tuple = 0;
    PyObject *empty_string = 0;
    PyCodeObject *py_code = 0;
    PyFrameObject *py_frame = 0;
    
    py_srcfile = PyString_FromString(__pyx_filename);
    if (!py_srcfile) goto bad;
    py_funcname = PyString_FromString(funcname);
    if (!py_funcname) goto bad;
    py_globals = PyModule_GetDict(__pyx_m);
    if (!py_globals) goto bad;
    empty_tuple = PyTuple_New(0);
    if (!empty_tuple) goto bad;
    empty_string = PyString_FromString("");
    if (!empty_string) goto bad;
    py_code = PyCode_New(
        0,            /*int argcount,*/
        0,            /*int nlocals,*/
        0,            /*int stacksize,*/
        0,            /*int flags,*/
        empty_string, /*PyObject *code,*/
        empty_tuple,  /*PyObject *consts,*/
        empty_tuple,  /*PyObject *names,*/
        empty_tuple,  /*PyObject *varnames,*/
        empty_tuple,  /*PyObject *freevars,*/
        empty_tuple,  /*PyObject *cellvars,*/
        py_srcfile,   /*PyObject *filename,*/
        py_funcname,  /*PyObject *name,*/
        __pyx_lineno,   /*int firstlineno,*/
        empty_string  /*PyObject *lnotab*/
    );
    if (!py_code) goto bad;
    py_frame = PyFrame_New(
        PyThreadState_Get(), /*PyThreadState *tstate,*/
        py_code,             /*PyCodeObject *code,*/
        py_globals,          /*PyObject *globals,*/
        0                    /*PyObject *locals*/
    );
    if (!py_frame) goto bad;
    py_frame->f_lineno = __pyx_lineno;
    PyTraceBack_Here(py_frame);
bad:
    Py_XDECREF(py_srcfile);
    Py_XDECREF(py_funcname);
    Py_XDECREF(empty_tuple);
    Py_XDECREF(empty_string);
    Py_XDECREF(py_code);
    Py_XDECREF(py_frame);
}
