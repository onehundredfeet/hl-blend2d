#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) blend2d_##x
#include <hl.h>

// Need to link in helpers
HL_API hl_type hltx_ui16;
HL_API hl_type hltx_ui8;

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define _unref_ptr_safe(v) (v != nullptr ? v->value : nullptr)
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

// Float vector
struct _hl_float2 {
	float x;
	float y;
};

struct _hl_float3 {
	float x;
	float y;
	float z;
};

struct _hl_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _hl_int2 {
	int x;
	int y;
};

struct _hl_int3 {
	int x;
	int y;
	int z;
};

struct _hl_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _hl_double2 {
	double x;
	double y;
};

struct _hl_double3 {
	double x;
	double y;
	double z;
};

struct _hl_double4 {
	double x;
	double y;
	double z;
	double w;
};

inline void testvector(_hl_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include <hl.h>
#include "hl-blend2d.h"





extern "C" {

static BLFormat BLFormat__values[] = { BL_FORMAT_NONE,BL_FORMAT_PRGB32,BL_FORMAT_XRGB32,BL_FORMAT_A8,BL_FORMAT_MAX_VALUE };
HL_PRIM int HL_NAME(BLFormat_toValue0)( int idx ) {
	return BLFormat__values[idx];
}
DEFINE_PRIM(_I32, BLFormat_toValue0, _I32);
HL_PRIM int HL_NAME(BLFormat_indexToValue0)( int idx ) {
	return BLFormat__values[idx];
}
DEFINE_PRIM(_I32, BLFormat_indexToValue0, _I32);
HL_PRIM int HL_NAME(BLFormat_valueToIndex0)( int value ) {
	for( int i = 0; i < 5; i++ ) if ( value == (int)BLFormat__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, BLFormat_valueToIndex0, _I32);
static BLCompOp BLCompOp__values[] = { BL_COMP_OP_SRC_OVER,BL_COMP_OP_SRC_COPY,BL_COMP_OP_SRC_IN,BL_COMP_OP_SRC_OUT,BL_COMP_OP_SRC_ATOP,BL_COMP_OP_DST_OVER,BL_COMP_OP_DST_COPY,BL_COMP_OP_DST_IN,BL_COMP_OP_DST_OUT,BL_COMP_OP_DST_ATOP,BL_COMP_OP_XOR,BL_COMP_OP_CLEAR,BL_COMP_OP_PLUS,BL_COMP_OP_MINUS,BL_COMP_OP_MODULATE,BL_COMP_OP_MULTIPLY,BL_COMP_OP_SCREEN,BL_COMP_OP_OVERLAY,BL_COMP_OP_DARKEN,BL_COMP_OP_LIGHTEN,BL_COMP_OP_COLOR_DODGE,BL_COMP_OP_COLOR_BURN,BL_COMP_OP_LINEAR_BURN,BL_COMP_OP_LINEAR_LIGHT,BL_COMP_OP_PIN_LIGHT,BL_COMP_OP_HARD_LIGHT,BL_COMP_OP_SOFT_LIGHT,BL_COMP_OP_DIFFERENCE,BL_COMP_OP_EXCLUSION,BL_COMP_OP_MAX_VALUE };
HL_PRIM int HL_NAME(BLCompOp_toValue0)( int idx ) {
	return BLCompOp__values[idx];
}
DEFINE_PRIM(_I32, BLCompOp_toValue0, _I32);
HL_PRIM int HL_NAME(BLCompOp_indexToValue0)( int idx ) {
	return BLCompOp__values[idx];
}
DEFINE_PRIM(_I32, BLCompOp_indexToValue0, _I32);
HL_PRIM int HL_NAME(BLCompOp_valueToIndex0)( int value ) {
	for( int i = 0; i < 30; i++ ) if ( value == (int)BLCompOp__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, BLCompOp_valueToIndex0, _I32);
static void finalize_ImageCodec( _ref(BLImageCodec)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(ImageCodec_delete)( _ref(BLImageCodec)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, ImageCodec_delete, _IDL);
static void finalize_Image( _ref(BLImage)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Image_delete)( _ref(BLImage)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Image_delete, _IDL);
static void finalize_Triangle( _ref(BLTriangle)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Triangle_delete)( _ref(BLTriangle)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Triangle_delete, _IDL);
static void finalize_Context( _ref(BLContext)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Context_delete)( _ref(BLContext)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Context_delete, _IDL);
HL_PRIM _ref(BLImageCodec)* HL_NAME(ImageCodec_new0)() {
	return alloc_ref((new BLImageCodec()),ImageCodec);
}
DEFINE_PRIM(_IDL, ImageCodec_new0,);

HL_PRIM void HL_NAME(ImageCodec_findByName1)(_ref(BLImageCodec)* _this, vstring * name) {
	const char* name__cstr = (name == nullptr) ? "" : hl_to_utf8( name->bytes ); // Should be garbage collected
	(_unref(_this)->findByName(name__cstr));
}
DEFINE_PRIM(_VOID, ImageCodec_findByName1, _IDL _STRING);

HL_PRIM _ref(BLImage)* HL_NAME(Image_new3)(int width, int height, int format) {
	return alloc_ref((new BLImage(width, height, BLFormat__values[format])),Image);
}
DEFINE_PRIM(_IDL, Image_new3, _I32 _I32 _I32);

HL_PRIM void HL_NAME(Image_writeToFile2)(_ref(BLImage)* _this, vstring * path, _ref(BLImageCodec)* codec) {
	const char* path__cstr = (path == nullptr) ? "" : hl_to_utf8( path->bytes ); // Should be garbage collected
	(_unref(_this)->writeToFile(path__cstr, *_unref_ptr_safe(codec)));
}
DEFINE_PRIM(_VOID, Image_writeToFile2, _IDL _STRING _IDL);

HL_PRIM _ref(BLContext)* HL_NAME(Context_new0)() {
	return alloc_ref((new BLContext()),Context);
}
DEFINE_PRIM(_IDL, Context_new0,);

HL_PRIM void HL_NAME(Context_begin1)(_ref(BLContext)* _this, _ref(BLImage)* image) {
	(_unref(_this)->begin(*_unref_ptr_safe(image)));
}
DEFINE_PRIM(_VOID, Context_begin1, _IDL _IDL);

HL_PRIM void HL_NAME(Context_fillAll0)(_ref(BLContext)* _this) {
	(_unref(_this)->fillAll());
}
DEFINE_PRIM(_VOID, Context_fillAll0, _IDL);

HL_PRIM void HL_NAME(Context_setCompOp1)(_ref(BLContext)* _this, int compOp) {
	(_unref(_this)->setCompOp(BLCompOp__values[compOp]));
}
DEFINE_PRIM(_VOID, Context_setCompOp1, _IDL _I32);

HL_PRIM void HL_NAME(Context_setFillStyleColour4)(_ref(BLContext)* _this, int r, int g, int b, int a) {
	(blend2_context_setFillStyleColour( _unref(_this) , r, g, b, a));
}
DEFINE_PRIM(_VOID, Context_setFillStyleColour4, _IDL _I32 _I32 _I32 _I32);

HL_PRIM void HL_NAME(Context_setFillStyleColourPacked1)(_ref(BLContext)* _this, unsigned int rgba) {
	(blend2_context_setFillStyleColourPacked( _unref(_this) , rgba));
}
DEFINE_PRIM(_VOID, Context_setFillStyleColourPacked1, _IDL _I32);

HL_PRIM void HL_NAME(Context_fillTriangle1)(_ref(BLContext)* _this, _ref(BLTriangle)* triangle) {
	(_unref(_this)->fillTriangle(*_unref_ptr_safe(triangle)));
}
DEFINE_PRIM(_VOID, Context_fillTriangle1, _IDL _IDL);

HL_PRIM void HL_NAME(Context_end0)(_ref(BLContext)* _this) {
	(_unref(_this)->end());
}
DEFINE_PRIM(_VOID, Context_end0, _IDL);

}
