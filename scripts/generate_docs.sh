#!/usr/bin/env bash
set -euo pipefail

(
  cd ..
  doxygen Doxyfile
)

echo "Docs generated in docs/doxygen"
