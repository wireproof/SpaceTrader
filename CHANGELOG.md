# Presentation and release history

Release 1.2.4: the four configurable Palm shortcuts now appear as native right-side glass buttons, using the original control events. The game canvas renders at 1600×1600 instead of 640×640 to avoid magnifying low-resolution text. Landscape Menu/Help use compact text insets and taller controls.

Release 1.2.5: removes title/banner; uses full available game width with no sidebar deduction or 520-point cap. On layouts exposing a right safe-area margin of at least 60 points, shortcuts are pinned in that outer strip below the status area. Otherwise shortcuts form a row beneath the game. Short viewports scroll vertically, preserving the square game proportions. Duo placement requires device verification.

Release 1.2.6: connected four-option glass shortcut bar with full labels, dividers and selected state. Native grouped Dock/View/Game/Help menus replace the tall action-sheet navigation. Cargo, dock, equipment and chart controls expose related destinations together. Navigation organization reference: https://github.com/SpaceTraderGame/SpaceTrader-Windows/blob/master/Form_SpaceTrader.cs (group boxes and menu definitions); no Windows source code copied. This is navigation adaptation, not a complete replacement of the Palm content panels.

At 700 points and wider, the active game panel sits beside persistent system/ship/cargo context and grouped navigation, following the Windows port’s side-by-side organization. Compact cover layouts retain full-width content.

Release 1.2.7: all navigation in one scrollable right-hand glass bar; content occupies available vertical space. Text is composited separately at its natural aspect ratio while row/control/map positions follow the expanded viewport. Original Palm logical coordinates and gameplay remain. Uploaded Windows 2.00 Form_SpaceTrader.cs and cargo forms were inspected as layout references; no uploaded files or Windows code added to the repository. This is an adaptive compatibility renderer, not a full C# port.

Release 1.2.8: chart, ship information, encounter and title screens preserve square geometry inside the tall viewport. Text, artwork and hit testing use the same fitted rectangle. Other forms retain adaptive vertical spacing. Blank shortcut slots and dividers collapse, and the navigation rail returns to its top when the active form changes.

Release 1.2.9: Commander, Bank and Menu occupy one three-button bottom row. Help remains in Menu. The right bar retains shortcuts, Cargo/Dock/Equipment/Charts and paging. Ship information moves the original sprite into a separate proportional artwork area beneath the square stats panel; short viewports scroll to expose it.

Release 1.2.10: compact dark Liquid Glass navigation with SF Symbols, short captions, white controls and blue selection accents. Dark material fallback on older iOS. Toolbar height follows visible content. Ship preview and three-button bottom row retained; no emulated Palm hardware or Graffiti area.

Release 1.2.11: follows system appearance, including native backgrounds, labels, menus, glass navigation and classic game canvas. Neutral canvas colors are mapped for dark contrast; ship preview retains original colors. Theme changes refresh without relaunch. Hardware-style arrows are removed; previous/next page or system actions remain under Menu > Paging.


## Native presentation — 1.3.1 (build 13)

The visible Palm bitmap display is replaced by UIKit screens in `ios/NativeScreens.inc`: native cargo rows and trade controls, adaptive information/action cards, native input prompts, proportional ship previews, and vector charts with native star/wormhole targets. Wide layouts add commander/ship context and a nearby-systems panel beside the active screen. Compact layouts scroll naturally and preserve the Duo rail plus three-button footer. System light/dark colors are used throughout.

The original C rules and binary save layout remain. The compatibility canvas still receives legacy drawing calls internally, but is no longer displayed or scaled into the interface. Less-common forms are translated into native rows from their current text/control state; they are not individually rewritten gameplay implementations. This is an iOS presentation rewrite, not a port of Windows-specific additional ships or rules.

Reference examined: user-supplied SpaceTrader201.zip, including Windows documentation and a REA static inspection of SpaceTrader.exe. REA identified assembly version 2.1.3146.26288, SHA-256 b61fdfc7094d8f3a3ed2a1019c13f1e46aa9d90e64e19de461a442abf44e3395. It did not execute the Windows application. No Windows executable, save data, or external artwork is redistributed here.

Validation covers native trade actions, quantity dialogs, ship/commander/equipment navigation, native map target selection, travel/encounters, save/restore, and fresh-process restore in isolated simulator data. Visual checks use the Duo simulator and a larger iPad viewport; physical-device testing remains separate. Release built with SDK 27.2.

### 1.3.1 layout refinement

Continuous narrative text reflows into a single card. Charts use the main content width without the duplicate commander panel. The right rail shows one set of four destinations, with additional actions available through Menu. Simulator trade, navigation, encounter and save checks passed.

