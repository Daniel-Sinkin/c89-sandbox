/* app/main.c */
#include "../src/binary_io.h"
#include "../src/development_markers.h" /* IWYU pragma: keep */
#include "../src/types.h"
/**/
#include <assert.h>
#include <limits.h>
#include <math.h> /* IWYU pragma: keep */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h> /* IWYU pragma: keep */
#include <string.h> /* IWYU pragma: keep */

/* static const char k_dans_magic[4] = {'d', 'a', 'n', 's'}; */

/* clang-format off */
#define DANS_PANIC(msg) do { \
    fprintf(stderr, "[PANIC, file=%s, line=%d] %s\n", __FILE__, __LINE__, msg); \
    abort(); \
} while (0)
#define DANS_FOPEN_WRITE_BINARY(file, fp) do { \
    file = fopen(fp, "wb"); \
    if (file == NULL) DANS_PANIC("Failed to open" fp);\
} while(0)
#define DANS_FOPEN_READ_BINARY(file, fp) do { \
    file = fopen(fp, "rb"); \
    if (file == NULL) DANS_PANIC("Failed to open" fp);\
} while(0)

/* Little Endian */
#define DANS_MAGIC_NUMBER ((dans_u32)((dans_u32)'d' | ((dans_u32)'a' << 8) | ((dans_u32)'n' << 16) | ((dans_u32)'s' << 24)))
/* clang-format on */

#define DANS_BINARY_FILE "sample.dans"

typedef struct dans_fatptr_u8
{
    dans_usize num_elems;
    dans_u8* data;
} dans_fatptr_u8;
#define DANS_FATPTR_U8_SIZEOF(ptr) ((ptr).num_elems * sizeof(dans_u8))

int dans_fatptr_u8_init(mut dans_fatptr_u8* self, dans_usize num_elems)
{
    mut void* data = NULL;

    if (self->data != NULL) DANS_PANIC("Initialised Pointer");

    if (num_elems == 0)
    {
        self->num_elems = 0;
        self->data = NULL;
        return 0;
    }

    data = malloc(num_elems * sizeof(dans_u8));
    if (data == NULL)
    {
        return 1;
    }
    self->num_elems = num_elems;
    self->data = data;
    return 0;
}

void dans_fatptr_u8_free(mut dans_fatptr_u8* self)
{
    if (self->data == NULL) return;
    assert(self->num_elems > 0);

    self->num_elems = 0;
    free(self->data);
}

void dans_fatptr_u8_fill(mut dans_fatptr_u8* self, dans_u8 value)
{
    mut dans_usize i;

    if (self->data == NULL) DANS_PANIC("Uninitialised Pointer");
    assert(self->num_elems > 0);

    for (i = 0; i < self->num_elems; ++i)
    {
        self->data[i] = value;
    }
}
void dans_fatptr_u8_print_metadata(const dans_fatptr_u8* self)
{
    printf("num_elems=%zu\n", self->num_elems);
}
void dans_fatptr_u8_print(const dans_fatptr_u8* self)
{
    mut dans_usize i;

    if (self->data == NULL) DANS_PANIC("Uninitialised Pointer");

    printf("[");
    for (i = 0; i < self->num_elems; ++i)
    {
        printf("%d", self->data[i]);
    }
    printf("]\n");
}
void dans_fatptr_u8_print_range(const dans_fatptr_u8* ptr, dans_usize left, dans_usize right)
{
    mut dans_usize i;

    if (right > ptr->num_elems) DANS_PANIC("OOB");
    if (right < left) DANS_PANIC("right < left in print_range");

    for (i = left; i < right; ++i)
    {
        printf("buffer[%zu] = %d\n", i, ptr->data[i]);
    }
}

void dans_fatptr_u8_write(mut dans_fatptr_u8* ptr, dans_usize idx, dans_u8 value)
{
    if (idx > ptr->num_elems) DANS_PANIC("OOB on print_range");

    ptr->data[idx] = value;
}

#define DANS_BUFFER_SIZE 1024

