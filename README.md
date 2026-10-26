# Space Trader for iOS

Native iPhone/iPad port of Pieter Spronck’s Space Trader 1.2.2, using the original C gameplay and Palm resources with a UIKit compatibility layer. Includes adaptive navigation, the supplied planet icon, and Liquid Glass controls on iOS 26 and later.

Current port release: **1.2.8 (build 9)**. Bundle ID: **com.wireproof.spacetrader**.

## Build

Requires macOS and Xcode with iOS SDK 27.1 or 27.2. No external Python packages required.

```sh
python3 ios/build.py
python3 ios/build.py simulator
```

The device build produces an ad-hoc-signed arm64 IPA in the repository root for LiveContainer or re-signing. The simulator build is under `ios/build/simulator/Payload/SpaceTrader.app`.

Version metadata lives in `ios/version.json`. Increase the patch version and integer build for each delivered update; filenames and Info.plist derive from that file.

## Save compatibility and bundle migration

Builds 1–3 used `com.wireproof.spacetraderclassic`. Build 4 changes to the requested permanent ID. LiveContainer may treat this as a separate app. Retain the old app/data container until migration is confirmed. Use the existing LiveContainer data container for the new app if supported, or copy its `Documents/SpaceTraderClassic-v1` folder into the new app’s Documents. The save format and folder name have not changed. Never delete the old save before checking the new installation.

## Validation

Simulator launch arguments `--selftest` and `--resume-selftest` exercise gameplay, native navigation, save/restore, and fresh-process restoration in isolated test directories. Device builds exclude this harness. The available simulator uses iOS 18.4; Liquid Glass rendering requires device testing. Build 2 launched successfully on the user’s iOS 27.2 phone; later releases require their own device verification.

## Source and licenses

Original game by Pieter Spronck, with Sam Anderson and Samuel Goldstein; original graphics by Alexander Lawrence. Source: https://spronck.net/spacetrader/ . `original/` preserves original source and licensing. `ios/Game/` contains the adapted C source, and `ios/Runtime.m` the iOS compatibility layer. The original game is GPL-2.0-or-later; the compatibility layer is GPL-3.0-or-later. License texts are in `ios/Resources/` and `original/`. Resource-format/palette references: https://github.com/migueletto/PumpkinOS (GPLv3). No Palm ROM is required.

Release 1.2.4: the four configurable Palm shortcuts now appear as native right-side glass buttons, using the original control events. The game canvas renders at 1600×1600 instead of 640×640 to avoid magnifying low-resolution text. Landscape Menu/Help use compact text insets and taller controls.

Release 1.2.5: removes title/banner; uses full available game width with no sidebar deduction or 520-point cap. On layouts exposing a right safe-area margin of at least 60 points, shortcuts are pinned in that outer strip below the status area. Otherwise shortcuts form a row beneath the game. Short viewports scroll vertically, preserving the square game proportions. Duo placement requires device verification.

Release 1.2.6: connected four-option glass shortcut bar with full labels, dividers and selected state. Native grouped Dock/View/Game/Help menus replace the tall action-sheet navigation. Cargo, dock, equipment and chart controls expose related destinations together. Navigation organization reference: https://github.com/SpaceTraderGame/SpaceTrader-Windows/blob/master/Form_SpaceTrader.cs (group boxes and menu definitions); no Windows source code copied. This is navigation adaptation, not a complete replacement of the Palm content panels.

At 700 points and wider, the active game panel sits beside persistent system/ship/cargo context and grouped navigation, following the Windows port’s side-by-side organization. Compact cover layouts retain full-width content.

Release 1.2.7: all navigation in one scrollable right-hand glass bar; content occupies available vertical space. Text is composited separately at its natural aspect ratio while row/control/map positions follow the expanded viewport. Original Palm logical coordinates and gameplay remain. Uploaded Windows 2.00 Form_SpaceTrader.cs and cargo forms were inspected as layout references; no uploaded files or Windows code added to the repository. This is an adaptive compatibility renderer, not a full C# port.

Release 1.2.8: chart, ship information, encounter and title screens preserve square geometry inside the tall viewport. Text, artwork and hit testing use the same fitted rectangle. Other forms retain adaptive vertical spacing. Blank shortcut slots and dividers collapse, and the navigation rail returns to its top when the active form changes.
