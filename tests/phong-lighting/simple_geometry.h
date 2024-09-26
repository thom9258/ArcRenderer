#ifndef _SIMPLE_GEOMETRY_H_
#define _SIMPLE_GEOMETRY_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #define SG_API_EXPORT __declspec(dllexport)
#else
  #define SG_API_EXPORT
#endif
	
#ifndef	SG_NO_ASSERT
#  include <assert.h>
#  ifndef SG_ASSERT_NOT_IMPLEMENTED
#    define SG_ASSERT_NOT_IMPLEMENTED(NAMESTR) \
	assert(1 && "FUNCTION [" NAMESTR "] HAS NOT BEEN IMPLEMENTED YET!")
#  endif
#endif

#ifndef	SG_UNREFERENCED
  #define SG_UNREFERENCED(VAR) (void)(VAR)
#endif
	
#ifndef	SG_IN
  #define SG_IN
#endif
#ifndef	SG_OUT
  #define SG_OUT
#endif
#ifndef	SG_INOUT
  #define SG_INOUT
#endif
	
#ifndef	SG_nullptr
  #define SG_nullptr (void*)0
#endif

#ifndef	SG_true
  #define SG_true 1
#endif

#ifndef	SG_false
  #define SG_false 0 
#endif

#ifndef	SG_float
  #define SG_float float
#endif

#ifndef	SG_bool
  #define SG_bool char
#endif
	
#ifndef	SG_size
  #define SG_size unsigned int
#endif
	
#ifndef	SG_indice
  #define SG_indice unsigned int
#endif
	
#define	SG_ARRAY_LEN(ARR) (sizeof(ARR) / sizeof(*(ARR)))
#define	SG_ARRAY_MEMSIZE(ARR) sizeof(ARR)
	
enum SG_status {
	SG_OK_RETURNED_BUFFER,
	SG_OK_RETURNED_LEN,
	SG_OK_COPIED_TO_DST,

	SG_ERR_NULLPTR_INPUT,
	SG_ERR_ZEROSIZE_INPUT,
	SG_ERR_DSTLEN_NOT_PROVIDED,
	SG_ERR_SRCBLKSIZE_LESSTHAN_SRCSTRIDE,
	SG_ERR_SRCBLKSIZE_LESSTHAN_DSTSTRIDE,

	SG_ERR_NOT_IMPLEMENTED_YET,
};

struct sg_position {
	SG_float x;
	SG_float y;
	SG_float z;
};
	
struct sg_texcoord {
	SG_float u;
	SG_float v;
};

struct sg_normal {
	SG_float x;
	SG_float y;
	SG_float z;
};
	
/**
 * @brief Check if returned SG_STATUS indicates success.
 *
 * A simplified summary of the return code, if ignoring any
 * specific error/success code is wanted.
 */
SG_API_EXPORT
SG_bool
sg_success(SG_IN const enum SG_status status); 
	
SG_API_EXPORT
const char*
sg_status_string(SG_IN const enum SG_status status);
	
/**
 * @brief Copy memory.
 *
 * Copies 'n' bytes of memory from 'src' to 'dst'.
 */
SG_API_EXPORT
enum SG_status
sg_memcpy(SG_IN const void* src,
		  SG_IN const SG_size n,
		  SG_OUT void* dst);