DANS_INLINE dans_usize dans_write_bytes_u8(mut dans_u8* out, dans_u8 x)
{
    out[0] = x;
    return sizeof(dans_u8);
}
DANS_INLINE dans_usize dans_write_bytes_u16(mut dans_u8* out, dans_u16 x)
{
    out[0] = x & 0xFF;
    out[1] = (x >> 8) & 0xFF;
    return sizeof(dans_u16);
}
DANS_INLINE dans_usize dans_write_bytes_u32(mut dans_u8* out, dans_u32 x)
{
    out[0] = x & 0xFF;
    out[1] = (x >> 8) & 0xFF;
    out[2] = (x >> 16) & 0xFF;
    out[3] = (x >> 24) & 0xFF;
    return sizeof(dans_u32);
}
DANS_INLINE dans_usize dans_write_bytes_u64(mut dans_u8* out, dans_u64 x)
{
    out[0] = (x >> (8 * 0)) & 0xFF;
    out[1] = (x >> (8 * 1)) & 0xFF;
    out[2] = (x >> (8 * 2)) & 0xFF;
    out[3] = (x >> (8 * 3)) & 0xFF;
    out[4] = (x >> (8 * 4)) & 0xFF;
    out[5] = (x >> (8 * 5)) & 0xFF;
    out[6] = (x >> (8 * 6)) & 0xFF;
    out[7] = (x >> (8 * 7)) & 0xFF;
    return sizeof(dans_u64);
}
DANS_INLINE dans_usize dans_write_bytes_usize(mut dans_u8* out, dans_usize x)
{
#if defined(DANS_USIZE_IS_64)
    return dans_write_bytes_u64(out, (dans_u64) x);
#else
    return dans_write_bytes_u32(out, (dans_u32) x);
#endif
}

DANS_INLINE dans_usize dans_write_bytes_i8(mut dans_u8* out, dans_i8 x)
{
    out[0] = (dans_u8) x;
    return sizeof(dans_u8);
}
DANS_INLINE dans_usize dans_write_bytes_i16(mut dans_u8* out, dans_i16 x)
{
    assert(out != NULL);
    return dans_write_bytes_u16(out, (dans_u16) x);
}
DANS_INLINE dans_usize dans_write_bytes_i32(mut dans_u8* out, dans_i32 x)
{
    assert(out != NULL);
    return dans_write_bytes_u32(out, (dans_u32) x);
}
DANS_INLINE dans_usize dans_write_bytes_i64(mut dans_u8* out, dans_i64 x)
{
    assert(out != NULL);
    return dans_write_bytes_u64(out, (dans_u64) x);
}

DANS_INLINE dans_usize dans_write_bytes_int(mut dans_u8* out, int x)
{
    DANS_STATIC_ASSERT(sizeof(x) == 4, int_4byte);
    assert(out != NULL);
    return dans_write_bytes_i32(out, (dans_u32) x);
}

DANS_INLINE dans_usize dans_write_bytes_f32(mut dans_u8* out, dans_f32 x)
{
    dans_u32 ux;
    assert(out != NULL);
    memcpy(&ux, &x, sizeof(dans_f32));
    return dans_write_bytes_u32(out, ux);
}
DANS_INLINE dans_usize dans_write_bytes_f64(mut dans_u8* out, dans_f64 x)
{
    dans_u64 ux;
    assert(out != NULL);
    if (out == NULL) DANS_PANIC("out Nullptr");
    memcpy(&ux, &x, sizeof(dans_f64));
    return dans_write_bytes_u64(out, ux);
}

