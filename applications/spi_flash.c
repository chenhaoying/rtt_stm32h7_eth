/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-09     chenhy       the first version
 */

#include "board.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_spi.h"
#include "spi_flash_sfud.h"
#include "fal.h"
#include "dfs_fs.h"

#define DBG_TAG "spi_flash"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define FS_PARTITION_NAME  "files"
int spi_flash_mount(void)
{
    rt_hw_spi_device_attach("spi1", "spi10", GPIOD, GPIO_PIN_6);
    rt_sfud_flash_probe(FAL_USING_NOR_FLASH_DEV_NAME, "spi10");

    fal_init();
    struct rt_device *flash_dev = fal_blk_device_create("files");
    if (flash_dev == NULL)
    {
        LOG_E("Can't create a block device on '%s' partition.", FS_PARTITION_NAME);
    }
    else
    {
        LOG_D("Create a block device on the %s partition of flash successful.", FS_PARTITION_NAME);
    }

    if (dfs_mount(flash_dev->parent.name, "/", "elm", 0, 0) == 0)
    {
        LOG_I("Filesystem initialized!");
    }
    else
    {
        LOG_E("Failed to initialize filesystem!");
        LOG_D("You should create a filesystem on the block device first!");
    }

    return 0;
}
INIT_ENV_EXPORT(spi_flash_mount);

//static void spi_w25q_sample(int argc, char *argv[])
//{
//    struct rt_spi_device *spi_dev_w25q;
//    rt_uint8_t w25x_read_id = 0x90;
//    rt_uint8_t id[5] = {0};
//
//    /* 查找 spi 设备获取设备句柄 */
//    spi_dev_w25q = (struct rt_spi_device *)rt_device_find(W25Q_SPI_DEVICE_NAME);
//    if (!spi_dev_w25q)
//    {
//        rt_kprintf("spi sample run failed! can't find %s device!\n", W25Q_SPI_DEVICE_NAME);
//    }
//    else
//    {
//        /* 方式1：使用 rt_spi_send_then_recv()发送命令读取ID */
//        rt_spi_send_then_recv(spi_dev_w25q, &w25x_read_id, 1, id, 5);
//        rt_kprintf("use rt_spi_send_then_recv() read w25q ID is:%x%x\n", id[3], id[4]);
//        /* 方式2：使用 rt_spi_transfer_message()发送命令读取ID */
//        struct rt_spi_message msg1, msg2;
//        msg1.send_buf   = &w25x_read_id;
//        msg1.recv_buf   = RT_NULL;
//        msg1.length     = 1;
//        msg1.cs_take    = 1;
//        msg1.cs_release = 0;
//        msg1.next       = &msg2;
//        msg2.send_buf   = RT_NULL;
//        msg2.recv_buf   = id;
//        msg2.length     = 5;
//        msg2.cs_take    = 0;
//        msg2.cs_release = 1;
//        msg2.next       = RT_NULL;
//        rt_spi_transfer_message(spi_dev_w25q, &msg1);
//        rt_kprintf("use rt_spi_transfer_message() read w25q ID is:%x%x\n", id[3], id[4]);
//    }
//}
//MSH_CMD_EXPORT(spi_w25q_sample, spi w25q sample);
