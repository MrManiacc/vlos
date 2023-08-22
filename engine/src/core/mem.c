
#include "mem.h"
#include "platform/platform.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEM_TAG_FLAGGED];
};

static struct memory_stats stats;

static const char *memory_tag_strings[MEM_TAG_FLAGGED] = {
        "ARRAY      ",
        "DARRAY     ",
        "DICT       ",
        "RING_QUEUE ",
        "BST        ",
        "APPLICATION",
        "STRING     ",
        "JOB        ",
        "TEXTURE    ",
        "MAT_INST   ",
        "RENDERER   ",
        "TRANSFORM  ",
        "ENTITY     ",
        "ENTITY_NODE",
        "SCENE      ",
        "MATERIAL   ",
        "MESH       ",};

void mem_init() {
    stats.total_allocated = 0;
    platform_zero_memory(&stats, sizeof(stats));
}

ptr mem_alloc(u64 size, mem_tag tag) {
    if (tag == MEM_TAG_FLAGGED) {
        vwarn("mem_alloc called using MEM_TAG_FLAG. Re-class this allocation.", tag)
    }
    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;
    //TODO: mem alignment
    void *block = platform_allocate(size, false);
    platform_zero_memory(block, size);
    return block;
}

ptr mem_zero(void *pointer, u64 size) {
    return platform_zero_memory(pointer, size);
}

void mem_free(void *pointer, u64 size, mem_tag tag) {
    if (tag == MEM_TAG_FLAGGED) {
        vwarn("mem_free called using MEM_TAG_FLAG. Re-class this allocation.", tag)
    }
    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;
    //TODO: mem alignment
    platform_free(pointer, false);
}

ptr mem_copy(void *dest, constptr src, u64 size) {
    return platform_copy_memory(dest, src, size);
}

void mem_shutdown() {
    if (stats.total_allocated != 0) {
        verror("Memory leak detected. Total memory allocated: %llu", stats.total_allocated);
        for (u32 i = 0; i < MEM_TAG_FLAGGED; ++i) {
            if (stats.tagged_allocations[i] != 0) {
                verror("Memory leak detected for tag %u. Total memory allocated: %llu", i, stats.tagged_allocations[i]);
            }
        }
    }
}

void *mem_set(void *dest_ptr, i32 value, u64 size) {
    return platform_set_memory(dest_ptr, value, size);
}

str mem_usage_str() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEM_TAG_FLAGGED; ++i) {
        char unit[4] = "XiB";
        float amount = 1.0f;
        if (stats.tagged_allocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float) gib;
        } else if (stats.tagged_allocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float) mib;
        } else if (stats.tagged_allocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float) kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float) stats.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }
    char *out_string = _strdup(buffer);
    return out_string;
}