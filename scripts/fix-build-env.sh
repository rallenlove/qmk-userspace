# Source this before running `make` directly for a QMK build (not needed for `qmk compile`/`qmk flash`,
# which already handle both of these).
#
#   source scripts/fix-build-env.sh
#
# Fixes two things that break cross-compiling for the RP2040 toolchain:
#
# 1. CFLAGS/CPPFLAGS/CPATH/LDFLAGS are set globally in this shell (from some other dev environment,
#    e.g. a Homebrew openssl setup) and leak into the QMK build's compiler/linker invocation, breaking
#    it in several different ways (a Clang-only flag rejected by GCC, and the macOS SDK's headers
#    shadowing the ARM toolchain's own headers).
# 2. QMK's own managed toolchain (installed via `qmk doctor`/`qmk setup`) lives under
#    ~/.local/share/qmk/bin, which isn't on PATH by default outside of the `qmk` CLI wrapper.

unset CFLAGS CPPFLAGS CPATH LDFLAGS

QMK_TOOLCHAIN_BIN="$HOME/.local/share/qmk/bin"
case ":$PATH:" in
  *":$QMK_TOOLCHAIN_BIN:"*) ;;
  *) export PATH="$QMK_TOOLCHAIN_BIN:$PATH" ;;
esac

export QMK_USERSPACE="/Users/rosslovelace/code/qmk-userspace"

# Run from inside the qmk_firmware checkout. `lily58-build` compiles only;
# `lily58-flash` also flashes (put the board in bootloader mode first).
alias lily58-build='make lily58/rev1:rallenlove -e USER_NAME=holykeebs -e POINTING_DEVICE=trackpoint -e POINTING_DEVICE_POSITION=right -e OLED=yes -j12'
alias lily58-flash='make lily58/rev1:rallenlove:flash -e USER_NAME=holykeebs -e POINTING_DEVICE=trackpoint -e POINTING_DEVICE_POSITION=right -e OLED=yes -j12'

echo "Build env fixed: unset CFLAGS/CPPFLAGS/CPATH/LDFLAGS, added $QMK_TOOLCHAIN_BIN to PATH"
echo "Aliases ready: lily58-build, lily58-flash (run from inside qmk_firmware)"
