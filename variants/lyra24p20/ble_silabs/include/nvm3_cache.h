/***************************************************************************//**
 * @file
 * @brief NVM3 object cache
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef NVM3_CACHE_H
#define NVM3_CACHE_H

#include "nvm3_config.h"
#include "nvm3.h"
#include "nvm3_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   TYPES   *************************************
 ******************************************************************************/

typedef bool (*nvm3_CacheScanCallback_t)(nvm3_Cache_t *h, nvm3_ObjectKey_t key, nvm3_ObjGroup_t group, nvm3_ObjPtr_t obj, void *user);

/*******************************************************************************
 ***************************   PROTOTYPES   ************************************
 ******************************************************************************/

void nvm3_cacheOpen(nvm3_Cache_t *h, nvm3_CacheEntry_t *ptr, size_t count);
void nvm3_cacheClear(nvm3_Cache_t *h);
void nvm3_cacheDelete(nvm3_Cache_t *h, nvm3_ObjectKey_t key);
nvm3_ObjPtr_t nvm3_cacheGet(nvm3_Cache_t *h, nvm3_ObjectKey_t key, nvm3_ObjGroup_t *group);
void nvm3_cacheSet(nvm3_Cache_t *h, nvm3_ObjectKey_t key, nvm3_ObjPtr_t obj, nvm3_ObjGroup_t group);

void nvm3_cacheScan(nvm3_Cache_t *h, nvm3_CacheScanCallback_t cacheScanCallback, void *user);
#if defined(NVM3_OPTIMIZATION) && (NVM3_OPTIMIZATION == 1)
sl_status_t nvm3_cacheSort(nvm3_Cache_t *h);
bool nvm3_cacheUpdateEntry(nvm3_Cache_t *h, nvm3_ObjectKey_t key, nvm3_ObjPtr_t obj, nvm3_ObjGroup_t group);
sl_status_t nvm3_cacheAddEntry(nvm3_Cache_t *h, nvm3_ObjectKey_t key, nvm3_ObjPtr_t obj, nvm3_ObjGroup_t group);
sl_status_t nvm3_cacheGetIdx(nvm3_Cache_t *h, nvm3_ObjectKey_t key, size_t low, size_t high, size_t *idx);
void nvm3_cacheOrganize(nvm3_Cache_t *h, size_t idx);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NVM3_CACHE_H */
