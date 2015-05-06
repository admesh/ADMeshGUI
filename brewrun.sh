#!/bin/bash

realpath() {
  if [ -d "$(dirname "$1")" ]; then
    echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
  fi
}

FILES=()
for f in "$@"; do
    FILES+=("$(realpath "$f")")
done

open -n $(brew --prefix)/opt/admeshgui/ADMeshGUI.app --args "${FILES[@]}"
