#!/usr/bin/env python3
import subprocess,sys,plistlib,shutil,concurrent.futures,zipfile,json
from pathlib import Path
root=Path(__file__).resolve().parent
version=json.loads((root/'version.json').read_text())
mode=sys.argv[1] if len(sys.argv)>1 else 'device'
sdk='iphonesimulator' if mode=='simulator' else 'iphoneos'
target='arm64-apple-ios16.0'+('-simulator' if mode=='simulator' else '')
path=subprocess.check_output(['xcrun','--sdk',sdk,'--show-sdk-path'],text=True).strip()
build=root/'build'/mode;build.mkdir(parents=True,exist_ok=True)
app=build/'Payload'/'SpaceTrader.app';app.mkdir(parents=True,exist_ok=True)
flags=['xcrun','clang','-target',target,'-isysroot',path,'-std=gnu99','-O1','-g','-DBPP8','-Wno-implicit-int','-Wno-implicit-function-declaration','-Wno-int-conversion','-Wno-pointer-sign','-I',str(root/'compat'),'-I',str(root/'Game')]
sources=[p for p in (root/'Game').glob('*.c') if p.name!='SetField.c']+[root/'Runtime.m']
def compile(p):
 obj=build/(p.stem+'.o');r=subprocess.run(flags+(['-fobjc-arc'] if p.suffix=='.m' else [])+['-c',str(p),'-o',str(obj)],capture_output=True,text=True)
 (build/(p.stem+'.log')).write_text(r.stderr)
 if r.returncode:print(r.stderr);raise RuntimeError(p)
 return str(obj)
objects=list(concurrent.futures.ThreadPoolExecutor(8).map(compile,sources))
subprocess.run(flags+objects+['-framework','UIKit','-framework','Foundation','-framework','CoreGraphics','-framework','QuartzCore','-o',str(app/'SpaceTrader')],check=True)
for p in (root/'Resources').iterdir():shutil.copy2(p,app/p.name)
info=dict(CFBundleExecutable='SpaceTrader',CFBundleIdentifier=version['bundle_id'],CFBundleName='Space Trader',CFBundleDisplayName='Space Trader',CFBundleVersion=str(version['build']),CFBundleShortVersionString=version['version'],CFBundlePackageType='APPL',MinimumOSVersion='16.0',LSRequiresIPhoneOS=True,UIDeviceFamily=[1,2],UILaunchScreen={},UIStatusBarStyle='UIStatusBarStyleDefault',UISupportedInterfaceOrientations=['UIInterfaceOrientationPortrait','UIInterfaceOrientationLandscapeLeft','UIInterfaceOrientationLandscapeRight'],UIRequiresFullScreen=False,UIUserInterfaceStyle='Light',CFBundleIcons={'CFBundlePrimaryIcon':{'CFBundleIconFiles':['AppIcon120','AppIcon180']}},LSSupportsOpeningDocumentsInPlace=True,UIFileSharingEnabled=True,ITSAppUsesNonExemptEncryption=False)
info['UIApplicationSceneManifest']={'UIApplicationSupportsMultipleScenes':False,'UISceneConfigurations':{'UIWindowSceneSessionRoleApplication':[{'UISceneConfigurationName':'Space Trader','UISceneDelegateClassName':'STClassicSceneDelegate'}]}}
info['CFBundleIcons~ipad']={'CFBundlePrimaryIcon':{'CFBundleIconFiles':['AppIcon120','AppIcon152','AppIcon167','AppIcon180']}}
(app/'Info.plist').write_bytes(plistlib.dumps(info))
subprocess.run(['codesign','--force','--sign','-','--timestamp=none',str(app)],check=True)
if mode!='simulator':
 sdk_version=subprocess.check_output(['xcrun','--sdk',sdk,'--show-sdk-version'],text=True).strip()
 ipa=root.parent/f"SpaceTrader-{version['version']}-build{version['build']}-iOS{sdk_version}.ipa"
 with zipfile.ZipFile(ipa,'w',zipfile.ZIP_DEFLATED) as z:
  for p in app.rglob('*'):
   if p.is_file():z.write(p,p.relative_to(build))
 print(ipa)
print(app)
