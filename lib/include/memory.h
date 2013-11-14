/**
 * \file memory.h
 *  License details are found in the file LICENSE.
 * \brief
 *  Convert virtual address from/to physical address.
 * \author Taku Shimosawa  <shimosawa@is.s.u-tokyo.ac.jp> \par
 *      Copyright (C) 2011 - 2012  Taku Shimosawa
 */

#ifndef __HEADER_GENERIC_MEMORY_H
#define __HEADER_GENERIC_MEMORY_H

#include <arch-memory.h>

#ifndef KERNEL_PHYS_OFFSET
#define KERNEL_PHYS_OFFSET 0

static unsigned long virt_to_phys(void *v)
{
	return (unsigned long)v - KERNEL_PHYS_OFFSET;
}
static void *phys_to_virt(unsigned long p)
{
	return (void *)(p + KERNEL_PHYS_OFFSET);
}
#endif

unsigned long virt_to_phys(void *v);
void *phys_to_virt(unsigned long p);

#endif

