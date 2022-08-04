/*
  Copyright 2019, Awesome Audio Apparatus.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
      limitations under the License.
*/

#include "lfs.h"
#include "demiurge.h"
#include <stdio.h>

int boot_counter() {

    lfs_file_t file;

    uint32_t boot_count = 0;
    lfs_file_open(&demiurge_flash_fs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&demiurge_flash_fs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&demiurge_flash_fs, &file);
    lfs_file_write(&demiurge_flash_fs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&demiurge_flash_fs, &file);

    return boot_count;
}