### 1.4.0 — shared trading overview

System, buy, sell, and shipyard destinations now share a responsive overview inspired by the supplied Windows screenshots. Wide layouts put system/dock information beside a combined cargo market, with galaxy, nearby, and destination panels below. Narrow layouts stack those panels and use larger commodity action rows. Panels scroll when needed to preserve touch targets. Stars select a destination; Review course opens the existing travel flow. Quantity dialogs and special events appear above a dimmed overview.

The original game engine and save format remain. This revision does not implement Windows-only target-price comparison columns or replace every secondary screen. Simulator verification covers combined buy/sell controls, destination panel, compact event presentation, navigation, encounters, travel and save restoration. Physical appearance requires device review.

### 1.4.1 — viewport fit

The wide overview fits its two panel rows above the footer. Market, system/dock and selected-target contents scroll within their panels, with offsets retained on redraw. Empty targets use a compact panel on narrow screens. Ship and commander information rows are denser so ship artwork fits beneath the information. Simulator checks include an 860 x 465 point overview fit assertion and the original gameplay/save suite.

### 1.5.0 — focused navigation and HD fleet

Includes ten transparent remastered normal-hull ship illustrations, used by the catalog, ship preview and matching encounter sprites. Original damaged/shielded artwork is retained. Market headings and system actions stay outside their scrolling content. Cover navigation separates market from system/dock services; Yard opens dock services directly. The native ship catalog has thumbnails, original-engine purchase actions, and a side-by-side detail pane on wide screens. Small charts show fewer labels and their header opens the full chart. The original rules and save format remain unchanged. Physical delivery is install-only at the user's request.

### 1.5.1 (build 18)

Buy and Sell shortcuts now open distinct cargo actions. The overview retains its combined market. Landscape ship information places the unchanged HD artwork beside the statistics, while portrait keeps it underneath. The content area gains 20 points of height, chart labels stay within panel edges, and unused cover dock space displays the current ship. The original engine and save format are unchanged.

Simulator checks cover distinct Buy/Sell controls, trading, travel, encounters, save restoration, and the enlarged landscape artwork. Device delivery is install-only; physical visual confirmation remains with the user.

### 1.5.2 (build 19)

Options now uses native labeled switches with scrolling settings and pinned Done/page-navigation buttons. System details and action titles are larger; standalone generic action buttons have a capped width. The layouts, HD artwork, original rules, and save format are retained. Simulator checks verify switch state reaches the original options model, both options pages open and close, and existing trading/travel/save tests pass. Delivery remains install-only on Duo.

### 1.5.3 (build 20)

Portrait trading uses one title, compact commodity rows, a cargo/cash summary, and plain availability text instead of disabled button blocks. Portrait docking combines the current ship artwork, ship status, and service actions above system details. Landscape retains its dashboard with enough height for all seven system details and spaced label/value columns. Ship catalog specifications use aligned rows. The approved HD assets and original game rules/save format are unchanged.

### 1.5.4 (build 21)

Yard is a dedicated shipyard in both orientations, without cargo trading or charts. Current ship information displays fuel and hull against actual capacity plus segmented cargo usage. Ship catalogs use responsive tiles with HD artwork and quick specs; Info opens the detailed specification view and purchase control. Buy Equipment uses the same tile/detail flow with original Windows 2.01 equipment illustrations recovered from the user-supplied executable (see ios/equipment-art-provenance.json). Equipment illustrations retain their original pixel art; approved remastered ship files are unchanged. Original purchase handlers, availability rules and saves remain authoritative.

### 1.5.5 (build 22)

Shipyard now shows occupied weapon, shield, gadget and crew capacity with segmented indicators, including a compact two-column portrait panel. Buy/Sell cargo has a persistent cargo-capacity meter and free-space summary. Current Ship now presents HD ship art, live fuel/hull/cargo gauges, equipment capacity and an illustrated installed loadout. All ten standard equipment icons are remastered using built-in image generation; prompts and asset paths are in `ios/equipment-hd-manifest.json`. Approved ship artwork, original game actions and save format are unchanged. Simulator checks cover the current-ship view, live slot counts, trade, travel, options and save/restore. Duo delivery is install-only.

### 1.5.6 (build 23)

Buy and Sell now show ten themed cargo illustrations beside commodity names, with unchanged trade actions and compact row sizing. Small semantic symbols accompany ship gauges, dashboard headings and service/navigation actions where space permits. Removed the commander explanatory note from Equipment & Crew. Approved ship and equipment art is retained. Cargo assets are generated with built-in image generation; prompts and file mappings are in `ios/cargo-art-manifest.json`. Original rules and save format are unchanged.

