/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Freescale Management Complex (MC) bus private declarations
 *
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 *
 */
#ifndef _FSL_MC_PRIVATE_H_
#define _FSL_MC_PRIVATE_H_

#include <linux/fsl/mc.h>
#include <linux/mutex.h>

/*
 * Data Path Management Complex (DPMNG) General API
 */

/* DPMNG command versioning */
#define DPMNG_CMD_BASE_VERSION		1
#define DPMNG_CMD_ID_OFFSET		4

#define DPMNG_CMD(id)	(((id) << DPMNG_CMD_ID_OFFSET) | DPMNG_CMD_BASE_VERSION)

/* DPMNG command IDs */
#define DPMNG_CMDID_GET_VERSION		DPMNG_CMD(0x831)

struct dpmng_rsp_get_version {
	__le32 revision;
	__le32 version_major;
	__le32 version_minor;
};

/*
 * Data Path Management Command Portal (DPMCP) API
 */

/* Minimal supported DPMCP Version */
#define DPMCP_MIN_VER_MAJOR		3
#define DPMCP_MIN_VER_MINOR		0

/* DPMCP command versioning */
#define DPMCP_CMD_BASE_VERSION		1
#define DPMCP_CMD_ID_OFFSET		4

#define DPMCP_CMD(id)	(((id) << DPMCP_CMD_ID_OFFSET) | DPMCP_CMD_BASE_VERSION)

/* DPMCP command IDs */
#define DPMCP_CMDID_CLOSE		DPMCP_CMD(0x800)
#define DPMCP_CMDID_OPEN		DPMCP_CMD(0x80b)
#define DPMCP_CMDID_RESET		DPMCP_CMD(0x005)

struct dpmcp_cmd_open {
	__le32 dpmcp_id;
};

/*
 * Initialization and runtime control APIs for DPMCP
 */
int dpmcp_open(struct fsl_mc_io *mc_io,
	       u32 cmd_flags,
	       int dpmcp_id,
	       u16 *token);

int dpmcp_close(struct fsl_mc_io *mc_io,
		u32 cmd_flags,
		u16 token);

int dpmcp_reset(struct fsl_mc_io *mc_io,
		u32 cmd_flags,
		u16 token);

/* Minimal supported DPRC Version */
#define DPRC_MIN_VER_MAJOR                      6
#define DPRC_MIN_VER_MINOR                      0

/* DPRC command versioning */
#define DPRC_CMD_BASE_VERSION                   1
#define DPRC_CMD_2ND_VERSION                    2
#define DPRC_CMD_3RD_VERSION                    3
#define DPRC_CMD_ID_OFFSET                      4

#define DPRC_CMD(id)    (((id) << DPRC_CMD_ID_OFFSET) | DPRC_CMD_BASE_VERSION)
#define DPRC_CMD_V2(id) (((id) << DPRC_CMD_ID_OFFSET) | DPRC_CMD_2ND_VERSION)
#define DPRC_CMD_V3(id) (((id) << DPRC_CMD_ID_OFFSET) | DPRC_CMD_3RD_VERSION)

/* DPRC command IDs */
#define DPRC_CMDID_CLOSE                        DPRC_CMD(0x800)
#define DPRC_CMDID_OPEN                         DPRC_CMD(0x805)
#define DPRC_CMDID_GET_API_VERSION              DPRC_CMD(0xa05)

#define DPRC_CMDID_GET_ATTR                     DPRC_CMD(0x004)
#define DPRC_CMDID_RESET_CONT                   DPRC_CMD(0x005)
#define DPRC_CMDID_RESET_CONT_V2                DPRC_CMD_V2(0x005)

#define DPRC_CMDID_SET_IRQ                      DPRC_CMD(0x010)
#define DPRC_CMDID_SET_IRQ_ENABLE               DPRC_CMD(0x012)
#define DPRC_CMDID_SET_IRQ_MASK                 DPRC_CMD(0x014)
#define DPRC_CMDID_GET_IRQ_STATUS               DPRC_CMD(0x016)
#define DPRC_CMDID_CLEAR_IRQ_STATUS             DPRC_CMD(0x017)

#define DPRC_CMDID_GET_CONT_ID                  DPRC_CMD(0x830)
#define DPRC_CMDID_GET_OBJ_COUNT                DPRC_CMD(0x159)
#define DPRC_CMDID_GET_OBJ                      DPRC_CMD(0x15A)
#define DPRC_CMDID_GET_OBJ_REG                  DPRC_CMD(0x15E)
#define DPRC_CMDID_GET_OBJ_REG_V2               DPRC_CMD_V2(0x15E)
#define DPRC_CMDID_GET_OBJ_REG_V3               DPRC_CMD_V3(0x15E)
#define DPRC_CMDID_SET_OBJ_IRQ                  DPRC_CMD(0x15F)

#define DPRC_CMDID_GET_CONNECTION               DPRC_CMD(0x16C)

struct dprc_cmd_get_connection {
        __le32 ep1_id;
        __le16 ep1_interface_id;
        u8 pad[2];
        u8 ep1_type[16];
};

struct dprc_rsp_get_connection {
        __le64 pad[3];
        __le32 ep2_id;
        __le16 ep2_interface_id;
        __le16 pad1;
        u8 ep2_type[16];
        __le32 state;
};

