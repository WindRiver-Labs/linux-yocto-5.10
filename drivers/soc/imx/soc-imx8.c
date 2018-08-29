// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2019 NXP.
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/platform_device.h>
#include <linux/arm-smccc.h>
#include <linux/of.h>

#define REV_B1				0x21

#define IMX8MQ_SW_INFO_B1		0x40
#define IMX8MQ_SW_MAGIC_B1		0xff0055aa

#define IMX_SIP_GET_SOC_INFO           0xc2000006

#define IMX_SIP_NOC                    0xc2000008
#define IMX_SIP_NOC_LCDIF              0x0
#define IMX_SIP_NOC_PRIORITY           0x1
#define NOC_GPU_PRIORITY               0x10
#define NOC_DCSS_PRIORITY              0x11
#define NOC_VPU_PRIORITY               0x12
#define NOC_CPU_PRIORITY               0x13
#define NOC_MIX_PRIORITY               0x14

struct imx8_soc_data {
	char *name;
	u32 (*soc_revision)(void);
};

static u32 imx8mq_soc_revision_from_atf(void)
{
       struct arm_smccc_res res;

       arm_smccc_smc(IMX_SIP_GET_SOC_INFO, 0, 0, 0, 0, 0, 0, 0, &res);

       if (res.a0 == SMCCC_RET_NOT_SUPPORTED)
               return 0;
       else
               return res.a0 & 0xff;
}

static u32 __init imx8mq_soc_revision(void)
{
	struct device_node *np;
	void __iomem *ocotp_base;
	u32 magic;
	u32 rev = 0;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx8mq-ocotp");
	if (!np)
		goto out;

	ocotp_base = of_iomap(np, 0);
	WARN_ON(!ocotp_base);

	/*
        * SOC revision on older imx8mq is not available in fuses so query
        * the value from ATF instead.
        */
       rev = imx8mq_soc_revision_from_atf();
       if (!rev) {
               magic = readl_relaxed(ocotp_base + IMX8MQ_SW_INFO_B1);
               if (magic == IMX8MQ_SW_MAGIC_B1)
                       rev = REV_B1;
       }

	iounmap(ocotp_base);

out:
	of_node_put(np);
	return rev;
}

static const struct imx8_soc_data imx8mq_soc_data = {
	.name = "i.MX8MQ",
	.soc_revision = imx8mq_soc_revision,
};

static const struct imx8_soc_data imx8mp_soc_data = {
       .name = "i.MX8MP",
       .soc_revision = imx8mm_soc_revision,
};

static const struct of_device_id imx8_soc_match[] = {
	{ .compatible = "fsl,imx8mq", .data = &imx8mq_soc_data, },
	{ .compatible = "fsl,imx8mp", .data = &imx8mp_soc_data, },
	{ }
};

#define imx8_revision(soc_rev) \
	soc_rev ? \
	kasprintf(GFP_KERNEL, "%d.%d", (soc_rev >> 4) & 0xf,  soc_rev & 0xf) : \
	"unknown"

static void __init imx8mq_noc_init(void)
{
	struct arm_smccc_res res;

	pr_info("Config NOC for VPU and CPU\n");

	arm_smccc_smc(IMX_SIP_NOC, IMX_SIP_NOC_PRIORITY, NOC_CPU_PRIORITY,
			0x80000300, 0, 0, 0, 0, &res);
	if (res.a0)
		pr_err("Config NOC for CPU fail!\n");

	arm_smccc_smc(IMX_SIP_NOC, IMX_SIP_NOC_LCDIF, 0,
			0, 0, 0, 0, 0, &res);
	if (res.a0)
		pr_err("Config NOC for VPU fail!\n");
}

static int __init imx8_soc_init(void)
{
	struct soc_device_attribute *soc_dev_attr;
	struct soc_device *soc_dev;
	struct device_node *root;
	const struct of_device_id *id;
	u32 soc_rev = 0;
	const struct imx8_soc_data *data;
	int ret;

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return -ENODEV;

	soc_dev_attr->family = "Freescale i.MX";

	root = of_find_node_by_path("/");
	ret = of_property_read_string(root, "model", &soc_dev_attr->machine);
	if (ret)
		goto free_soc;

	id = of_match_node(imx8_soc_match, root);
	if (!id)
		goto free_soc;

	of_node_put(root);

	data = id->data;
	if (data) {
		soc_dev_attr->soc_id = data->name;
		if (data->soc_revision)
			soc_rev = data->soc_revision();
	}

	soc_dev_attr->revision = imx8_revision(soc_rev);
	if (!soc_dev_attr->revision)
		goto free_soc;

	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev))
		goto free_rev;

	if (of_machine_is_compatible("fsl,imx8mq"))
		imx8mq_noc_init();

	return 0;

free_rev:
	kfree(soc_dev_attr->revision);
free_soc:
	kfree(soc_dev_attr);
	of_node_put(root);
	return -ENODEV;
}
device_initcall(imx8_soc_init);
