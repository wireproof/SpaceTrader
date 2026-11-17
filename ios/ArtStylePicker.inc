// A presentation preference, separate from the original game save and app icon.
@interface STArtStylePicker : UITableViewController
@end
@implementation STArtStylePicker
- (void)viewDidLoad{[super viewDidLoad];self.title=@"Art Style";self.tableView.rowHeight=110;self.navigationItem.rightBarButtonItem=[[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(close)];}
- (void)close{dirty=YES;[self dismissViewControllerAnimated:YES completion:nil];}
- (NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section{return 3;}
- (NSString*)tableView:(UITableView*)tableView titleForFooterInSection:(NSInteger)section{return @"Changes ship artwork throughout the game. Original Upscaled also uses original equipment. Both remasters share equipment art; cargo illustrations are shared by all styles. App icons are chosen separately.";}
- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)path{UITableViewCell*c=[[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];c.textLabel.text=@[@"Original Upscaled",@"Remaster 1",@"Remaster 2"][path.row];c.detailTextLabel.text=@[@"Classic pixel artwork",@"Clean illustrated shading",@"Detailed metallic shading"][path.row];c.imageView.image=shipArtForStyle(1,path.row);c.imageView.contentMode=UIViewContentModeScaleAspectFit;c.accessoryType=selectedArtStyle()==path.row?UITableViewCellAccessoryCheckmark:UITableViewCellAccessoryNone;return c;}
- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)path{[NSUserDefaults.standardUserDefaults setInteger:path.row forKey:@"STArtStyle"];shipPreview=nil;[nativeArtwork removeAllObjects];dirty=YES;[tableView reloadData];}
@end
static void showArtStylePicker(void){if(controller.presentedViewController)return;STArtStylePicker*p=[[STArtStylePicker alloc]initWithStyle:UITableViewStyleInsetGrouped];UINavigationController*nav=[[UINavigationController alloc]initWithRootViewController:p];nav.modalPresentationStyle=UIModalPresentationFormSheet;nav.preferredContentSize=CGSizeMake(560,480);[controller presentViewController:nav animated:YES completion:nil];}