/**
 * struct dprc_endpoint - Endpoint description for link connect/disconnect
 *                      operations
 * @type:       Endpoint object type: NULL terminated string
 * @id:         Endpoint object ID
 * @if_id:      Interface ID; should be set for endpoints with multiple
 *              interfaces ("dpsw", "dpdmux"); for others, always set to 0
 */
struct dprc_endpoint {
        char type[16];
        int id;
        u16 if_id;
};

int dprc_get_connection(struct fsl_mc_io *mc_io,
                        u32 cmd_flags,
                        u16 token,
                        const struct dprc_endpoint *endpoint1,
                        struct dprc_endpoint *endpoint2,
                        int *state);

/*
 * Data Path Buffer Pool (DPBP) API
 */

/* DPBP Version */
#define DPBP_VER_MAJOR				3
#define DPBP_VER_MINOR				2

/* Command versioning */
#define DPBP_CMD_BASE_VERSION			1
#define DPBP_CMD_ID_OFFSET			4

#define DPBP_CMD(id)	(((id) << DPBP_CMD_ID_OFFSET) | DPBP_CMD_BASE_VERSION)

/* Command IDs */
#define DPBP_CMDID_CLOSE		DPBP_CMD(0x800)
#define DPBP_CMDID_OPEN			DPBP_CMD(0x804)

#define DPBP_CMDID_ENABLE		DPBP_CMD(0x002)
#define DPBP_CMDID_DISABLE		DPBP_CMD(0x003)
#define DPBP_CMDID_GET_ATTR		DPBP_CMD(0x004)
#define DPBP_CMDID_RESET		DPBP_CMD(0x005)

struct dpbp_cmd_open {
	__le32 dpbp_id;
};

#define DPBP_ENABLE			0x1

struct dpbp_rsp_get_attributes {
	/* response word 0 */
	__le16 pad;
	__le16 bpid;
	__le32 id;
	/* response word 1 */
	__le16 version_major;
	__le16 version_minor;
};

/*
 * Data Path Concentrator (DPCON) API
 */

/* DPCON Version */
#define DPCON_VER_MAJOR				3
#define DPCON_VER_MINOR				2

/* Command versioning */
#define DPCON_CMD_BASE_VERSION			1
#define DPCON_CMD_ID_OFFSET			4

#define DPCON_CMD(id)	(((id) << DPCON_CMD_ID_OFFSET) | DPCON_CMD_BASE_VERSION)

/* Command IDs */
#define DPCON_CMDID_CLOSE			DPCON_CMD(0x800)
#define DPCON_CMDID_OPEN			DPCON_CMD(0x808)

#define DPCON_CMDID_ENABLE			DPCON_CMD(0x002)
#define DPCON_CMDID_DISABLE			DPCON_CMD(0x003)
#define DPCON_CMDID_GET_ATTR			DPCON_CMD(0x004)
#define DPCON_CMDID_RESET			DPCON_CMD(0x005)

#define DPCON_CMDID_SET_NOTIFICATION		DPCON_CMD(0x100)

struct dpcon_cmd_open {
	__le32 dpcon_id;
};

#define DPCON_ENABLE			1

struct dpcon_rsp_get_attr {
	/* response word 0 */
	__le32 id;
	__le16 qbman_ch_id;
	u8 num_priorities;
	u8 pad;
};

struct dpcon_cmd_set_notification {
	/* cmd word 0 */
	__le32 dpio_id;
	u8 priority;
	u8 pad[3];
	/* cmd word 1 */
	__le64 user_ctx;
};

int __must_check fsl_mc_device_add(struct fsl_mc_obj_desc *obj_desc,
				   struct fsl_mc_io *mc_io,
				   struct device *parent_dev,
				   const char *driver_override,
				   struct fsl_mc_device **new_mc_dev);

int __init dprc_driver_init(void);

void dprc_driver_exit(void);

int __init fsl_mc_allocator_driver_init(void);

void fsl_mc_allocator_driver_exit(void);

int __must_check fsl_mc_resource_allocate(struct fsl_mc_bus *mc_bus,
					  enum fsl_mc_pool_type pool_type,
					  struct fsl_mc_resource
							  **new_resource);

void fsl_mc_resource_free(struct fsl_mc_resource *resource);

int fsl_mc_msi_domain_alloc_irqs(struct device *dev,
				 unsigned int irq_count);

void fsl_mc_msi_domain_free_irqs(struct device *dev);

bool fsl_mc_is_root_dprc(struct device *dev);

#ifdef CONFIG_FSL_MC_UAPI_SUPPORT

int fsl_mc_uapi_create_device_file(struct fsl_mc_bus *mc_bus);

void fsl_mc_uapi_remove_device_file(struct fsl_mc_bus *mc_bus);

#else

static inline int fsl_mc_uapi_create_device_file(struct fsl_mc_bus *mc_bus)
{
       return 0;
}

static inline void fsl_mc_uapi_remove_device_file(struct fsl_mc_bus *mc_bus)
{
}

#endif

void fsl_mc_get_root_dprc(struct device *dev,
			 struct device **root_dprc_dev);

struct fsl_mc_device *fsl_mc_device_lookup(struct fsl_mc_obj_desc *obj_desc,
					   struct fsl_mc_device *mc_bus_dev);


int disable_dprc_irq(struct fsl_mc_device *mc_dev);
int enable_dprc_irq(struct fsl_mc_device *mc_dev);
int get_dprc_irq_state(struct fsl_mc_device *mc_dev);

#endif /* _FSL_MC_PRIVATE_H_ */