### 1.5.7 (build 24)

Adds a native app-icon picker to Menu and Options. Includes Classic, three remastered green-planet designs, and all ten ship designs with consistent dark teal space backgrounds. iPhone and iPad alternate-icon declarations and appropriately sized resources are generated from a shared manifest. The current selection is read from iOS; failures are shown without changing saves. Approved in-game artwork and gameplay remain unchanged.

### 1.5.8 (build 25)

All ten ships now use the approved second-remaster style: textured metallic panels, stronger dimensional highlights and luminous accents. Matching transparent artwork appears in the existing game views; dark forest-green starfield versions are selectable Home Screen icons. Emerald becomes the default, Classic remains selectable, and the prior Emerald alternate identifier remains registered for compatibility. Gameplay, ship statistics, layout and save format are unchanged.

## 1.5.9 (build 26)

Combined buying and selling into one Trade page with commodity artwork and cargo capacity. Current-system details, nearby systems and target details sit together below the market; the full galaxy map is available from Warp. Commander status moves to the top, while Trade, Yard, Warp, Commander, Bank and Menu share the side bar. The welcome screen now centers full-resolution Emerald artwork and start/help actions without gameplay controls. Ship and icon artwork and save format are unchanged.

## 1.5.10 (build 27)

- Combined nearby systems and galaxy on Warp, with shared destination routing.
- Combined commander status, crew roster, current ship gauges and installed equipment artwork. Crew hire/dismiss actions retain original game rules. Removed unrelated charts from the roster.
- Replaced the compact top commander summary with ship type, cargo, fuel and hull; tapping it opens the combined overview.
- Matching Trade, Dock, Warp, Bank and Options navigation: right rail on Duo, bottom tabs on other devices.
- Preserved approved artwork, bundle identity and save format.

## 1.5.11 (build 28)

- Full-page Trade with larger cargo artwork; system information and charts move to Warp.
- Unified Dock, commander, current ship and crew overview, with installed equipment images below Equipment & crew. Redundant Dock header stats removed.
- Nearby chart, current and target system comparison, then galaxy. Star selection remains on the page and highlights the target; Review course opens travel details.
- Native Bank balance, credit and insurance cards retain original transaction rules.
- Back navigation uses existing cancel/done controls for pending actions and history for navigation; combat cannot be bypassed.
- Flat Options menu containing game actions and App icon. Duo rail aligns with the system safe-area strip; other devices retain matching bottom navigation.


## Dock and navigation polish — 1.5.12 (build 29)

- Group installed equipment images inside Equipment & crew, directly above Manage equipment.
- Present commander and individual crew skills as labeled meters with clear role and hiring information.
- Align Review course with the current system action row.
- Remove the redundant ship summary from Trade, Warp and Bank.
- Use a native navigation bar for Back and keep Duo app navigation inside the safe content area, separate from system-owned status controls. iOS retains control of status-indicator placement.
- Preserve all approved artwork, game rules and save formats.

Validation: the final iPad simulator build passed gameplay, native Back, save/restore and layout-structure checks; Warp action alignment and the integrated equipment card were visually inspected. Duo simulator installation timed out under host load, so Duo visual validation remains pending. Both physical devices received the update with matching pre/post saved-data hashes; physical launch, runtime and visual testing were not performed.


## Native system tabs — 1.5.13 (build 30)

Replaces the manually positioned navigation rail with UITabBarController, following WireDRIVE's use of standard system tabs. iOS now owns navigation placement and safe-area allocation, including the Duo system column. Trade, Dock, Warp, Bank and Options share the original game engine through a single contained game controller. Options is a dedicated native destination; Back retains the original game history and gains enough header clearance for its system glass button. Approved artwork and saves are unchanged.

Validation for 1.5.13: Duo and iPad simulator gameplay, native navigation, Options and fresh-process save restoration passed. System screenshots confirmed Duo navigation in the system column and iPad bottom tabs.

## 1.5.14 (31)

Circular glass Back control with a 44-point touch target. Only the Duo cover display is restricted to portrait; the inner display and other devices retain rotation. Display classification uses physical screen dimensions rather than the content window.

## 1.5.15 (32)

Places the circular Back button above the native Duo navigation bar and reclaims the top content space. Warp restores Find system through the original search dialog and original cheat handlers.
Commander and crew share one card; the commander is listed once, with personal and effective skill values together.
