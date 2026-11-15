// SPDX-License-Identifier: GPL-3.0-or-later
// Icon preference belongs to iOS, not the legacy game/save state.
static NSArray<NSDictionary*> *appIconChoices(void) {
 NSData *data=[NSData dataWithContentsOfFile:[NSBundle.mainBundle pathForResource:@"app-icons" ofType:@"json"]];
 return data?[NSJSONSerialization JSONObjectWithData:data options:0 error:nil]:@[];
}
@interface STAppIconPicker : UICollectionViewController<UICollectionViewDelegateFlowLayout>
@property(nonatomic,strong) NSArray<NSDictionary*> *choices;
@property(nonatomic) BOOL changing;
@end
@implementation STAppIconPicker
- (instancetype)init {
 UICollectionViewFlowLayout *layout=[UICollectionViewFlowLayout new];
 layout.sectionInset=UIEdgeInsetsMake(16,16,24,16);layout.minimumInteritemSpacing=12;layout.minimumLineSpacing=12;
 if((self=[super initWithCollectionViewLayout:layout]))self.choices=appIconChoices();
 return self;
}
- (void)viewDidLoad {
 [super viewDidLoad];self.title=@"App icon";
 self.collectionView.backgroundColor=UIColor.systemGroupedBackgroundColor;
 [self.collectionView registerClass:UICollectionViewCell.class forCellWithReuseIdentifier:@"icon"];
 self.navigationItem.rightBarButtonItem=[[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(close)];
 self.collectionView.accessibilityIdentifier=@"app-icon-picker";
}
- (void)close {if(!self.changing)[self dismissViewControllerAnimated:YES completion:nil];}
- (void)viewWillAppear:(BOOL)animated {[super viewWillAppear:animated];[self.collectionView reloadData];}
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
 [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
 [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext>context){[self.collectionView.collectionViewLayout invalidateLayout];} completion:nil];
}
- (NSInteger)collectionView:(UICollectionView*)view numberOfItemsInSection:(NSInteger)section {return self.choices.count;}
- (CGSize)collectionView:(UICollectionView*)view layout:(UICollectionViewLayout*)layout sizeForItemAtIndexPath:(NSIndexPath*)path {
 CGFloat available=view.bounds.size.width-view.adjustedContentInset.left-view.adjustedContentInset.right-32;
 NSInteger columns=MAX(2,MIN(4,(NSInteger)(available/150)));
 CGFloat width=floor((available-12*(columns-1))/columns);
 return CGSizeMake(width,MIN(width-24,128)+66);
}
- (UICollectionViewCell*)collectionView:(UICollectionView*)view cellForItemAtIndexPath:(NSIndexPath*)path {
 UICollectionViewCell *cell=[view dequeueReusableCellWithReuseIdentifier:@"icon" forIndexPath:path];
 for(UIView *sub in cell.contentView.subviews)[sub removeFromSuperview];
 NSDictionary *item=self.choices[path.item];
 BOOL selected=[(UIApplication.sharedApplication.alternateIconName?:@"") isEqualToString:item[@"name"]];
 cell.backgroundColor=UIColor.secondarySystemGroupedBackgroundColor;cell.layer.cornerRadius=16;
 cell.layer.borderWidth=selected?2:0;cell.layer.borderColor=UIColor.systemBlueColor.CGColor;
 CGFloat width=cell.bounds.size.width,side=MIN(width-24,128);
 UIImageView *image=[[UIImageView alloc]initWithImage:[UIImage imageNamed:item[@"preview"]]];
 image.frame=CGRectMake((width-side)/2,12,side,side);image.contentMode=UIViewContentModeScaleAspectFit;
 image.layer.cornerRadius=side*.22;image.clipsToBounds=YES;[cell.contentView addSubview:image];
 UILabel *title=[[UILabel alloc]initWithFrame:CGRectMake(6,side+19,width-12,36)];
 title.text=[NSString stringWithFormat:@"%@%@",selected?@"✓ ":@"",item[@"title"]];title.textAlignment=NSTextAlignmentCenter;
 title.numberOfLines=2;title.font=[UIFont systemFontOfSize:16 weight:UIFontWeightSemibold];title.textColor=UIColor.labelColor;
 [cell.contentView addSubview:title];cell.isAccessibilityElement=YES;cell.accessibilityLabel=item[@"title"];
 cell.accessibilityValue=selected?@"Current app icon":@"";cell.accessibilityTraits=UIAccessibilityTraitButton|(selected?UIAccessibilityTraitSelected:0);
 cell.accessibilityIdentifier=[@"app-icon-" stringByAppendingString:item[@"name"]];return cell;
}
- (void)collectionView:(UICollectionView*)view didSelectItemAtIndexPath:(NSIndexPath*)path {
 [view deselectItemAtIndexPath:path animated:YES];if(self.changing)return;
 UIApplication *app=UIApplication.sharedApplication;NSString *name=self.choices[path.item][@"name"];
 if([(app.alternateIconName?:@"") isEqualToString:name])return;
 if(!app.supportsAlternateIcons){[self showError:@"This installation does not support alternate app icons."];return;}
 self.changing=YES;self.collectionView.userInteractionEnabled=NO;self.navigationItem.rightBarButtonItem.enabled=NO;
 self.modalInPresentation=YES;
 [app setAlternateIconName:name.length?name:nil completionHandler:^(NSError *error){
  dispatch_async(dispatch_get_main_queue(),^{
   self.changing=NO;self.collectionView.userInteractionEnabled=YES;self.navigationItem.rightBarButtonItem.enabled=YES;
   self.modalInPresentation=NO;[self.collectionView reloadData];
   if(error)[self showError:error.localizedDescription?:@"The icon could not be changed. Please try again."];
  });
 }];
}
- (void)showError:(NSString*)message {
 UIAlertController *alert=[UIAlertController alertControllerWithTitle:@"Unable to change icon" message:message preferredStyle:UIAlertControllerStyleAlert];
 [alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
 [self presentViewController:alert animated:YES completion:nil];
}
@end
static void showAppIconPicker(void) {
 if(controller.presentedViewController)return;
 UINavigationController *nav=[[UINavigationController alloc]initWithRootViewController:[STAppIconPicker new]];
 nav.modalPresentationStyle=UIModalPresentationFormSheet;nav.preferredContentSize=CGSizeMake(620,720);
 [controller presentViewController:nav animated:YES completion:nil];
}
