# Install script for directory: /Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Users/jasperwei/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32s3-elf-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_dependencies.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/Users/jasperwei/esp/v5.4.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