/**
 * @brief Copy strided blocks of memory.
 *
 * Copies 'src_block_count' blocks from 'src' of 'src_block_size'
 * with 'src_stride' to specified 'dst' with specified 'dst_stride'.
 *
 * This function allows interlacing of different buffers into a single,
 * coherent buffer, converting multiple Structure of Arrays (SoA)'s into a
 * single Array of Structures (AoS) buffer.
 *
 * @note if 'src_stride' is zero, this function can be used 
 *       as a block duplication mechanism.
 * 
 * @example Observe a 'SoA' pos buffer & its texcoord buffer:
 *  xyzType XYZ[] = [xyz1][xyz2][xyz3]
 *  uvType UV[] = [uv1][uv2][uv3]
 *
 * interlacing using strided blockcopying could create an 'AoS' buffer:
 *   [xyz1][uv1][xyz2][uv2][xyz3][uv3]
 *
 * The function signature is complex, but in order to create the 'AoS'
 * buffer above, the process would be:
 *
 *   NewBuffer[] = ...
 *
 * A suitable buffer needs to be prepared, with enough memory to fit
 * all the interlazed buffers.
 * Then, we start by copying the first buffer:
 *
 *   sg_strided_blockcopy(sizeof(xyzType),
 *                        sizeof(xyzType),
 *                        3,
 *                        XYZ,
 *                        sizeof(xyzType) + sizeof(uvType),
 *                        NewBuffer);
 *
 * Here it should be noted that the source buffer has its data packed
 * linearly, so the source stride is just the size of the individual elements.
 * Similarly, in the new buffer we would need to fit a element from each
 * buffer alongsize each-other, so the destination stride is the cumulative
 * size of each element.
 *
 * Now we copy over the second buffer:
 *
 *   sg_strided_blockcopy(sizeof(uvType),
 *                        sizeof(uvType),
 *                        3,
 *                        UV,
 *                        sizeof(xyzType) + sizeof(uvType),
 *                        NewBuffer + sizeof(xyzType));
 *
 * Here it is imperative to note that the destination pointer needs to be
 * offset with the size of the other buffer elements already placed, so
 * that we do not overwrite them.
 * Other than this, the inputs follow a similar structure as all the
 * previous strided copying operations. 
 */
SG_API_EXPORT
enum SG_status
sg_strided_blockcopy(SG_IN  const SG_size src_block_size,
					 SG_IN  const SG_size src_stride,
					 SG_IN  const SG_size src_block_count,
					 SG_IN  const void* src,
					 SG_IN  const SG_size dst_stride,
					 SG_OUT void* dst);
	
/**
 * @brief Calculate flat normals for all vertices.
 */
SG_API_EXPORT
enum SG_status
sg_calculate_flat_normals(SG_IN  const struct sg_position* vertices,
						  SG_IN  const SG_indice* indices,
						  SG_OUT struct sg_normal* normals);

/**
 * @brief Get Plane Position Vertex.
 *
 * Call Signature 1:
 * To get the length of the required memory buffer, call this
 * function with 'dst' set to SG_nullptr.
 * When this is done, specified 'width' & 'height' is unused.

 * Call Signature 2:
 * To get the vertices, provide a buffer of *atleast* the
 * returned length as gotten when using call signature 1.
 * the provided 'width' & 'height' is used to calculate the
 * size of the returned plane.
 *
 * @return a return code indicating the status of the use.
 */
