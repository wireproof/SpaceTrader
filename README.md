# Space Trader for iOS

A native iPhone and iPad adaptation of Pieter Spronck’s Space Trader 1.2.2. The original C game engine runs behind an adaptive UIKit interface, with illustrated ship and equipment catalogs, cargo trading, galaxy charts, encounters, quests, banking and saved games. All gameplay resources are bundled; no Palm ROM or online service is required.

## Current release

**1.5.8 (build 25)** — [Download the release](https://github.com/wireproof/SpaceTrader/releases/tag/v1.5.8).

- Ten ships with a second pass of detailed metallic artwork, ten equipment icons and ten themed cargo icons.
- Separate Buy and Sell screens with cargo-capacity indicators.
- Dedicated shipyard and current-ship views with fuel, hull, equipment and crew gauges.
- Responsive ship/equipment tiles and detail views, portrait and landscape layouts, native options and system light/dark appearance.
- Original trading, travel, combat and save rules retained.

This release brings the approved second-remaster finish to all ten ships, both in-game and as Home Screen icons. Emerald is now the default app icon. Menu > App icon (also available in Options) offers the full fleet, the other planet designs and Classic. Icon selection is managed by iOS and does not alter game saves. See [CHANGELOG.md](CHANGELOG.md) for previous presentation changes.

## Build and install

Requires macOS, Xcode with iOS SDK 27.1 or 27.2, and Python 3. No external Python packages are required.

```sh
python3 ios/build.py
python3 ios/build.py simulator
```

The device build produces an arm64 IPA in the repository root. The published IPA is ad-hoc signed for LiveContainer or re-signing; direct Home Screen installation requires a suitable development signature and device provisioning. Minimum supported OS is iOS 16. Liquid Glass controls are used on iOS 26 and later.

Version metadata is in `ios/version.json`. The permanent bundle ID is `com.wireproof.spacetrader`, and the app name is **Space Trader**. Device builds exclude the simulator test harness.

## Source and artwork

`ios/Game/` contains adapted gameplay source; `ios/Runtime.m` and the included native screen files implement the iOS interface. `original/` preserves upstream source and licensing. `SpaceTrader_color.prc` supplies the original Palm resources.

All runtime assets are included in `ios/Resources/`: original sprites, equipment reference artwork, the app icon, and all 30 remastered ship/equipment/cargo images. `artwork/ships-hd-v1/` preserves the first remaster. `artwork/ships-hd-v2/` includes the current opaque app icons, transparent game artwork and generation prompts; `ios/ship-art-v2-manifest.json` records resource mappings and hashes. `artwork/app-icon-remastered/` contains all 13 alternate-icon source images and provenance; `ios/app-icons.json` maps them to the picker. Equipment and cargo prompts and mappings are recorded in `ios/equipment-hd-manifest.json` and `ios/cargo-art-manifest.json`; equipment provenance is in `ios/equipment-art-provenance.json`.

## Saves

Save data remains in `Documents/SpaceTraderClassic-v1`. Keep this folder when updating. Very early builds used `com.wireproof.spacetraderclassic`; migrating from that identifier requires retaining or copying the old save container before removing the earlier app.

## Validation

Build 25 passed gameplay and fresh-process save restoration checks on the iPhone Duo (iOS 27.1) and 11-inch iPad Pro (iOS 18.4) simulators. The ship catalog, current-ship artwork, and icon picker were visually reviewed. All 14 visible icon choices loaded, both simulators reported alternate-icon support, and Emerald appeared as the default. This update did not repeat every alternate-icon switching transition.

Installed-app readback confirmed 1.5.8 (25) on the iPhone Duo and 11-inch iPad Pro. Saved-data and preference backup hashes were unchanged after installation; the iPad directories were empty before and after. Delivery was install-only: physical-device launch, runtime testing and visual confirmation were not performed. Simulator checks do not establish physical-device behavior.

## Credits and licenses

Original game by Pieter Spronck, with Sam Anderson and Samuel Goldstein; original graphics by Alexander Lawrence. [Original project](https://spronck.net/spacetrader/). The original game is GPL-2.0-or-later; the compatibility layer is GPL-3.0-or-later. License texts are in `ios/Resources/` and `original/`. Resource-format and palette references: [PumpkinOS](https://github.com/migueletto/PumpkinOS). The native presentation retains the original engine rather than implementing Windows-only rules.
