# Ship artwork, second remaster

Generated using the built-in image-generation tool from the first remastered ship artwork. The user approved the Gnat and Wasp previews, then requested the same treatment for the entire fleet, both in-game and as selectable app icons.

`icons/` contains opaque square dark forest-green starfield compositions. `transparent/` contains matching alpha cutouts for game presentation. The first remaster remains in `artwork/ships-hd-v1/`; no gameplay statistics or rules change.

## Icon prompt template

Create a premium remastered Space Trader iOS app icon featuring the named spaceship from reference 1. Preserve its exact recognizable silhouette, top-down right-facing orientation, components, proportions and signature colors. Match the dimensional realistic metal, fine panel seams, cinematic edge lighting and luminous details of the approved Gnat in reference 2. Polished detailed sci-fi game artwork rather than a flat sprite. Center whole ship about 78% square canvas width with safe margins. Full bleed opaque nearly black forest-green space background, extremely sparse tiny stars, restrained emerald halo matching reference 2. No planet, rings, text, badges, border, rounded corners, extra ships or invented weapons. Square high-resolution image.

Gnat and Wasp were the approved style prototypes, using their original remastered game ships as references and this same lighting/material/background direction without a second reference.

## Transparent prompt template

Background extraction for the named in-game ship. Remove ONLY the dark green space background, stars and broad atmospheric halo from the attached icon. Preserve this exact ship, silhouette, top-down right-facing orientation, colors, detailed metallic textures, component placement, cinematic highlights and engine lighting. Real transparent alpha background, clean anti-aliased edges, no dark rectangle, no checkerboard. Keep the entire ship fully visible at identical scale and centered placement on the same square canvas; preserve engine emission immediately around thrusters but remove distant glow. Do not redesign or add parts. Transparent PNG.

All edits use image generation, with no procedural recoloring or shape replacement. `ios/ship-art-v2-manifest.json` maps source and runtime resources and records file hashes. `ios/app-icons.json` maps Home Screen selections. Emerald is the primary/default icon. The existing PlanetEmerald identifier remains registered as a compatibility alias for prior selections; Classic is now an alternate.
