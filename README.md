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
otherwise a **stock, unmodified 2.0.0.144 checkout** - no BLTouch config
has been added yet. That's tracked as follow-up work; see the umbrella
`PROJECT.md` in the `lulzbot-marlin-bltouch` workspace folder for goals,
background, and open questions across both Mini 1 repos.

# Safety and warnings:

**This repository may contain untested software.** It has not been extensively tested and may damage your printer and present other hazards. Use at your own risk. Do not operate your printer while unattended and be sure to power it off when leaving the room. Please consult the documentation that came with your printer for additional safety and warning information.
