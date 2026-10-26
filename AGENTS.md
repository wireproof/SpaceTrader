# Space Trader iOS

Use bundle ID `com.wireproof.spacetrader` for all future releases.
Increment both the patch version and build number in `ios/version.json` for each delivered update. Rebuilds of the same release retain that version. Never reuse an already delivered version for changed code.
Preserve the Documents/SpaceTraderClassic-v1 save format and data.
Build with iOS SDK 27.1 or 27.2. Keep simulator tests out of device builds.
Never commit crash reports, device identifiers, local save data, credentials, or build output.