SG_API_EXPORT
enum SG_status
sg_plane_positions(SG_IN  const SG_float width,
				   SG_IN  const SG_float height,
				   SG_OUT struct sg_position* dst,
				   SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_plane_indices(SG_OUT SG_indice* dst,
				 SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_plane_texcoords(SG_OUT struct sg_texcoord* dst,
				   SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_indexed_cube_positions(SG_IN  const SG_float width,
						  SG_IN  const SG_float height,
						  SG_IN  const SG_float depth,
						  SG_OUT struct sg_position* dst,
						  SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_indexed_cube_indices(SG_OUT SG_indice* dst,
						SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_indexed_cube_texcoords(SG_OUT struct sg_texcoord* dst,
						  SG_OUT SG_size* dstlen);
	
SG_API_EXPORT
enum SG_status
sg_cube_positions(SG_IN  const SG_float width,
				  SG_IN  const SG_float height,
				  SG_IN  const SG_float depth,
				  SG_OUT struct sg_position* dst,
				  SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_cube_normals(SG_OUT struct sg_normal* dst,
				SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_sphere_positions(SG_IN  const SG_float width,
					SG_IN  const SG_float height,
					SG_IN  const SG_float depth,
					SG_OUT struct sg_position* dst,
					SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_sphere_indices(SG_OUT SG_indice* dst,
				  SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_sphere_texcoords(SG_OUT struct sg_texcoord* dst,
					SG_OUT SG_size* dstlen);

SG_API_EXPORT
enum SG_status
sg_sphere_normals(SG_OUT struct sg_normal* dst,
				  SG_OUT SG_size* dstlen);


/** ***********************************************************
 * Implementation
 */
#ifdef SIMPLE_GEOMETRY_IMPLEMENTATION

SG_bool
sg_success(SG_IN const enum SG_status status)
{
	switch (status) {
	case SG_OK_RETURNED_BUFFER:
	case SG_OK_RETURNED_LEN:
	case SG_OK_COPIED_TO_DST:
		return SG_true;
	default:
		return SG_false;
	};
}
	
const char*
sg_status_string(SG_IN const enum SG_status status)
{
	switch (status) {
	case SG_OK_RETURNED_BUFFER:                return "SG_OK_RETURNED_BUFFER"; 
	case SG_OK_RETURNED_LEN:                   return "SG_OK_RETURNED_LEN";
	case SG_OK_COPIED_TO_DST:                  return "SG_OK_COPIED_TO_DST";
	case SG_ERR_NULLPTR_INPUT:                 return "SG_ERR_NULLPTR_INPUT";
	case SG_ERR_ZEROSIZE_INPUT:                return "SG_ERR_ZEROSIZE_INPUT";
	case SG_ERR_DSTLEN_NOT_PROVIDED:           return "SG_ERR_DSTLEN_NOT_PROVIDED";
	case SG_ERR_SRCBLKSIZE_LESSTHAN_SRCSTRIDE: return "SG_ERR_SRCBLKSIZE_LESSTHAN_SRCSTRIDE";
	case SG_ERR_SRCBLKSIZE_LESSTHAN_DSTSTRIDE: return "SG_ERR_SRCBLKSIZE_LESSTHAN_DSTSTRIDE";
	case SG_ERR_NOT_IMPLEMENTED_YET:           return "SG_ERR_NOT_IMPLEMENTED_YET";
	};
	return "SG_UNKNOWN_STATUS";
}

enum SG_status
sg_memcpy(SG_IN  const void* src,
		  SG_IN  const SG_size n,
		  SG_OUT void* dst)
{
	SG_size i;
	if (src == SG_nullptr || dst == SG_nullptr)
		return SG_ERR_NULLPTR_INPUT;
	if (n < 1)
		return SG_ERR_ZEROSIZE_INPUT;

	for (i = 0; i < n; i++)
		((char*)dst)[i] = ((char*)src)[i];
	return SG_OK_COPIED_TO_DST;
}

enum SG_status
sg_strided_blockcopy(SG_IN  const SG_size src_block_size,
					 SG_IN  const SG_size src_stride,
					 SG_IN  const SG_size src_block_count,
					 SG_IN  const void* src,
					 SG_IN  const SG_size dst_stride,
					 SG_OUT void* dst)
{
	SG_size i;
	char* currsrc;
	char* currdst;
	enum SG_status copystatus;
	if (src == SG_nullptr || dst == SG_nullptr)
		return SG_ERR_NULLPTR_INPUT;
	if (dst_stride < 1 || src_block_size < 1 || src_block_count < 1)
		return SG_ERR_ZEROSIZE_INPUT;
	if (dst_stride < src_block_size)
		return SG_ERR_SRCBLKSIZE_LESSTHAN_DSTSTRIDE;
	
	for (i = 0; i < src_block_count; i++) {
		currsrc = ((char*)src) + (i * src_stride);
		currdst = ((char*)dst) + (i * dst_stride);
		copystatus = sg_memcpy(currsrc, src_block_size, currdst);
		if (!sg_success(copystatus))
			return copystatus;
	}

	return SG_OK_COPIED_TO_DST;
}

enum SG_status
sg_calculate_flat_normals(SG_IN  const struct sg_position* vertices,
						  SG_IN  const SG_indice* indices,
						  SG_OUT struct sg_normal* normals)
{
	SG_ASSERT_NOT_IMPLEMENTED("sg_calculate_flat_normals");
	SG_UNREFERENCED(vertices);
	SG_UNREFERENCED(indices);
	SG_UNREFERENCED(normals);
	return SG_ERR_NOT_IMPLEMENTED_YET;
}
	
enum SG_status
sg_plane_positions(SG_IN  const SG_float width,
				   SG_IN  const SG_float height,
				   SG_OUT struct sg_position* dst,
				   SG_OUT SG_size* dstlen)
{
	const struct sg_position positions[] = {
        {-width, -height, 0.0f},
        { width, -height, 0.0f},
        { width,  height, 0.0f},
        {-width,  height, 0.0f}
    };

	if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = SG_ARRAY_LEN(positions);
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	return sg_memcpy(positions,
					 SG_ARRAY_MEMSIZE(positions),
					 dst);
}
	
enum SG_status
sg_plane_indices(SG_OUT SG_indice* dst,
				 SG_OUT SG_size* dstlen)
{
	const SG_indice indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = SG_ARRAY_LEN(indices);
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	return sg_memcpy(indices,
					 SG_ARRAY_MEMSIZE(indices),
					 dst);
}
	
enum SG_status
sg_plane_texcoords(SG_OUT struct sg_texcoord* dst,
				   SG_OUT SG_size* dstlen)
{
	const struct sg_texcoord out[] = {
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};
	const SG_size outlen = SG_ARRAY_LEN(out);
	SG_size i;
		if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = outlen;
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	for (i = 0; i < outlen; ++i) {
		dst[i] = out[i];
	}

	return SG_OK_RETURNED_BUFFER;
}

enum SG_status
sg_indexed_cube_positions(SG_IN  const SG_float width,
						  SG_IN  const SG_float height,
						  SG_IN  const SG_float depth,
						  SG_OUT struct sg_position* dst,
						  SG_OUT SG_size* dstlen)
{
	const struct sg_position positions[] = {
		{-width, -height, -depth},  // A 0
		{ width, -height, -depth},   // B 1
		{ width,  height, -depth},   // C 2
		{-width,  height, -depth},  // D 3
		{-width, -height,  depth},  // E 4
		{ width, -height,  depth},   // F 5
		{ width,  height,  depth},   // G 6
		{-width,  height,  depth},  // H 7

		{-width,  height, -depth},  // D 8
		{-width, -height, -depth},  // A 9
		{-width, -height,  depth},  // E 10
		{-width,  height,  depth},  // H 11
		{ width, -height, -depth},   // B 12
		{ width,  height, -depth},   // C 13
		{ width,  height,  depth},   // G 14
		{ width, -height,  depth},   // F 15

		{-width, -height, -depth},  // A 16
		{ width, -height, -depth},   // B 17
		{ width, -height,  depth},   // F 18
		{-width, -height,  depth},  // E 19
		{ width,  height, -depth},   // C 20
		{-width,  height, -depth},  // D 21
		{-width,  height,  depth},  // H 22
		{ width,  height,  depth},   // G 23
    };

	if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = SG_ARRAY_LEN(positions);
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	return sg_memcpy(positions,
					 SG_ARRAY_MEMSIZE(positions),
					 dst);
}
	
enum SG_status
sg_indexed_cube_indices(SG_OUT SG_indice* dst,
						SG_OUT SG_size* dstlen)
{
	const SG_indice out[] = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7 ,4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
	};
	const SG_size outlen = SG_ARRAY_LEN(out);
	SG_size i;
		if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = outlen;
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	for (i = 0; i < outlen; ++i) {
		dst[i] = out[i];
	}

	return SG_OK_RETURNED_BUFFER;
}
	
enum SG_status
sg_indexed_cube_texcoords(SG_OUT struct sg_texcoord* dst,
						  SG_OUT SG_size* dstlen)
{
	const struct sg_texcoord out[] = {
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},    
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f}
	};
	const SG_size outlen = SG_ARRAY_LEN(out);
	SG_size i;
		if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = outlen;
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	for (i = 0; i < outlen; ++i) {
		dst[i] = out[i];
	}

	return SG_OK_RETURNED_BUFFER;
}

enum SG_status
sg_cube_positions(SG_IN  const SG_float width,
				  SG_IN  const SG_float height,
				  SG_IN  const SG_float depth,
				  SG_OUT struct sg_position* dst,
				  SG_OUT SG_size* dstlen)
{
	const struct sg_position positions[] = {
		{-width, -height, -depth},
		{ width, -height, -depth},
		{ width,  height, -depth},
		{ width,  height, -depth},
		{-width,  height, -depth},
		{-width, -height, -depth},

		{-width, -height,  depth},
		{ width, -height,  depth},
		{ width,  height,  depth},
		{ width,  height,  depth},
		{-width,  height,  depth},
		{-width, -height,  depth},

		{-width,  height,  depth},
		{-width,  height, -depth},
		{-width, -height, -depth},
		{-width, -height, -depth},
		{-width, -height,  depth},
		{-width,  height,  depth},

		{ width,  height,  depth},
		{ width,  height, -depth},
		{ width, -height, -depth},
		{ width, -height, -depth},
		{ width, -height,  depth},
		{ width,  height,  depth},

		{-width, -height, -depth},
		{ width, -height, -depth},
		{ width, -height,  depth},
		{ width, -height,  depth},
		{-width, -height,  depth},
		{-width, -height, -depth},

		{-width,  height, -depth},
		{ width,  height, -depth},
		{ width,  height,  depth},
		{ width,  height,  depth},
		{-width,  height,  depth},
		{-width,  height, -depth},
	};
	SG_size i;
		if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = SG_ARRAY_LEN(positions);
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	for (i = 0; i < SG_ARRAY_LEN(positions); ++i)
		dst[i] = positions[i];

	return SG_OK_RETURNED_BUFFER;
}
	
enum SG_status
sg_cube_normals(SG_OUT struct sg_normal* dst,
				SG_OUT SG_size* dstlen)
{
	const struct sg_normal out[] = {
		{0.0f,  0.0f, -1.0f},
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 

		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},

		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},

		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},

		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},

		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f}
	};
	const SG_size outlen = SG_ARRAY_LEN(out);
	SG_size i;
		if (dstlen == SG_nullptr)
		return SG_ERR_DSTLEN_NOT_PROVIDED;

	*dstlen = outlen;
	if (dst == SG_nullptr)
		return SG_OK_RETURNED_LEN;
	
	for (i = 0; i < outlen; ++i) {
		dst[i] = out[i];
	}

	return SG_OK_RETURNED_BUFFER;
}


enum SG_status
sg_sphere_positions(SG_IN  const SG_float width,
					SG_IN  const SG_float height,
					SG_IN  const SG_float depth,
					SG_OUT struct sg_position* dst,
					SG_OUT SG_size* dstlen)
{
	SG_ASSERT_NOT_IMPLEMENTED("sg_sphere_positions");
	SG_UNREFERENCED(width);
	SG_UNREFERENCED(height);
	SG_UNREFERENCED(depth);
	SG_UNREFERENCED(dst);
	SG_UNREFERENCED(dstlen);
	return SG_ERR_NOT_IMPLEMENTED_YET;
}

enum SG_status
sg_sphere_indices(SG_OUT SG_indice* dst,
				  SG_OUT SG_size* dstlen)
{
	SG_ASSERT_NOT_IMPLEMENTED("sg_sphere_indices");
	SG_UNREFERENCED(dst);
	SG_UNREFERENCED(dstlen);
	return SG_ERR_NOT_IMPLEMENTED_YET;
}

enum SG_status
sg_sphere_texcoords(SG_OUT struct sg_texcoord* dst,
					SG_OUT SG_size* dstlen)
{
	SG_ASSERT_NOT_IMPLEMENTED("sg_sphere_texcoords");
	SG_UNREFERENCED(dst);
	SG_UNREFERENCED(dstlen);
	return SG_ERR_NOT_IMPLEMENTED_YET;
}

enum SG_status
sg_sphere_normals(SG_OUT struct sg_normal* dst,
				  SG_OUT SG_size* dstlen)
{
	SG_ASSERT_NOT_IMPLEMENTED("sg_sphere_normals");
	SG_UNREFERENCED(dst);
	SG_UNREFERENCED(dstlen);
	return SG_ERR_NOT_IMPLEMENTED_YET;
}


	
#endif // SIMPLE_GEOMETRY_IMPLEMENTATION
#ifdef __cplusplus
}
#endif
#endif //_SIMPLE_GEOMETRY_H_

