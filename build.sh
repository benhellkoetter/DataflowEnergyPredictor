set -euo pipefail

CXX_COMPILER=${CXX_COMPILER:-clang++-15}
C_COMPILER=${C_COMPILER:-clang-15}
LTO_FLAGS="-flto"

BUILD_DIR="build"

if [[ "${1:-}" == "clean" ]]; then
  echo "==> Cleaning build directory"
  rm -rf "${BUILD_DIR}"
  exit 0
fi

if [[ ! -d "${BUILD_DIR}" ]]; then
  echo "==> Creating ${BUILD_DIR}/"
  mkdir "${BUILD_DIR}"
fi

cd "${BUILD_DIR}"

echo "==> Running cmake ..."
cmake \
  -DCMAKE_CXX_COMPILER="${CXX_COMPILER}" \
  -DCMAKE_C_COMPILER="${C_COMPILER}" \
  -DCMAKE_EXE_LINKER_FLAGS="${LTO_FLAGS}" \
  -DCMAKE_SHARED_LINKER_FLAGS="${LTO_FLAGS}" \
  ..

echo "==> Building with $(nproc) parallel jobs..."
make -j"$(nproc)"

echo "==> Build finished successfully!"
