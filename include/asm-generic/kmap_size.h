/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_KMAP_SIZE_H
#define _ASM_GENERIC_KMAP_SIZE_H

/* For debug this provides guard pages between the maps */
#ifdef CONFIG_DEBUG_HIGHMEM
# define KM_MAX_IDX	33
#else
#if defined(CONFIG_ARCH_AXXIA) && defined(CONFIG_SMP)
#if (CONFIG_NR_CPUS > 15)
/* Prevent overlap between fixmap mapping and CPU vector page for 16th core */
#define KM_MAX_IDX 15
#else
# define KM_MAX_IDX	16
#endif
#else
#define KM_MAX_IDX 16
#endif
#endif

#endif
