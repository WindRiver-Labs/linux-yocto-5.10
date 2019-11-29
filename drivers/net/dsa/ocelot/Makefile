# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_NET_DSA_MSCC_FELIX) += mscc_felix.o
obj-$(CONFIG_NET_DSA_MSCC_SEVILLE) += mscc_seville.o

mscc_felix-objs := \
	felix.o \
	felix_vsc9959.o

mscc_seville-objs := \
	felix.o \
	seville_vsc9953.o

obj-$(CONFIG_MSCC_FELIX_SWITCH_TSN) += felix_tsn.o