DANS_INLINE dans_usize dans_write_bytes_fatptr_u8(mut dans_u8* out, const dans_fatptr_u8* ptr)
{
    assert(out != NULL);
    if (ptr == NULL) DANS_PANIC("Nullptr");
    if (ptr->data == NULL) DANS_PANIC("Ptr not Initialised");
    assert(ptr->num_elems > 0);

    memcpy(out, ptr->data, ptr->num_elems);

    return DANS_FATPTR_U8_SIZEOF(*ptr);
}
DANS_INLINE dans_usize
dans_write_bytes_ptr_u8(mut dans_u8* out, const dans_u8* ptr, dans_usize num_elems)
{
    assert(out != NULL);
    if (ptr == NULL) DANS_PANIC("ptr Nullptr");

    memcpy(out, ptr, num_elems);

    return num_elems * sizeof(dans_u8);
}
typedef struct dans_f32x3
{
    dans_f32 a;
    dans_f32 b;
    dans_f32 c;
} dans_f32x3;
DANS_INLINE dans_usize dans_write_bytes_f32x3(mut dans_u8* out, dans_f32x3 value)
{
    dans_usize bytes_written = 0;
    bytes_written += dans_write_bytes_f32(out, value.a);
    bytes_written += dans_write_bytes_f32(out + bytes_written, value.b);
    bytes_written += dans_write_bytes_f32(out + bytes_written, value.c);
    assert(bytes_written == 3 * sizeof(dans_f32));
    return bytes_written;
}
typedef struct dans_f32x16
{
    dans_f32 vals[16];
} dans_f32x16;
DANS_INLINE dans_usize dans_write_bytes_f32x16(mut dans_u8* out, const dans_f32x16* value)
{
    dans_usize i;
    dans_usize bytes_written = 0;

    for (i = 0; i < 16; ++i)
    {
        bytes_written += dans_write_bytes_f32(out + bytes_written, value->vals[i]);
    }
    assert(bytes_written == 16 * sizeof(dans_f32));
    return bytes_written;
}

typedef struct dans_view_u8
{
    dans_usize num_elems;
    dans_u8* data; /* Does not own */
} dans_view_u8;

DANS_INLINE dans_usize dans_write_bytes_view_u8(mut dans_u8* out, dans_view_u8 slice)
{
    dans_usize bytes_written;

    assert(out != NULL);
    if (slice.data == NULL) DANS_PANIC("Slice not initialised (should never happen)");

    bytes_written = dans_write_bytes_usize(out, slice.num_elems);
    bytes_written += dans_write_bytes_ptr_u8(out + bytes_written, slice.data, slice.num_elems);
    return bytes_written;
}

typedef enum
{
    DANS_VALUE_I64,
    DANS_VALUE_F64,
    DANS_VALUE_VIEW
} dans_value_tag;
typedef union
{
    dans_i64 integeral;
    dans_f64 floating;
    dans_view_u8 view;
} dans_value_data;

typedef struct dans_tagged_value
{
    dans_value_tag tag;
    dans_value_data data;
} dans_tagged_value;

DANS_INLINE dans_usize write_bytes_tagged_value(mut dans_u8* out, dans_tagged_value value)
{
    dans_usize bytes_written;
    if (value.tag == DANS_VALUE_I64)
    {
        bytes_written = dans_write_bytes_int(out, value.tag);
        bytes_written += dans_write_bytes_i64(out, value.data.integeral);
    }
    else if (value.tag == DANS_VALUE_F64)
    {
        bytes_written = dans_write_bytes_int(out, value.tag);
        bytes_written += dans_write_bytes_f64(out, value.data.floating);
    }
    else if (value.tag == DANS_VALUE_VIEW)
    {
        bytes_written = dans_write_bytes_int(out, value.tag);
        bytes_written += dans_write_bytes_view_u8(out, value.data.view);
    }
    else
    {
        DANS_PANIC("ENUM_END");
    }
    return bytes_written;
}

int main(void)
{
    mut dans_usize bytes_written = 0;
    {
        mut dans_fatptr_u8 buffer = {0};

        dans_fatptr_u8_init(&buffer, DANS_BUFFER_SIZE);
        printf("%u", DANS_MAGIC_NUMBER);
        bytes_written += dans_write_bytes_u32(buffer.data, DANS_MAGIC_NUMBER);

        { /* Write file */
            mut FILE* file;
            DANS_FOPEN_WRITE_BINARY(file, DANS_BINARY_FILE);
            fwrite(buffer.data, sizeof(*buffer.data), bytes_written, file);
            fclose(file);
        }
        dans_fatptr_u8_free(&buffer);
    }
    return EXIT_SUCCESS;
}
