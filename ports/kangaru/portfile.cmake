set(VCPKG_USE_HEAD_VERSION ON)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO sweet-niamhy/kangaru
    REF 19895dac60f6afcc51791e704dbfd8f142f3fd04
    SHA512 0
    HEAD_REF master
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        hashtypeid KANGARU_HASH_TYPE_ID
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DKANGARU_EXPORT=OFF
        -DKANGARU_TEST=OFF
        -DKANGARU_REVERSE_DESTRUCTION=ON
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/kangaru)

file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/lib"
    "${CURRENT_PACKAGES_DIR}/debug"
)

# Put the license file where vcpkg expects it
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
