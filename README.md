# lulzbot-mini1-marlin-2x-bltouch

Marlin firmware for the LulzBot Mini (1st gen, with the graphic LCD), on a
**real Marlin 2.x core**, intended to get BLTouch auto bed-leveling probe
support added the same way it was added on the 1.x codebase.

Board: Mini-RAMBo.

## Relationship to the 1.x repo

This is a **separate, independent repo** from
[`lulzbot-mini1-marlin-bltouch`](https://github.com/ckirkpatrick1984/lulzbot-mini1-marlin-bltouch)
(no shared git history). That repo has working BLTouch support flashed to
hardware, but its firmware base turned out to be Marlin core **1.1.9**, not
2.x, despite the "2.0.7" in the original download URL - that number was
LulzBot's own Mini firmware/software release counter, not the Marlin core
version. See that repo's `README.md` / the umbrella `PROJECT.md` for the
full story.

This repo exists to redo the same BLTouch work on an actual Marlin 2.x
base. **The 1.x repo is not being replaced or modified by this one** - both
are kept, side by side, as independent checkouts in the parent
`lulzbot-marlin-bltouch` workspace folder.

## Base source (verified, not guessed)

- **GitLab tag `v2.0.0.144`** on `gitlab.com/lulzbot3d/marlin` (tagged
  2019-07-12). `Marlin/src/inc/Version.h` at that tag defines
  `SHORT_BUILD_VERSION "2.0.0" LULZBOT_FW_VERSION`, and the tree uses
  Marlin 2.x's reorganized `Marlin/src/` layout - confirmed present in this
  checkout, unlike the 1.x repo's flat `Marlin/*.cpp` layout. This is a
  genuine Marlin 2.x core, not a mislabeled 1.x build.
- **Download mirror**:
  `download.lulzbot.com/Software/Marlin/2.0.0.144/Marlin-src_2.0.0.144_aded3b617.zip`
  (commit `aded3b617`, dated 2019-07-22). This is what was actually pulled
  into this repo.
- `Marlin/Configuration_LulzBot.h` at this version lists `Gladiola_Mini`
  and `Gladiola_MiniLCD` (LulzBot Mini 1st gen) as supported printer
  defines, matching this printer.

LulzBot's own build/dev-branch docs are preserved at
[README_LulzBot.md](README_LulzBot.md); stock upstream Marlin's README is
preserved at [README_Marlin.md](README_Marlin.md).

## Current state

Printer model is set in `Marlin/Configuration_LulzBot.h`:
`#define LULZBOT_Gladiola_MiniLCD` (the LCD variant of the Mini - these
printers have the graphic LCD, not the headless/bare-board variant), with
`TOOLHEAD_Gladiola_SingleExtruder` selected as the toolhead - the same
printer/toolhead combination used in the working 1.x repo. This is
otherwise the LulzBot 2.0.0.144 core with a BLTouch conversion on top:
`LULZBOT_USE_BLTOUCH` (in the same file) switches the Mini from its stock
nozzle-contact probe to a BLTouch, which is now the sole Z reference
(Z homes down at bed center, the stock rewipe/retry-on-probe-failure
workflow is disabled). **The BLTouch works on real hardware** (confirmed
2026-09-02). Decision history and open questions are in the umbrella
`PROJECT.md` in the `lulzbot-marlin-bltouch` workspace folder.

## Prebuilt firmware

A ready-to-flash **`firmware.hex`** is committed at the repo root, built
from the current `master` with `LULZBOT_USE_BLTOUCH` enabled
(`python3 -m platformio run -e rambo`; Flash 67.2%, RAM 67.7%). With
`LULZBOT_USE_BLTOUCH` commented out the same tree builds the stock
configuration (Flash 65.7%, RAM 66.6%).

**Read this before flashing it.** This build is not a drop-in for a stock
Mini. It assumes a specific, physically modified machine:

- **The mechanical Z endstop is gone.** The BLTouch is the *sole* Z
  reference for both homing and probing; Z homes down with `Z_SAFE_HOMING`
  at bed center.
- **BLTouch wiring:** 2-pin trigger/signal on the Z-min endstop header
  (`Z_MIN_PIN`, pin 10); 3-pin servo/control on pin 23 (`SERVO0_PIN`,
  formerly Z-max).
- **Probe offsets are specific to this mount**: X `+47`, Y `-36` (measured
  46.9 mm right / 35.7 mm in front, rounded - the 2.0 fork requires
  integers). Your mount will almost certainly differ. The `+47` X offset
  also limits the probed mesh to X 50-145.
- **The Z probe offset (`-3.0`) is a ruler measurement, not a calibrated
  one.** Dial it in with an `M851` paper test, then `M500`.

**Verify `M119` shows the probe actually toggling before running `G28`** -
an unconnected or miswired probe reads as "never triggered," and Z homing
will then drive the nozzle into the bed.

After flashing, run **`M502` then `M500`** to load and save the compiled
defaults. EEPROM values (probe offset, PID, steps/mm, mesh) survive a
flash and will otherwise silently shadow the firmware's settings.

# Safety and warnings:

**This repository may contain untested software.** It has not been extensively tested and may damage your printer and present other hazards. Use at your own risk. Do not operate your printer while unattended and be sure to power it off when leaving the room. Please consult the documentation that came with your printer for additional safety and warning information.
